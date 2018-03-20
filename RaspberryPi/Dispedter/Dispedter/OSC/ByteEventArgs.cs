using System;

namespace Dispedter.OSC
{
    internal sealed class ByteEventArgs
    {
        private readonly byte[] _bytes;

        public ByteEventArgs(byte[] bytes)
        {
            _bytes = bytes;
        }
        
        public byte[] GetBytes()
        {
            return _bytes;
        }
    }
}
