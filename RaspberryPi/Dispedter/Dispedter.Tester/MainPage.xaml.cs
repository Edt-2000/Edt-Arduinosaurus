using Dispedter.Common.OSC;
using Dispedter.Common.OSCArduino;
using Dispedter.Common.Factories;
using Dispedter.Common.DMX;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Diagnostics;
using Dispedter.Common.Managers;
using Windows.UI;
using System.Collections.ObjectModel;
using System.Net;
using Windows.Storage;
using Windows.Storage.Streams;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Dispedter.Tester
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private readonly CommandFactory _commandFactory = new CommandFactory(new[] { "/F?", "/R?" });
        private readonly CommandFactory _specialCommandFactory = new CommandFactory(new[] { "/?1", "/?2", "/?3", "/?4", "/?5", "/?6", "/?7", "/?8" });
        private readonly ListenerManager _listenerManager = new ListenerManager(detectUsb: false);
        private readonly SenderManager _senderManager = new SenderManager(detectUsb: false, udpDestinations: new[] { IPAddress.Parse("169.254.219.81")/*, IPAddress.Parse("169.254.219.93")*/ });

        private Dictionary<Mode, Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>> _commandMapping = new Dictionary<Mode, Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>>();
        private Dictionary<Mode, Dictionary<VirtualKey, Func<int, (int delay, IEnumerable<OscMessage> command)>>> _proceduralCommandMapping = new Dictionary<Mode, Dictionary<VirtualKey, Func<int, (int delay, IEnumerable<OscMessage> command)>>>();

        private ObservableCollection<DmxSlave> _dmxSlaves;
        private ObservableCollection<DmxType> _dmxTypes;
        private ObservableCollection<int> _dmxAddresses;
        private DmxConfig _dmxConfig;

        private Mode _mode;
        private enum Mode
        {
            Default = 0,
            Partial = 1,
            Chase = 2
        }

        private Task _senderTask;
        private Task _listenerTask;

        private DateTime _previousOut = DateTime.UtcNow;
        private List<string> _outHistory = new List<string>();
        private DateTime _previousIn = DateTime.UtcNow;
        private List<string> _inHistory = new List<string>();


        enum CommandDirection
        {
            In,
            Out
        }

        public MainPage()
        {
            _dmxSlaves = new ObservableCollection<DmxSlave>();
            _dmxConfig = new DmxConfig(_dmxSlaves);
            _dmxTypes = new ObservableCollection<DmxType>(_dmxConfig.Types);
            _dmxAddresses = new ObservableCollection<int>(Enumerable.Range(1, 512));
            
            InitializeComponent();
            InitializeListeners();

            _senderTask = _senderManager.ManageDevicesAsync();
            _listenerTask = _listenerManager.ManageDevicesAsync();

            InitializeCommandMappings();

            Window.Current.CoreWindow.KeyDown += async (s, e) =>
            {
                var key = e.VirtualKey;

                KeyCode.Text = $"{(int)key}";

                if (e.VirtualKey == VirtualKey.Control)
                {
                    _mode++;

                    if ((int)_mode >= _commandMapping.Count)
                    {
                        _mode = 0;
                    }

                    CommandMode.Text = _mode.ToString();
                }
                else
                {
                    await SendCommandAsync(key);
                }
            };
        }

        private void InitializeListeners()
        {
            _listenerManager.AttachEventHandler(async (IListener listener, OscEventArgs args) =>
            {
                await LogCommandAsync(CommandDirection.In, new[] { args.GetOscPacket() as OscMessage });
            });
        }

        #region LED
        private async Task SendCommandAsync(VirtualKey key)
        {
            if (!_senderManager.Senders?.Any() ?? false)
            {
                return;
            }

            if (_commandMapping[_mode].TryGetValue(key, out var commandGenerator))
            {
                var command = commandGenerator();

                foreach (var sender in _senderManager.Senders)
                {
                    await sender.SendAsync(command);
                }

                await LogCommandAsync(CommandDirection.Out, command);
            }
            else if (_proceduralCommandMapping[_mode].TryGetValue(key, out var proceduralCommandGenerator))
            {
                var i = 0;
                do
                {
                    var (delay, command) = proceduralCommandGenerator(i);

                    foreach (var sender in _senderManager.Senders)
                    {
                        await sender.SendAsync(command);
                    }

                    await LogCommandAsync(CommandDirection.Out, command);

                    await Task.Delay(delay);

                } while (++i < 100);
            }
            else
            {
                ;
            }
        }

        private async Task LogCommandAsync(CommandDirection commandDirection, IEnumerable<OscMessage> messages)
        {
            if (commandDirection == CommandDirection.Out)
            {
                foreach (var message in messages)
                {
                    var delta = DateTime.UtcNow - _previousOut;
                    _previousOut = DateTime.UtcNow;

                    if (_outHistory.Count > 40)
                    {
                        _outHistory.RemoveAt(0);
                    }

                    _outHistory.Add($"+{delta.TotalSeconds:0.0000} - {message.Address} - {string.Join(", ", message.Arguments)}");
                }

                OutCommandHistory.Text = string.Join("\r\n", _outHistory);
            }
            else
            {
                foreach (var message in messages)
                {
                    var delta = DateTime.UtcNow - _previousIn;
                    _previousIn = DateTime.UtcNow;
                    var deltaOutIn = _previousIn - _previousOut;

                    if (_inHistory.Count > 40)
                    {
                        _inHistory.RemoveAt(0);
                    }

                    _inHistory.Add($"+{delta.TotalSeconds:0.0000} - {message.Address} - {string.Join(", ", message.Arguments)} -- <+{deltaOutIn.TotalSeconds:0.0000}>");
                }

                var data = string.Join("\r\n", _inHistory);

                await Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                {
                    InCommandHistory.Text = data;
                });
            }
        }

        private void InitializeCommandMappings()
        {
            InitializeDefaultCommandMapping();
            InitializeDefaultProceduralCommandMapping();

            InitializePartialCommandMapping();
            InitializePartialProceduralCommandMapping();
        }

        private void InitializeDefaultCommandMapping()
        {
            var i = (byte)0;
            var strobo = (byte)0;

            var test = 0;

            _commandMapping.Add(Mode.Default, new Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>
            {
                { VirtualKey.Back, () => _commandFactory.CreateTestMessage(++test) },
                {
                    VirtualKey.Up, () =>
                    {
                        i++;

                        ColorIndex.Text = i.ToString();
                        ColorIndex.Foreground = new SolidColorBrush(ColorFromHSV(i, 1.0, 1.0));

                        return _commandFactory.CreateSingleSolid((ColorPreset)i, 255, 254);
                    }
                },
                {
                    VirtualKey.Down, () =>
                    {
                        i--;

                        ColorIndex.Text = i.ToString();
                        ColorIndex.Foreground = new SolidColorBrush(ColorFromHSV(i, 1.0, 1.0));

                        return _commandFactory.CreateSingleSolid((ColorPreset)i, 255, 254);
                    }
                },
                {
                    VirtualKey.Right,
                    () => {
                        strobo++;

                        return _commandFactory.CreateStrobo((ColorPreset)Random(), strobo);
                    }
                },
                {
                    VirtualKey.Left,
                    () => {
                        strobo--;

                        return _commandFactory.CreateStrobo((ColorPreset)Random(), strobo);
                    }
                },

                { VirtualKey.Q, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 20) },
                { VirtualKey.W, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 40) },
                { VirtualKey.E, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 60) },
                { VirtualKey.R, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 80) },

                { VirtualKey.T, () => _commandFactory.CreateRainbowSpark(PulseLength.Long) },
                { VirtualKey.Y, () => _commandFactory.CreateRainbowSpark(PulseLength.Medium) },

                { VirtualKey.Number1, () => _commandFactory.CreateSingleSolid(ColorPreset.Purple, 255, 254) },
                { VirtualKey.Number2, () => _commandFactory.CreateSingleSolid(ColorPreset.Pink, 255, 254) },
                { VirtualKey.Number3, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 254) },
                { VirtualKey.Number4, () => _commandFactory.CreateSingleSolid(ColorPreset.Orange, 255, 254) },
                { VirtualKey.Number5, () => _commandFactory.CreateSingleSolid(ColorPreset.Yellow, 255, 254) },
                { VirtualKey.Number6, () => _commandFactory.CreateSingleSolid(ColorPreset.Lime, 255, 254) },
                { VirtualKey.Number7, () => _commandFactory.CreateSingleSolid(ColorPreset.Green, 255, 254) },
                { VirtualKey.Number8, () => _commandFactory.CreateSingleSolid(ColorPreset.SeaGreen, 255, 254) },
                { VirtualKey.Number9, () => _commandFactory.CreateSingleSolid(ColorPreset.Turquoise, 255, 254) },
                { VirtualKey.Number0, () => _commandFactory.CreateSingleSolid(ColorPreset.Blue, 255, 254) },

                { VirtualKey.NumberPad1, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 25) },
                { VirtualKey.NumberPad2, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 50) },
                { VirtualKey.NumberPad3, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 75) },
                { VirtualKey.NumberPad4, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 100) },
                { VirtualKey.NumberPad5, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 125) },
                { VirtualKey.NumberPad6, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 150) },
                { VirtualKey.NumberPad7, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 175) },
                { VirtualKey.NumberPad8, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 200) },
                { VirtualKey.NumberPad9, () => _commandFactory.CreateDualSolid(ColorPreset.Red, ColorPreset.Blue, 225) },

                { VirtualKey.F1, () => _commandFactory.CreateSingleSolid(ColorPreset.Purple, 245, 75) },
                { VirtualKey.F2, () => _commandFactory.CreateSingleSolid(ColorPreset.Pink, 255, 75) },
                { VirtualKey.F3, () => _commandFactory.CreateSingleSolid(ColorPreset.Red, 255, 75) },
                { VirtualKey.F4, () => _commandFactory.CreateSingleSolid(ColorPreset.Orange, 255, 75) },
                { VirtualKey.F5, () => _commandFactory.CreateSingleSolid(ColorPreset.Yellow, 255, 75) },
                { VirtualKey.F6, () => _commandFactory.CreateSingleSolid(ColorPreset.Lime, 255, 75) },
                { VirtualKey.F7, () => _commandFactory.CreateSingleSolid(ColorPreset.Green, 255, 75) },
                { VirtualKey.F8, () => _commandFactory.CreateSingleSolid(ColorPreset.SeaGreen, 255, 75) },
                { VirtualKey.F9, () => _commandFactory.CreateSingleSolid(ColorPreset.Turquoise,255, 75) },
                { VirtualKey.F10, () => _commandFactory.CreateSingleSolid(ColorPreset.Blue, 255, 75) },

                { VirtualKey.F11, () => _commandFactory.CreateDualSpark(ColorPreset.Red, ColorPreset.Blue, 127, PulseLength.Long) },
                { VirtualKey.F12, () => _commandFactory.CreateDualSpark(ColorPreset.Green, ColorPreset.Pink, 127, PulseLength.Long) },

                { VirtualKey.G, () => _commandFactory.CreateSingleSolid(0, 0, 255) },

                { VirtualKey.U, () => _commandFactory.CreateSingleSpark(ColorPreset.Red, 255, 254, PulseLength.Medium) },
                { VirtualKey.I, () => _commandFactory.CreateSingleSpark(ColorPreset.Blue, 255, 254, PulseLength.Medium) },
                { VirtualKey.O, () => _commandFactory.CreateSingleSpark(ColorPreset.Purple, 255, 254, PulseLength.Medium) },
                { VirtualKey.P, () => _commandFactory.CreateSingleSpark(ColorPreset.Green, 255, 254, PulseLength.Medium) },

                { VirtualKey.H, () => _commandFactory.CreateSinglePulse(ColorPreset.Red, 255, 254, PulseLength.Medium) },
                { VirtualKey.J, () => _commandFactory.CreateSinglePulse(ColorPreset.Blue, 255, 254, PulseLength.Medium) },
                { VirtualKey.K, () => _commandFactory.CreateSinglePulse(ColorPreset.Purple, 255, 254, PulseLength.Medium) },
                { VirtualKey.L, () => _commandFactory.CreateSinglePulse(ColorPreset.Green, 255, 254, PulseLength.Medium) },

                { VirtualKey.B, () => _commandFactory.CreateSinglePulse(ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.N, () => _commandFactory.CreateSinglePulse(ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.M, () => _commandFactory.CreateSinglePulse(ColorPreset.Purple, 255, 254, PulseLength.Long) },
                { (VirtualKey)188, () => _commandFactory.CreateSinglePulse(ColorPreset.Green, 255, 254, PulseLength.Long) }, // comma

                { VirtualKey.Space, () => _commandFactory.CreateStrobo((ColorPreset)Random(), strobo) },
                { VirtualKey.Escape, () => _commandFactory.CreateStrobo(0, 0) },

                { VirtualKey.Z, () => _commandFactory.CreateTwinkle((ColorPreset)Random(), Random()) },
                { VirtualKey.X, () => _commandFactory.CreateRainbowSolid() },
                { VirtualKey.CapitalLock, () => _specialCommandFactory.CreateRainbowUsingAddresses() },

                { VirtualKey.C, () => _commandFactory.CreateChase((ColorPreset)Random(), 1, 1) },
                { VirtualKey.V, () => _commandFactory.CreateChase((ColorPreset)Random(), Math.Max(1, Random() / 16), 0) },

                { (VirtualKey)187, () => _commandFactory.CreateChase((ColorPreset)Random(), 1, 3) },
                { (VirtualKey)189, () => _commandFactory.CreateChase((ColorPreset)Random(), Math.Max(1, Random() / 16), 2) },


                { (VirtualKey)219, () => _commandFactory.CreateBash((ColorPreset)Random(), 16) },
                { (VirtualKey)221, () => _commandFactory.CreateBash((ColorPreset)Random(), 127) },
                { (VirtualKey)220, () => _commandFactory.CreateBash((ColorPreset)Random(), 255) }
            });
        }

        private void InitializePartialCommandMapping()
        {
            _commandMapping.Add(Mode.Partial, new Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>
            {
                { VirtualKey.Number1, () => _commandFactory.CreateSinglePulse(0, 12, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number2, () => _commandFactory.CreateSinglePulse(12, 24, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number3, () => _commandFactory.CreateSinglePulse(24, 36, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number4, () => _commandFactory.CreateSinglePulse(36, 48, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number5, () => _commandFactory.CreateSinglePulse(48, 60, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number6, () => _commandFactory.CreateSinglePulse(60, 72, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number7, () => _commandFactory.CreateSinglePulse(72, 84, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number8, () => _commandFactory.CreateSinglePulse(84, 96, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number9, () => _commandFactory.CreateSinglePulse(96, 108, (ColorPreset)Random(), 255, 254, PulseLength.Long) },
                { VirtualKey.Number0, () => _commandFactory.CreateSinglePulse(108, 127, (ColorPreset)Random(), 255, 254, PulseLength.Long) },

                { VirtualKey.Q, () => _commandFactory.CreateSinglePulse(0, 12, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.W, () => _commandFactory.CreateSinglePulse(12, 24, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.E, () => _commandFactory.CreateSinglePulse(24, 36, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.R, () => _commandFactory.CreateSinglePulse(36, 48, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.T, () => _commandFactory.CreateSinglePulse(48, 60, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.Y, () => _commandFactory.CreateSinglePulse(60, 72, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.U, () => _commandFactory.CreateSinglePulse(72, 84, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.I, () => _commandFactory.CreateSinglePulse(84, 96, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.O, () => _commandFactory.CreateSinglePulse(96, 108, ColorPreset.Red, 255, 254, PulseLength.Long) },
                { VirtualKey.P, () => _commandFactory.CreateSinglePulse(108, 127, ColorPreset.Red, 255, 254, PulseLength.Long) },

                { VirtualKey.A, () => _commandFactory.CreateSinglePulse(0, 12, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.S, () => _commandFactory.CreateSinglePulse(12, 24, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.D, () => _commandFactory.CreateSinglePulse(24, 36, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.F, () => _commandFactory.CreateSinglePulse(36, 48, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.G, () => _commandFactory.CreateSinglePulse(48, 60, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.H, () => _commandFactory.CreateSinglePulse(60, 72, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.J, () => _commandFactory.CreateSinglePulse(72, 84, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.K, () => _commandFactory.CreateSinglePulse(84, 96, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { VirtualKey.L, () => _commandFactory.CreateSinglePulse(96, 108, ColorPreset.Blue, 255, 254, PulseLength.Long) },
                { (VirtualKey)186, () => _commandFactory.CreateSinglePulse(108, 127, ColorPreset.Blue, 255, 254, PulseLength.Long) },

                { VirtualKey.Z, () => _commandFactory.CreateSinglePulse(0, 12, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.X, () => _commandFactory.CreateSinglePulse(12, 24, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.C, () => _commandFactory.CreateSinglePulse(24, 36, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.V, () => _commandFactory.CreateSinglePulse(36, 48, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.B, () => _commandFactory.CreateSinglePulse(48, 60, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.N, () => _commandFactory.CreateSinglePulse(60, 72, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { VirtualKey.M, () => _commandFactory.CreateSinglePulse(72, 84, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { (VirtualKey)188, () => _commandFactory.CreateSinglePulse(84, 96, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { (VirtualKey)190, () => _commandFactory.CreateSinglePulse(96, 108, ColorPreset.Pink, 255, 254, PulseLength.Long) },
                { (VirtualKey)191, () => _commandFactory.CreateSinglePulse(108, 127, ColorPreset.Pink, 255, 254, PulseLength.Long) },
            });
        }

        private void InitializeDefaultProceduralCommandMapping()
        {
            _proceduralCommandMapping.Add(Mode.Default, new Dictionary<VirtualKey, Func<int, (int, IEnumerable<OscMessage>)>>
            {
                { VirtualKey.A, (i) => (10, _commandFactory.CreateVuMeter(Wave(i))) },
                { VirtualKey.S, (i) => (20, _commandFactory.CreateTwinkle(ColorPreset.Red, Wave(i))) },
                { VirtualKey.D, (i) => (5, _commandFactory.CreateTwinkle((ColorPreset)Random(), Random())) },
                { VirtualKey.F, (i) => (5, _commandFactory.CreateSingleSolid((ColorPreset)Clamp(i / 100.0), 255, 254)) }
            });
        }

        private void InitializePartialProceduralCommandMapping()
        {
            _proceduralCommandMapping.Add(Mode.Partial, new Dictionary<VirtualKey, Func<int, (int, IEnumerable<OscMessage>)>>
            {

            });
        }

        private static int Wave(int i)
        {
            return (int)(Math.Sin((i / 100.0) * Math.PI) * 255);
        }
        private static int Random()
        {
            return Clamp(new Random().NextDouble());
        }
        private static int Clamp(double i)
        {
            return (int)(i * 255);
        }

        public static Color ColorFromHSV(double hue, double saturation, double value)
        {
            var baseValue = 60 * (255.0 / 360.0);

            var hi = Convert.ToInt32(Math.Floor(hue / baseValue)) % 6;
            var f = hue / baseValue - Math.Floor(hue / baseValue);

            value = value * 255;
            var v = Convert.ToByte(value);
            var p = Convert.ToByte(value * (1 - saturation));
            var q = Convert.ToByte(value * (1 - f * saturation));
            var t = Convert.ToByte(value * (1 - (1 - f) * saturation));

            if (hi == 0)
            {
                return Color.FromArgb(255, v, t, p);
            }
            else if (hi == 1)
            {
                return Color.FromArgb(255, q, v, p);
            }
            else if (hi == 2)
            {
                return Color.FromArgb(255, p, v, t);
            }
            else if (hi == 3)
            {
                return Color.FromArgb(255, p, q, v);
            }
            else if (hi == 4)
            {
                return Color.FromArgb(255, t, p, v);
            }
            else
            {
                return Color.FromArgb(255, v, p, q);
            }
        }

        #endregion

        #region DMX

        private async void DmxLoadButton_Click(object sender, RoutedEventArgs e)
        {
            var tag = (sender as Button).Tag as string;

            try
            {
                var fileName = GetFileName(tag);

                var folder = await GetStorageFolderAsync();

                if (folder == null)
                {
                    return;
                }

                var fileContents = await GetReadFileContentsAsync(fileName, folder);

                _dmxConfig.ReadConfig(fileContents);
            }
            catch
            {
                _dmxConfig.RemoveAllSlaves();
            }
        }

        private async void DmxDownloadSaveButton_Click(object sender, RoutedEventArgs e)
        {
            var tag = (sender as Button).Tag as string;
            try
            {
                var fileName = GetFileName(tag);

                var folder = await GetStorageFolderAsync();

                if (folder != null)
                {
                    var fileContents = _dmxConfig.WriteConfig();

                    await WriteStreamToFileAsync(fileName, folder, fileContents);
                }
            }
            catch
            {

            }

            var command = _dmxConfig.GenerateOscConfig(tag);

            foreach (var oscSender in _senderManager.Senders)
            {
                await oscSender.SendAsync(command);
            }

            await LogCommandAsync(CommandDirection.Out, command);
        }

        private static string GetFileName(string tag)
        {
            return $"edt-{tag.Replace("/", "")}.edt";
        }

        private static async Task<StorageFolder> GetStorageFolderAsync()
        {
            StorageFolder folder = null;
            try
            {
                folder = await ApplicationData.Current.LocalFolder.GetFolderAsync("Edt-2000").AsTask();
            }
            catch
            {
                folder = await ApplicationData.Current.LocalFolder.CreateFolderAsync("Edt-2000").AsTask();
            }

            return folder;
        }

        private static async Task<string> GetReadFileContentsAsync(string fileName, StorageFolder folder)
        {
            var fileHandle = await folder.GetFileAsync(fileName).AsTask();

            return await FileIO.ReadTextAsync(fileHandle).AsTask();
        }

        private static async Task WriteStreamToFileAsync(string fileName, StorageFolder folder, string fileContentString)
        {
            StorageFile fileHandle;
            try
            {
                fileHandle = await folder.GetFileAsync(fileName).AsTask();
            }
            catch
            {
                fileHandle = await folder.CreateFileAsync(fileName).AsTask();
            }

            await FileIO.WriteTextAsync(fileHandle, fileContentString).AsTask();
        }
        
        private void AddSlaveButton_Click(object sender, RoutedEventArgs e)
        {
            var slaveType = SlaveType.SelectedValue as int?;
            var slaveAddress = SlaveAddress.SelectedValue as int?;
            var minimumBrightness = MinimumBrightness.Value;
            var maximumBrightness = MaximumBrightness.Value;

            if (slaveType.HasValue && slaveAddress.HasValue)
            {
                _dmxConfig.AddSlave(slaveType.Value, slaveAddress.Value, maximumBrightness, minimumBrightness);
            }
        }
        
        private void DeleteSlaveButton_Click(object sender, RoutedEventArgs e)
        {
            _dmxConfig.RemoveSlave(((sender as Button).Tag as int?) ?? -1);
        }

        #endregion
    }
}