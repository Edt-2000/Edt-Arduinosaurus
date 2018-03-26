using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.OSC
{
    public interface ISender
    {
        Task SendAsync(byte[] message);
        Task SendAsync(OscPacket packet);
        Task SendAsync(IEnumerable<OscPacket> packets);

        bool IsBroken();
    }
}
