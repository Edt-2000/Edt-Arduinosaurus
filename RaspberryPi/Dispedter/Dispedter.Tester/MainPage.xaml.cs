using Dispedter.Common.OSC;
using Dispedter.Common.OSCArduino;
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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Dispedter.Tester
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private readonly CommandFactory _commandFactory = new CommandFactory(new[] { "/L" });
        private readonly ISender _usbSender = new UsbSender("COM3", 9600);

        private Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>> _commandMapping;
        private Dictionary<VirtualKey, Func<int, (int delay, IEnumerable<OscMessage> command)>> _proceduralCommandMapping;

        public MainPage()
        {
            InitializeComponent();

            InitializeCommandMapping();
            InitializeProceduralCommandMapping();

            Window.Current.CoreWindow.KeyDown += async (s, e) =>
            {
                var key = e.VirtualKey;

                if (_commandMapping.TryGetValue(key, out var command))
                {
                    _usbSender.Send(command());
                }
                else if (_proceduralCommandMapping.TryGetValue(key, out var procedure))
                {
                    var i = 0;
                    do
                    {
                        var data = procedure(i);

                        _usbSender.Send(data.command);

                        await Task.Delay(data.delay);

                    } while (++i < 100);
                }
                else
                {
                    ;
                }
            };
        }



        public void InitializeCommandMapping()
        {
            _commandMapping = new Dictionary<VirtualKey, Func<IEnumerable<OscMessage>>>
            {
                { VirtualKey.Q, () => _commandFactory.CreateSingleSolid(97, 255, 20) },
                { VirtualKey.W, () => _commandFactory.CreateSingleSolid(97, 255, 40) },
                { VirtualKey.E, () => _commandFactory.CreateSingleSolid(97, 255, 60) },
                { VirtualKey.R, () => _commandFactory.CreateSingleSolid(97, 255, 80) },

                { VirtualKey.T, () => _commandFactory.CreateRainbowSpark(PulseLength.Long) },
                { VirtualKey.Y, () => _commandFactory.CreateRainbowSpark(PulseLength.Medium) },

                { VirtualKey.Number1, () => _commandFactory.CreateSingleSolid(20, 255, 254) },
                { VirtualKey.Number2, () => _commandFactory.CreateSingleSolid(60, 255, 254) },
                { VirtualKey.Number3, () => _commandFactory.CreateSingleSolid(97, 255, 254) },
                { VirtualKey.Number4, () => _commandFactory.CreateSingleSolid(140, 255, 254) },
                { VirtualKey.Number5, () => _commandFactory.CreateSingleSolid(180, 255, 254) },
                { VirtualKey.Number6, () => _commandFactory.CreateSingleSolid(220, 255, 254) },
                { VirtualKey.Number7, () => _commandFactory.CreateSingleSolid(10, 255, 254) },
                { VirtualKey.Number8, () => _commandFactory.CreateSingleSolid(70, 255, 254) },
                { VirtualKey.Number9, () => _commandFactory.CreateSingleSolid(140, 255, 254) },
                { VirtualKey.Number0, () => _commandFactory.CreateSingleSolid(210, 255, 254) },

                { VirtualKey.F1, () => _commandFactory.CreateSingleSolid(20, 255, 75) },
                { VirtualKey.F2, () => _commandFactory.CreateSingleSolid(60, 255, 75) },
                { VirtualKey.F3, () => _commandFactory.CreateSingleSolid(97, 255, 75) },
                { VirtualKey.F4, () => _commandFactory.CreateSingleSolid(140, 255, 75) },
                { VirtualKey.F5, () => _commandFactory.CreateSingleSolid(180, 255, 75) },
                { VirtualKey.F6, () => _commandFactory.CreateSingleSolid(220, 255, 75) },
                { VirtualKey.F7, () => _commandFactory.CreateSingleSolid(10, 255, 75) },
                { VirtualKey.F8, () => _commandFactory.CreateSingleSolid(70, 255, 75) },
                { VirtualKey.F9, () => _commandFactory.CreateSingleSolid(140, 255, 75) },
                { VirtualKey.F10, () => _commandFactory.CreateSingleSolid(210, 255, 75) },

                { VirtualKey.F11, () => _commandFactory.CreateDualSpark(97, 180, 127, PulseLength.Long) },
                { VirtualKey.F12, () => _commandFactory.CreateDualSpark(20, 140, 127, PulseLength.Long) },

                { VirtualKey.G, () => _commandFactory.CreateSingleSolid(0, 0, 255) },

                { VirtualKey.U, () => _commandFactory.CreateSingleSpark(0, 255, 254, PulseLength.Medium) },
                { VirtualKey.I, () => _commandFactory.CreateSingleSpark(64, 255, 254, PulseLength.Medium) },
                { VirtualKey.O, () => _commandFactory.CreateSingleSpark(97, 255, 254, PulseLength.Medium) },
                { VirtualKey.P, () => _commandFactory.CreateSingleSpark(192, 255, 254, PulseLength.Medium) },

                { VirtualKey.H, () => _commandFactory.CreateSinglePulse(0, 255, 254, PulseLength.Medium) },
                { VirtualKey.J, () => _commandFactory.CreateSinglePulse(64, 255, 254, PulseLength.Medium) },
                { VirtualKey.K, () => _commandFactory.CreateSinglePulse(97, 255, 254, PulseLength.Medium) },
                { VirtualKey.L, () => _commandFactory.CreateSinglePulse(192, 255, 254, PulseLength.Medium) },

                { VirtualKey.B, () => _commandFactory.CreateSinglePulse(0, 255, 254, PulseLength.Long) },
                { VirtualKey.N, () => _commandFactory.CreateSinglePulse(64, 255, 254, PulseLength.Long) },
                { VirtualKey.M, () => _commandFactory.CreateSinglePulse(97, 255, 254, PulseLength.Long) },
                { (VirtualKey)188, () => _commandFactory.CreateSinglePulse(192, 255, 254, PulseLength.Long) }, // comma

                { VirtualKey.Space, () => _commandFactory.CreateStrobo(Random(), 127) },
                { VirtualKey.Escape, () => _commandFactory.CreateStrobo(0, 0) },

                { VirtualKey.Z, () => _commandFactory.CreateTwinkle(Random(), Random()) },
                { VirtualKey.X, () => _commandFactory.CreateRainbowSolid() },
            };
        }
        private void InitializeProceduralCommandMapping()
        {
            _proceduralCommandMapping = new Dictionary<VirtualKey, Func<int, (int, IEnumerable<OscMessage>)>>
            {
                { VirtualKey.A, (i) => (10, _commandFactory.CreateVuMeter(Wave(i))) },
                { VirtualKey.S, (i) => (20, _commandFactory.CreateTwinkle(97, Wave(i))) },
                { VirtualKey.D, (i) => (5, _commandFactory.CreateTwinkle(Random(), Random())) },
                { VirtualKey.F, (i) => (5, _commandFactory.CreateSingleSolid(Clamp(i / 100.0), 255, 254)) }
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
    }
}
