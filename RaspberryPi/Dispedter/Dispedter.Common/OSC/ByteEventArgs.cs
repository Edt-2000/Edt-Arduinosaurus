using System;

namespace Dispedter.Common.OSC
{
    //   public delegate void HandleOscPacket(OscPacket packet);
    //public delegate void HandleBytePacket(byte[] packet);

    public class ByteEventArgs : EventArgs
    {
        public ByteEventArgs(byte[] bytes)
        {
            Bytes = bytes;
        }

        public byte[] Bytes { get; }
    }
}
