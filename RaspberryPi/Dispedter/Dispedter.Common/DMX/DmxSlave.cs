using Newtonsoft.Json;

namespace Dispedter.Common.DMX
{
    public class DmxSlave
    {
        public DmxType Type { get; set; }
        public int Address { get; set; }

        [JsonIgnore]
        public string FullName => $"{Address}-{(Address + Type.Width - 1)}: {Type.Name}";
    }
}
