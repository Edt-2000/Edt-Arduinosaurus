using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        private int _failures = 0;

        private bool _isConfiguring = false;

        private DataWriter _serialPortStream;

        public UsbSender(DeviceInformation deviceInfo)
        {
            Id = deviceInfo.Id;

            _deviceInfo = deviceInfo;
            _deviceTask = ConfigureDeviceAsync();
        }

        public string Id { get; private set; }

        private async Task ConfigureDeviceAsync()
        {
            Trace.TraceInformation($"Configuring {Id}..");

            if(_isConfiguring)
            {
                return;
            }

            _isConfiguring = true;
            _healthy = false;
            
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
                    _isConfiguring = false;
                    _failures = 0;

                    Trace.TraceInformation($"Configuring {Id} success!");

                    // we're healthy.
                    return;
                }
                catch (Exception)
                {
                    Trace.TraceWarning($"Configuring {Id} failed..");

                    _failures++;
                }
                
                // give the system some time to recover
                await Task.Delay(1000);

            } while (!IsBroken());

            // kill me
            _isConfiguring = false;

            Trace.TraceError($"{Id} broken..");
        }

        public Task ReconnectAsync()
        {
            Trace.TraceWarning($"Reconnecting {Id}..");

            return ConfigureDeviceAsync();
        }

        public async Task SendAsync(byte[] message)
        {
            try
            {
                if (_healthy)
                {
                    _serialPortStream.WriteBytes(message);
                    await _serialPortStream.StoreAsync();

                    Trace.TraceInformation($"Message to {Id} success!");
                }
            }
            catch (Exception)
            {
                Trace.TraceError($"Message to {Id} fail!");
                _failures++;

                Trace.TraceError($"Failures: {_failures}.");

                try
                {
                    _serialPortStream?.Dispose();
                    _device?.Dispose();
                }
                catch (Exception)
                {
                }
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
            return _failures > 3;
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
