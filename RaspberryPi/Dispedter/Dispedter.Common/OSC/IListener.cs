using System;
using Windows.Foundation;

namespace Dispedter.Common.OSC
{
    public interface IListener
    {
        void AddPacketHandler(TypedEventHandler<UdpListener, OscEventArgs> handler);
    }
}
