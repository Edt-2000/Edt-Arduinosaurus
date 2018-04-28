using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;
using Windows.Foundation;

namespace Dispedter.Common.Managers
{
    public class ListenerManager
    {
        private const int _udpPort = 12345;
        private readonly bool _detectUsb;

        private List<IListener> _listeners = new List<IListener>();
        private List<TypedEventHandler<IListener, OscEventArgs>> _eventHandlers = new List<TypedEventHandler<IListener, OscEventArgs>>();

        public ListenerManager(bool detectUsb = false)
        {
            _detectUsb = detectUsb;
        }

        public List<IListener> Listeners { get => _listeners; }

        public void AttachEventHandler(TypedEventHandler<IListener, OscEventArgs> eventHandler)
        {
            if(_listeners.Count > 0)
            {
                throw new Exception("Attach event handlers prior to manage devices.");
            }

            _eventHandlers.Add(eventHandler);
        }

        public async Task ManageDevicesAsync()
        {
            do
            {
                try
                {
                    if (_detectUsb)
                    {
                        var allDevices = await GetAllUsbDevicesAsync();

                        var newListeners = allDevices.Where(s => !_listeners.Any(l => l.Id == s.Id));

                        foreach (var newListener in newListeners)
                        {
                            var usbListener = new UsbListener(newListener);

                            foreach (var handler in _eventHandlers)
                            {
                                usbListener.AddPacketHandler(handler);
                            }

                            _listeners.Add(usbListener);
                        }
                    }

                    _listeners.RemoveAll(l => l.IsBroken());

                    if (!_listeners.Exists(s => s.Id == _udpPort.ToString()))
                    {
                        var udpListener = new UdpListener(_udpPort);

                        foreach (var handler in _eventHandlers)
                        {
                            udpListener.AddPacketHandler(handler);
                        }

                        _listeners.Add(udpListener);
                    }

                    await Task.Delay(5000);
                }
                catch (Exception)
                {
                }
            }
            while (true);
        }

        private async Task<IEnumerable<DeviceInformation>> GetAllUsbDevicesAsync()
        {
            var devices = new List<DeviceInformation>();

            try
            {
                var _serialSelector = SerialDevice.GetDeviceSelector();
                var infos = await DeviceInformation.FindAllAsync(_serialSelector);

                foreach (var info in infos.Where(i => IsTargetDevice(i.Name)))
                {
                    devices.Add(info);
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
