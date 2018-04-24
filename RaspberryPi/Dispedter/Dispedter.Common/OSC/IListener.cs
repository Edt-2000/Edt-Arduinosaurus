using System;
using Windows.Foundation;

namespace Dispedter.Common.OSC
{
    public interface IListener
    {
        void AddPacketHandler(TypedEventHandler<IListener, OscEventArgs> handler);

        bool IsBroken();
        string Id { get; }
    }
}
