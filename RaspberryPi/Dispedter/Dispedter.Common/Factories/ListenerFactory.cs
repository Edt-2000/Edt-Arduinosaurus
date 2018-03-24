using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.Factories
{
    public class ListenerFactory
    {
        private readonly bool _udpListener;

        public ListenerFactory(bool udpListener = false)
        {
            _udpListener = udpListener;
        }

        public IEnumerable<IListener> GetAllListeners()
        {
            var listeners = new List<IListener>();

            if (_udpListener)
            {
                listeners.Add(new UdpListener(12345));
            }

            return listeners;
        }
    }
}
