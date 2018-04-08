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
        enum State
        {
            Idle,
            Configuring,
            Running,
            Broken
        }

        private readonly DeviceInformation _deviceInfo;
        private Task _deviceTask;
        private SerialDevice _device = null;

        private State _state = State.Idle;

        private DataWriter _serialPortStream;

        public UsbSender(DeviceInformation deviceInfo)
        {
            Id = deviceInfo.Id;

            SetDeviceState(State.Idle);

            _deviceInfo = deviceInfo;
            _deviceTask = ConfigureDeviceAsync();
        }

        public string Id { get; private set; }

        private async Task ConfigureDeviceAsync()
        {
            //Trace.TraceInformation($"Configuring {Id}..");

            if (_state != State.Idle)
            {
                return;
            }

            SetDeviceState(State.Configuring);

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

                    SetDeviceState(State.Running);

                    //Trace.TraceInformation($"Configuring {Id} success!");

                    // we're healthy.
                    return;
                }
                catch (Exception)
                {
                    //Trace.TraceWarning($"Configuring {Id} failed..");

                    retries++;
                }

                // give the system some time to recover
                await Task.Delay(1000);

            } while (retries < 3);

            // kill me
            SetDeviceState(State.Broken);

            //Trace.TraceError($"{Id} broken..");
        }

        public bool IsBroken()
        {
            return _state == State.Broken;
        }

        private void SetDeviceState(State newState)
        {
            _state = newState;

            switch (newState)
            {
                case State.Idle:
                    break;
                case State.Configuring:
                    break;
                case State.Running:
                    break;
                case State.Broken:
                    Dispose(true);
                    break;
            }
        }

        public async Task SendAsync(byte[] message)
        {
            try
            {
                if (_state == State.Running)
                {
                    _serialPortStream.WriteBytes(message);
                    await _serialPortStream.StoreAsync();
                }
            }
            catch
            {
                //Trace.TraceError($"Message to {Id} fail!");

                try
                {
                    _serialPortStream?.Dispose();
                    _device?.Dispose();
                }
                catch
                {
                }

                SetDeviceState(State.Broken);
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

        #region IDisposable Support
        private bool _disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    _serialPortStream?.Dispose();
                    _device?.Dispose();
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
