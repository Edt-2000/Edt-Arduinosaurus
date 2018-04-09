using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace Dispedter.Common.Managers
{
    public class ListenerManager
    {
        private const int _udpPort = 12345;

        private List<IListener> _listeners = new List<IListener>();
        private List<TypedEventHandler<UdpListener, OscEventArgs>> _eventHandlers = new List<TypedEventHandler<UdpListener, OscEventArgs>>();

        public ListenerManager()
        {

        }

        public List<IListener> Listeners { get => _listeners; }

        public void AttachEventHandler(TypedEventHandler<UdpListener, OscEventArgs> eventHandler)
        {
            if(_listeners.Count > 0)
            {
                throw new Exception("Attach event handlers prior to manage devices.");
            }

            _eventHandlers.Add(eventHandler);
        }

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

                        foreach (var handler in _eventHandlers)
                        {
                            udpListener.OscPacketReceived += handler;
                        }

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
