using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.DMX
{
    public class DmxSlave
    {
        public DmxType Type { get; set; }
        public int Address { get; set; }

        public string FullName => $"{Address}-{(Address + Type.Width - 1)}: {Type.Name}";
    }
}
