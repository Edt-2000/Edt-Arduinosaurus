using Dispedter.Common.Factories;
using Dispedter.Common.OSC;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

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
            new DmxType { Name = "LED Spot", TypeNr = 1, Width = 6 }
        };


        public async Task ReadConfigAsync(Stream configStream)
        {
            using (var reader = new StreamReader(configStream))
            {
                var configString = await reader.ReadToEndAsync();
                var config = JsonConvert.DeserializeObject<List<DmxSlave>>(configString);

                if (config != null)
                {
                    _dmxSlaves = config;
                }
            }
        }

        public async Task<Stream> WriteConfigAsync()
        {
            var stream = new MemoryStream();
            var writer = new StreamWriter(stream);
            var configString = JsonConvert.SerializeObject(_dmxSlaves);

            await writer.WriteAsync(configString);

            return stream;
        }

        public IEnumerable<OscMessage> GenerateOscConfig(string target)
        {
            var commandFactory = new CommandFactory(new[] { target });

            var commands = new List<OscMessage>();

            commands.AddRange(commandFactory.ClearDMX());

            foreach (var slave in _dmxSlaves)
            {
                commands.AddRange(commandFactory.ProgramDmxSlave(slave.Type.TypeNr, slave.Address));
            }

            commands.AddRange(commandFactory.RestartDMX());

            return commands;
        }

        public void AddSlave(int typeNr, int address)
        {
            if (_dmxSlaves.Any(s => s.Address >= address && (s.Address + s.Type.Width - 1) <= address))
            {
                return;
            }

            _dmxSlaves.Add(new DmxSlave
            {
                Type = Types.First(t => t.TypeNr == typeNr),
                Address = address
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
