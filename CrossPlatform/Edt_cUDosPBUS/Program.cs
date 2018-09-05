using Edt_cUDosPBUS.Managers;
using Edt_cUDosPBUS.OSC;
using System.Linq;
using System.Threading.Tasks;

namespace Edt_cUDosPBUS
{
    class Program
    {
        private static ListenerManager _listenerManager = new ListenerManager(detectUsb: false);
        private static SenderManager _senderManager = new SenderManager(detectUsb: true);

        private static Task _listenerTask;
        private static Task _senderTask;

        static async Task Main(string[] args)
        {
            _listenerManager.AttachEventHandler(async (IListener listener, OscEventArgs oscArgs) =>
            {
                await Task.WhenAll(_senderManager.Senders.Select(sender => sender.SendAsync(oscArgs.GetOscPacket())));
            });

            _listenerTask = _listenerManager.ManageDevicesAsync();
            _senderTask = _senderManager.ManageDevicesAsync();

            do
            {
                //_senderManager.Senders.ForEach(sender => sender.SendAsync(new OscMessage("/F?", 3, 0, 127, 0, 127, 63)));

                await Task.Delay(1000);//.ConfigureAwait(false);
            }
            while (true);
        }
    }
}
