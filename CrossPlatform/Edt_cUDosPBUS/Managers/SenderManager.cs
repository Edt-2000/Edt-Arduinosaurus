using Edt_cUDosPBUS.OSC;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Threading.Tasks;

namespace Edt_cUDosPBUS.Managers
{
    public class SenderManager
    {
        private readonly bool _detectUsb;
        private readonly IEnumerable<IPAddress> _udpDestinations;

        private const int _udpPort = 12345;

        private List<ISender> _senders = new List<ISender>();

        public SenderManager(bool detectUsb = false, IEnumerable<IPAddress> udpDestinations = null)
        {
            _detectUsb = detectUsb;
            _udpDestinations = udpDestinations;
        }

        public List<ISender> Senders { get => _senders; }

        public async Task ManageDevicesAsync()
        {
            do
            {
                try
                {
                    _senders.RemoveAll(s => s.IsBroken());

                    if (_detectUsb)
                    {
                        var allDevices = await GetAllUsbDevicesAsync();

                        var newSenders = allDevices.Where(s => !_senders.Select(sender => sender.Id).Contains(s.PortName));

                        foreach (var newSender in newSenders)
                        {
                            var usbSender = new UsbSender(newSender);

                            _senders.Add(usbSender);
                        }
                    }

                    if (_udpDestinations?.Any() ?? false)
                    {
                        foreach (var ip in _udpDestinations)
                        {
                            if (!_senders.Exists(s => s.Id == UdpSender.CreateId(ip, _udpPort)))
                            {
                                var udpSender = new UdpSender(ip, _udpPort);

                                _senders.Add(udpSender);
                            }
                        }
                    }

                    await Task.Delay(5000);
                }
                catch (Exception)
                {
                }
            }
            while (true);
        }

        private async Task<IEnumerable<SerialPort>> GetAllUsbDevicesAsync()
        {
            await Task.Yield();

            var devices = new List<SerialPort>();

            try
            {
                foreach (var portName in SerialPort.GetPortNames())
                {
                    Console.WriteLine(portName);

                    devices.Add(new SerialPort(portName)
                    {
                        BaudRate = 57600,
                        DtrEnable = true,
                        RtsEnable = false,
                        ReadTimeout = 100,
                        WriteTimeout = 100
                    });
                }
            }
            catch (Exception)
            {
            }

            return devices;
        }

        private bool IsTargetDevice(string name)
        {
            var names = new[] { "SparkFun", "Arduino", "UART" };
            return names.Any(n => name.Contains(n));
        }
    }
}
