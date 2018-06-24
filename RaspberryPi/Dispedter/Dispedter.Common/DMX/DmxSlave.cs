using Newtonsoft.Json;

namespace Dispedter.Common.DMX
{
    public class DmxSlave
    {
        public DmxType Type { get; set; }
        public int Address { get; set; }
        public double MaximumBrightness { get; set; }
        public double MinimumBrightness { get; set; }

        [JsonIgnore]
        public string FullName => $"{Address}-{(Address + Type.Width - 1)}: {Type.Name} ({(int)(MinimumBrightness * 100)} % - {(int)(MaximumBrightness * 100)} %)";
    }
}
