using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.Managers
{
    public class ListenerManager
    {
        private const int _udpPort = 12345;

        private List<IListener> _listeners = new List<IListener>();

        public ListenerManager()
        {

        }

        public List<IListener> Listeners { get => _listeners; }

        public async Task ManageDevicesAsync()
        {
            do
            {
                try
                {
                    _listeners.RemoveAll(l => l.IsBroken());

                    if (!_listeners.Exists(s => s.Id == _udpPort.ToString()))
                    {
                        var udpListener = new UdpListener(_udpPort);

                        _listeners.Add(udpListener);
                    }

                    await Task.Delay(5000);
                }
                catch (Exception)
                {
                }
            }
            while (true);
        }
    }
}
