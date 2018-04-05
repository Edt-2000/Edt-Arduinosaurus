using Dispedter.Common.Factories;
using Dispedter.Common.OSC;
using Dispedter.Common.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;
using Windows.Devices.Gpio;
using Windows.Networking.Sockets;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace Dispedter
{
    public sealed class StartupTask : IBackgroundTask
    {
        private readonly ListenerFactory _listenerFactory = new ListenerFactory(udpListener: true);
        //private readonly SenderFactory _senderFactory = new SenderFactory(detectUsb: true);

        private IEnumerable<IListener> _listeners;
        //private IEnumerable<SendTask> _senders;

        private BackgroundTaskDeferral _defer;

        private const int _ledPin = 5;
        
        private GpioPin _pin;
        
        private bool _pinState;

        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            SetupGpio();
            //await SetupSendersAsync();
            SetupListeners();

            _defer = taskInstance.GetDeferral();

            //do
            //{
            //    await Task.WhenAny(_senders.Select(sender => sender.KeepAliveAsync()));

            //    // TODO: a device has failed. see which, and try to get it back up

            //}
            //while (true);

            //_pinState = false;

            

            do
            {
                _pin.Write(_pinState ? GpioPinValue.High : GpioPinValue.Low);

            //    //_pinState = !_pinState;

                await Task.Delay(50);
            }
            while (true);

            //defer.Complete();
        }

        //private async Task SetupSendersAsync()
        //{
        //    var senders = await _senderFactory.GetAllSendersAsync();

        //    _senders = senders.Select(sender => new SendTask(sender));
        //}

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

        private void SetupListeners()
        {
            _listeners = _listenerFactory.GetAllListeners();

            foreach(var listener in _listeners)
            {
                listener.AddPacketHandler(OscPacketReceived);
            }
        }

        private void OscPacketReceived(UdpListener sender, OscEventArgs args)
        {
            _pinState = !_pinState;

            //foreach(var s in _senders)
            //{
            //    s.AddMessage(args.GetOscPacket());
            //}
        }
    }
}
