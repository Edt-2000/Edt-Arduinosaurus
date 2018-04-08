using Dispedter.Common.Factories;
using Dispedter.Common.Managers;
using Dispedter.Common.OSC;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.ApplicationModel.Background;

// The Background Application template is documented at http://go.microsoft.com/fwlink/?LinkID=533884&clcid=0x409

namespace Dispedter
{
    public sealed class StartupTask : IBackgroundTask
    {
        private readonly ListenerManager _listenerManager = new ListenerManager();
        private readonly SenderManager _senderManager = new SenderManager(detectUsb: true, udpDestinations: new[] { "10.0.0.1" });

        private BackgroundTaskDeferral _defer;

        private Task _manageListenersTask;
        private Task _manageSendersTask;

        public async void Run(IBackgroundTaskInstance taskInstance)
        {
            _manageSendersTask = _senderManager.ManageDevicesAsync();
            _manageListenersTask = _listenerManager.ManageDevicesAsync();

            SetupListeners();

            _defer = taskInstance.GetDeferral();
            
            // we never complete
            //defer.Complete();
        }
        private void SetupListeners()
        {
            foreach(var listener in _listenerManager.Listeners)
            {
                listener.AddPacketHandler(OscPacketReceived);
            }
        }

        private void OscPacketReceived(UdpListener sender, OscEventArgs args)
        {
            var packet = args.GetOscPacket();

            foreach (var s in _senderManager.Senders)
            {
                s.SendAsync(packet);
            }
        }
    }
}
