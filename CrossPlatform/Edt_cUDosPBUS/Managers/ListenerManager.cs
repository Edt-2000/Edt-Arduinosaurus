using Edt_cUDosPBUS.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace Edt_cUDosPBUS.Managers
{
    public class ListenerManager
    {
        private const int _udpPort = 12345;
        private readonly bool _detectUsb;

        private List<IListener> _listeners = new List<IListener>();
        private List<OscReceivedEvent> _eventHandlers = new List<OscReceivedEvent>();

        public ListenerManager(bool detectUsb = false)
        {
            _detectUsb = detectUsb;
        }

        public List<IListener> Listeners { get => _listeners; }

        public void AttachEventHandler(OscReceivedEvent eventHandler)
        {
            if (_listeners.Count > 0)
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
                    //if (_detectUsb)
                    //{
                    //    var allDevices = await GetAllUsbDevicesAsync();

                    //    var newListeners = allDevices.Where(s => !_listeners.Any(l => l.Id == s.PortName));

                    //    foreach (var newListener in newListeners)
                    //    {
                    //        var usbListener = new UsbListener(newListener);

                    //        //foreach (var handler in _eventHandlers)
                    //        //{
                    //        //    usbListener.AddPacketHandler(handler);
                    //        //}

                    //        _listeners.Add(usbListener);
                    //    }
                    //}

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

        private async Task<IEnumerable<SerialPort>> GetAllUsbDevicesAsync()
        {
            await Task.Yield();

            var devices = new List<SerialPort>();

            try
            {
                foreach (var portName in SerialPort.GetPortNames())
                {
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
