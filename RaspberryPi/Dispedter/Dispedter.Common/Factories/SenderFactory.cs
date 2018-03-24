using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;

namespace Dispedter.Common.Factories
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

        public async Task<IEnumerable<ISender>> GetAllSendersAsync()
        {
            var devices = new List<ISender>();

            try
            {
                if (_detectUsb)
                {
                    var _serialSelector = SerialDevice.GetDeviceSelector();
                    var infos = await DeviceInformation.FindAllAsync(_serialSelector);

                    foreach (var info in infos.Where(i => i.Name.Contains("SparkFun")))
                    {
                        var serialDevice = await SerialDevice.FromIdAsync(info.Id);
                        devices.Add(new UsbSender(serialDevice));
                    }
                }
            }
            catch (Exception ex)
            {

            }

            return devices;
        }
    }
}
