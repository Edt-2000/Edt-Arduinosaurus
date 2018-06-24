using Dispedter.Common.Factories;
using Dispedter.Common.OSC;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Linq;

namespace Dispedter.Common.DMX
{
    public class DmxConfig
    {
        public ICollection<DmxSlave> _dmxSlaves;

        public DmxConfig(ICollection<DmxSlave> collection)
        {
            _dmxSlaves = collection;
        }

        public ICollection<DmxType> Types { get; private set; } = new[]
        {
            new DmxType { Name = "LED Spot - LED Spot light 4 channel", TypeNr = 1, Width = 6 },
            new DmxType { Name = "LED Par - Generic 3 channel", TypeNr = 2, Width = 3 },
            new DmxType { Name = "LED Par - Show Tec Compact Par 8 channel", TypeNr = 3, Width = 8 },
            new DmxType { Name = "Fixed - Single channel", TypeNr = 4, Width = 1 }
        };


        public void ReadConfig(string configString)
        {
            var config = JsonConvert.DeserializeObject<List<DmxSlave>>(configString);

            RemoveAllSlaves();
            
            config?.OrderBy(s => s.Address).ToList().ForEach(slave => _dmxSlaves.Add(slave));
        }

        public string WriteConfig()
        {
            var configString = JsonConvert.SerializeObject(_dmxSlaves);
            return configString;
        }

        public IEnumerable<OscMessage> GenerateOscConfig(string target)
        {
            var commandFactory = new CommandFactory(new[] { target });

            var commands = new List<OscMessage>();

            commands.AddRange(commandFactory.ClearDMX());

            foreach (var slave in _dmxSlaves)
            {
                commands.AddRange(commandFactory.ProgramDmxSlave(slave.Type.TypeNr, slave.Address, slave.MaximumBrightness, slave.MinimumBrightness));
            }

            commands.AddRange(commandFactory.RestartDMX());

            return commands;
        }

        public void AddSlave(int typeNr, int address, double maximumBrightness, double minimumBrightness)
        {
            var newType = Types.First(t => t.TypeNr == typeNr);
            
            // TODO: check for all collisions
            if (_dmxSlaves.Any(s => address >= s.Address && address <= (s.Address + s.Type.Width - 1)))
            {
                return;
            }

            _dmxSlaves.Add(new DmxSlave
            {
                Type = newType,
                Address = address,
                MaximumBrightness = maximumBrightness,
                MinimumBrightness = minimumBrightness
            });
        }

        public void RemoveSlave(int address)
        {
            if (_dmxSlaves.Any(s => s.Address == address))
            {
                _dmxSlaves.Remove(_dmxSlaves.First(s => s.Address == address));
            }
        }

        public void RemoveAllSlaves()
        {
            _dmxSlaves.Clear();
        }
    }
}
