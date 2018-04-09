using Dispedter.Common.OSC;
using Dispedter.Common.OSCArduino;
using Dispedter.Common.Factories;
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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Dispedter.Tester
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private readonly CommandFactory _commandFactory = new CommandFactory(new[] { "/L" });
        private readonly ListenerManager _listenerManager = new ListenerManager();
        private readonly SenderManager _senderManager = new SenderManager(detectUsb: true, udpDestinations: new[] { "10.0.0.10" });

        private Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>> _commandMapping;
        private Dictionary<VirtualKey, Func<int, (int delay, IEnumerable<OscMessage> command)>> _proceduralCommandMapping;

        private Task _scanForDevicesTask;
        private Task _manageDevicesTask;

        enum CommandDirection
        {
            In,
            Out
        }

        public MainPage()
        {
            InitializeComponent();
            InitializeListeners();

            _scanForDevicesTask = _senderManager.ManageDevicesAsync();
            _manageDevicesTask = _listenerManager.ManageDevicesAsync();

            InitializeCommandMapping();
            InitializeProceduralCommandMapping();

            Window.Current.CoreWindow.KeyDown += async (s, e) =>
            {
                var key = e.VirtualKey;
                await SendCommandAsync(key);
            };
        }

        private void InitializeListeners()
        {
            _listenerManager.AttachEventHandler((UdpListener listener, OscEventArgs args) =>
            {
                LogCommand(CommandDirection.In, new[] { args.GetOscPacket() as OscMessage });
            });
        }

        private async Task SendCommandAsync(VirtualKey key)
        {
            if (!_senderManager.Senders?.Any() ?? false)
            {
                return;
            }

            if (_commandMapping.TryGetValue(key, out var commandGenerator))
            {
                var command = commandGenerator();

                foreach (var sender in _senderManager.Senders)
                {
                    await sender.SendAsync(command);
                }

                LogCommand(CommandDirection.Out, command);
            }
            else if (_proceduralCommandMapping.TryGetValue(key, out var proceduralCommandGenerator))
            {
                var i = 0;
                do
                {
                    var (delay, command) = proceduralCommandGenerator(i);

                    foreach (var sender in _senderManager.Senders)
                    {
                        await sender.SendAsync(command);
                    }

                    LogCommand(CommandDirection.Out, command);

                    await Task.Delay(delay);

                } while (++i < 100);
            }
            else
            {
                ;
            }
        }

        private List<string> _outHistory = new List<string>();
        private ObservableCollection<string> _inHistory = new ObservableCollection<string>();

        private void LogCommand(CommandDirection commandDirection, IEnumerable<OscMessage> messages)
        {
            if (commandDirection == CommandDirection.Out)
            {
                foreach (var message in messages)
                {
                    if (_outHistory.Count > 30)
                    {
                        _outHistory.RemoveAt(0);
                    }

                    _outHistory.Add($"{message.Address} - {string.Join(", ", message.Arguments)}");
                }

                OutCommandHistory.Text = string.Join("\r\n", _outHistory);
            }
            else
            {
                foreach (var message in messages)
                {
                    if (_inHistory.Count > 30)
                    {
                        _inHistory.RemoveAt(0);
                    }

                    _inHistory.Add($"{message.Address} - {string.Join(", ", message.Arguments)}");
                }

                InCommandHistory.Text = string.Join("\r\n", _inHistory);
            }
        }

        private void InitializeCommandMapping()
        {
            var i = (byte)0;
            var strobo = (byte)0;

            var test = 0;

            _commandMapping = new Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>
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
                        strobo <<= 1;
                        strobo++;

                        return _commandFactory.CreateStrobo((ColorPreset)Random(), strobo);
                    }
                },
                {
                    VirtualKey.Left,
                    () => {
                        strobo >>= 1;

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
            };
        }
        private void InitializeProceduralCommandMapping()
        {
            _proceduralCommandMapping = new Dictionary<VirtualKey, Func<int, (int, IEnumerable<OscMessage>)>>
            {
                { VirtualKey.A, (i) => (10, _commandFactory.CreateVuMeter(Wave(i))) },
                { VirtualKey.S, (i) => (20, _commandFactory.CreateTwinkle(ColorPreset.Red, Wave(i))) },
                { VirtualKey.D, (i) => (5, _commandFactory.CreateTwinkle((ColorPreset)Random(), Random())) },
                { VirtualKey.F, (i) => (5, _commandFactory.CreateSingleSolid((ColorPreset)Clamp(i / 100.0), 255, 254)) }
            };
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
    }
}
