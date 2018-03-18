using Dispedter.OSC;
using System;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Devices.Gpio;
using Windows.Networking.Sockets;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace Dispedter
{
    public sealed class StartupTask : IBackgroundTask
    {
        private const int _ledPin = 5;
        private const int _udpPort = 12345;

        private GpioPin _pin;
        private UdpListener _listener;

        private bool _pinState;

        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            SetupGpio();
            SetupOscListener();

            var defer = taskInstance.GetDeferral();

            _pinState = false;

            

            do
            {
                _pin.Write(_pinState ? GpioPinValue.High : GpioPinValue.Low);

                //_pinState = !_pinState;

                await Task.Delay(50);
            }
            while (true);

            defer.Complete();
        }

        private void SetupGpio()
        {
            var gpio = GpioController.GetDefault();

            // Show an error if there is no GPIO controller
            if (gpio == null)
            {
                _pin = null;
                return;
            }

            _pin = gpio.OpenPin(_ledPin);
            _pin.Write(GpioPinValue.High);
            _pin.SetDriveMode(GpioPinDriveMode.Output);
        }
        
        private void SetupOscListener()
        {
            _listener = new UdpListener(_udpPort);
            _listener.OscPacketReceived += _listener_OscPacketReceived;
        }

        private void _listener_OscPacketReceived(UdpListener sender, OscEventArgs args)
        {
            _pinState = !_pinState;
        }
    }
}
