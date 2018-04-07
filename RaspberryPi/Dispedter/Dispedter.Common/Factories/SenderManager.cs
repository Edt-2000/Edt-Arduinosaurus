using Dispedter.Common.OSC;
using Dispedter.Common.Tasks;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;

namespace Dispedter.Common.Factories
{
    public class SenderManager
    {
        private readonly bool _detectUsb;
        private readonly bool _detectUdp;

        private List<SendTask> _senders = new List<SendTask>();

        public SenderManager(bool detectUsb = false, bool detectUdp = false)
        {
            _detectUsb = detectUsb;
            _detectUdp = detectUdp;
        }

        public List<SendTask> Senders { get => _senders; }

        public async Task ScanForDevicesAsync()
        {
            do
            {
                try
                {
                    var allDevices = await GetAllDevicesAsync();

                    var newSenders = allDevices.Where(s => !_senders.Select(sendTask => sendTask.SenderId).Contains(s.Id));

                    foreach (var newSender in newSenders)
                    {
                        Trace.TraceInformation($"A new sender has been found: {newSender.Id}!");

                        var usbSender = new UsbSender(newSender);

                        _senders.Add(new SendTask(usbSender));
                    }

                    await Task.Delay(15000);

                }
                catch (Exception)
                {
                }
            }
            while (true);
        }

        public async Task ManageDevicesAsync()
        {
            do
            {
                try
                {
                    while (!_senders.Any())
                    {
                        await Task.Delay(1000);
                    }

                    var defectiveTask = await Task.WhenAny(_senders.Select(sender => sender.KeepAliveAsync()));

                    // a device has failed. see which, remove it
                    var brokenSender = defectiveTask.Result;

                    Trace.TraceWarning($"A sender broke down: {brokenSender.SenderId}!");

                    _senders.Remove(brokenSender);
                }
                catch (Exception)
                {
                    ;
                }
            }
            while (true);
        }

        private async Task<IEnumerable<DeviceInformation>> GetAllDevicesAsync()
        {
            var devices = new List<DeviceInformation>();

            try
            {
                if (_detectUsb)
                {
                    var _serialSelector = SerialDevice.GetDeviceSelector();
                    var infos = await DeviceInformation.FindAllAsync(_serialSelector);

                    foreach (var info in infos.Where(i => IsTargetDevice(i.Name)))
                    {
                        devices.Add(info);
                    }
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
