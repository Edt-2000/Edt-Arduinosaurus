using Dispedter.Common.OSC;
using System.Collections.Generic;
using System.Linq;

namespace Dispedter.Common.OSCArduino
{
    public class CommandFactory
    {
        private readonly string[] _addresses;

        public CommandFactory(string[] addresses)
        {
            _addresses = addresses;
        }

        public IEnumerable<OscMessage> CreateSingleSolid(int h, int s, int v)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.SingleSolid, 0, 127, h, s, v));
        }

        public IEnumerable<OscMessage> CreateSinglePulse(int h, int s, int v, PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.SinglePulse, 0, 127, h, s, v, (int)p));
        }

        public IEnumerable<OscMessage> CreateSingleSpark(int h, int s, int v, PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.SingleSpark, 0, 127, h, s, v, (int)p));
        }

        public IEnumerable<OscMessage> CreateDualSolid(int h1, int h2, int percentage)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.DualSolid, 0, 127, h1, h2, percentage));
        }

        public IEnumerable<OscMessage> CreateDualPulse(int h1, int h2, int percentage, PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.DualPulse, 0, 127, h1, h2, percentage, (int)p));
        }

        public IEnumerable<OscMessage> CreateDualSpark(int h1, int h2, int percentage, PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.DualSpark, 0, 127, h1, h2, percentage, (int)p));
        }

        public IEnumerable<OscMessage> CreateRainbowSolid()
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.RainbowSolid, 0, 127, 0, 127));
        }

        public IEnumerable<OscMessage> CreateRainbowPulse(PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.RainbowPulse, 0, 127, 0, 127, (int)p));
        }

        public IEnumerable<OscMessage> CreateRainbowSpark(PulseLength p)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.RainbowSpark, 0, 127, 0, 127, (int)p));
        }

        public IEnumerable<OscMessage> CreateVuMeter(int intensity)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.VUMeter, 0, 127, 63, 0, 128, intensity));
        }

        public IEnumerable<OscMessage> CreateTwinkle(int h, int intensity)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.Twinkle, 0, 127, h, intensity));
        }

        public IEnumerable<OscMessage> CreateStrobo(int h, int speed)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.Strobo, h, speed));
        }

        public IEnumerable<OscMessage> CretaeKitt(int position, int length, int h)
        {
            return _addresses.Select(a => new OscMessage(a, (int)Command.Kitt, position, length, h));
        }
    }
}
