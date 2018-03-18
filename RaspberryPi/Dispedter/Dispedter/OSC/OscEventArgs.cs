using System;

namespace Dispedter.OSC
{
    internal sealed class OscEventArgs
    {
        private readonly OscPacket _oscPacket;

        public OscEventArgs(OscPacket oscPacket)
        {
            _oscPacket = oscPacket;
        }

        public OscPacket GetOscPacket()
        {
            return _oscPacket;
        }
    }
}
