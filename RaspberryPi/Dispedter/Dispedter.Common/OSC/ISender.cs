using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.OSC
{
    public interface ISender
    {
        void Send(byte[] message);
        void Send(OscPacket packet);
        void Send(IEnumerable<OscPacket> packets);
    }
}
