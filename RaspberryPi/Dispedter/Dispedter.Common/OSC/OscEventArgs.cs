using System;

namespace Dispedter.Common.OSC
{
    public class OscEventArgs : EventArgs
    {
        public OscEventArgs(OscPacket oscPacket)
        {
            OscPacket = oscPacket;
        }

        public OscPacket OscPacket { get; }
    }
}
