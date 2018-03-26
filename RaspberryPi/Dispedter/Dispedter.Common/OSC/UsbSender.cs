using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;
using Windows.Storage.Streams;

namespace Dispedter.Common.OSC
{
    public class UsbSender : ISender, IDisposable
    {
        private readonly DeviceInformation _deviceInfo;
        private Task _deviceTask;
        private SerialDevice _device = null;

        private bool _healty = false;
        private bool _broken = false;

        private DataWriter _serialPortStream;

        public UsbSender(DeviceInformation deviceInfo)
        {
            _deviceInfo = deviceInfo;
            _deviceTask = ConfigureDeviceAsync();
        }

        private async Task ConfigureDeviceAsync()
        {
            _healty = false;

            var retries = 0;
            do
            {
                try
                {
                    _device = await SerialDevice.FromIdAsync(_deviceInfo.Id);

                    _device.BaudRate = 9600;

                    _device.WriteTimeout = TimeSpan.FromMilliseconds(1000);
                    _device.ReadTimeout = TimeSpan.FromMilliseconds(1000);

                    _device.IsDataTerminalReadyEnabled = true;
                    _device.IsRequestToSendEnabled = false;
                    _device.Parity = SerialParity.None;
                    _device.DataBits = 8;
                    _device.StopBits = SerialStopBitCount.One;
                    _device.Handshake = SerialHandshake.None;

                    _serialPortStream = new DataWriter(_device.OutputStream);

                    _healty = true;
                }
                catch (Exception)
                {

                }
            } while (++retries < 3);

            _broken = true;
        }

        public async Task SendAsync(byte[] message)
        {
            try
            {
                if (_healty)
                {
                    _serialPortStream.WriteBytes(message);
                    await _serialPortStream.StoreAsync();
                }
            }
            catch (Exception)
            {
                try
                {
                    _serialPortStream?.Dispose();
                    _device?.Dispose();
                }
                catch (Exception)
                {
                }

                _deviceTask = ConfigureDeviceAsync();
            }
        }

        public Task SendAsync(OscPacket packet)
        {
            var data = packet.GetBytes();
            return SendAsync(data);
        }

        public async Task SendAsync(IEnumerable<OscPacket> packets)
        {
            foreach (var packet in packets)
            {
                await SendAsync(packet);
            }
        }

        public bool IsBroken()
        {
            return _broken;
        }

        #region IDisposable Support
        private bool _disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    if (_serialPortStream != null)
                    {
                        _serialPortStream.Dispose();
                        _device.Dispose();
                    }
                }

                _disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
        }
        #endregion
    }
}
