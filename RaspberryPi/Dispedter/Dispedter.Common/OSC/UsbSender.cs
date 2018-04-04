using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;
using Windows.Storage.Streams;

namespace Dispedter.Common.OSC
{
    public class UsbSender : ISender
    {
        private readonly DeviceInformation _deviceInfo;
        private Task _deviceTask;
        private SerialDevice _device = null;

        private bool _healthy = false;
        private bool _broken = false;

        private DataWriter _serialPortStream;

        public UsbSender(DeviceInformation deviceInfo)
        {
            _deviceInfo = deviceInfo;
            _deviceTask = ConfigureDeviceAsync();

            Id = deviceInfo.Id;
        }

        public string Id { get; private set; }

        private async Task ConfigureDeviceAsync()
        {
            // TODO: this must not become a resouce hogging dead lock
            if (!_deviceTask?.IsCompleted ?? false)
            {
                await _deviceTask;
            }

            _healthy = false;

            var retries = 0;
            do
            {
                try
                {
                    _device = await SerialDevice.FromIdAsync(_deviceInfo.Id);

                    _device.BaudRate = 57600;

                    _device.WriteTimeout = TimeSpan.FromMilliseconds(1000);
                    _device.ReadTimeout = TimeSpan.FromMilliseconds(1000);

                    _device.IsDataTerminalReadyEnabled = true;
                    _device.IsRequestToSendEnabled = false;
                    _device.Parity = SerialParity.None;
                    _device.DataBits = 8;
                    _device.StopBits = SerialStopBitCount.One;
                    _device.Handshake = SerialHandshake.None;

                    _serialPortStream = new DataWriter(_device.OutputStream);

                    _healthy = true;

                    // we're healthy.
                    return;
                }
                catch (Exception)
                {

                }
                
                // give the system some time to recover
                await Task.Delay(retries * 1000);

            } while (++retries < 3);

            // kill me
            _broken = true;
        }

        public async Task SendAsync(byte[] message)
        {
            try
            {
                if (_healthy)
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
                        _serialPortStream?.Dispose();
                        _device?.Dispose();
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
