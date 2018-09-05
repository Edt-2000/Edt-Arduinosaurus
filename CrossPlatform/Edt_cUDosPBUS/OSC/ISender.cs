using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Edt_cUDosPBUS.OSC
{
    public interface ISender : IDisposable
    {
        Task SendAsync(byte[] message);
        Task SendAsync(OscPacket packet);
        Task SendAsync(IEnumerable<OscPacket> packets);

        bool IsBroken();
        
        string Id { get; }
    }
}
