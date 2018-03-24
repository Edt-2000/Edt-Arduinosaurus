using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;

namespace Dispedter.Common.System
{
    public class SenderFactory
    {
        private readonly bool _detectUsb;
        private readonly bool _detectUdp;

        public SenderFactory(bool detectUsb = false, bool detectUdp = false)
        {
            _detectUsb = detectUsb;
            _detectUdp = detectUdp;
        }

        public async Task<IEnumerable<ISender>> DetectAllSendersAsync()
        {
            var devices = new List<ISender>();

            if(_detectUsb)
            {
                var _serialSelector = SerialDevice.GetDeviceSelector();
                var infos = await DeviceInformation.FindAllAsync(_serialSelector);

                var ports = new List<(string portName, uint baudRate)>();

                foreach (var info in infos)
                {
                    using (var serialDevice = await SerialDevice.FromIdAsync(info.Id))
                    {
                        if (serialDevice != null)
                        {
                            ports.Add((serialDevice.PortName, 9600));
                        }
                    }
                }

                devices.AddRange(ports.Select((data) => new UsbSender(data.portName, data.baudRate)));
            }

            return devices;
        }
    }
}
