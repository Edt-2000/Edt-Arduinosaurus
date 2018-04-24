using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;
using Windows.Devices.SerialCommunication;
using Windows.Foundation;
using Windows.Storage.Streams;

namespace Dispedter.Common.OSC
{
    public class UsbListener : IListener
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
        private Task _readTask;
        private SerialDevice _device = null;

        private State _state = State.Idle;

        private DataReader _serialPortStream;
        private byte[] _buffer;

        private event TypedEventHandler<IListener, OscEventArgs> OscPacketReceived;

        public UsbListener(DeviceInformation deviceInfo)
        {
            Id = deviceInfo.Id;

            SetDeviceState(State.Idle);

            _deviceInfo = deviceInfo;
            _deviceTask = ConfigureDeviceAsync();
            _buffer = new byte[28];
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
                    _device.ErrorReceived += ErrorReceived;

                    _serialPortStream = new DataReader(_device.InputStream);
                    _serialPortStream.InputStreamOptions = InputStreamOptions.Partial;

                    _readTask = ReadTaskAsync();

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

        private void ErrorReceived(SerialDevice sender, ErrorReceivedEventArgs args)
        {
            //SetDeviceState(State.Broken);
        }

        private async Task ReadTaskAsync()
        {
            do
            {
                var numberOfBytes = (uint)_buffer.Length;

                try
                {
                    var bytesRead = await _serialPortStream.LoadAsync(numberOfBytes);

                    if (bytesRead > 0)
                    {
                        _serialPortStream.ReadBytes(_buffer);

                        var message = OscPacket.GetPacket(_buffer);
                        OscPacketReceived?.Invoke(this, new OscEventArgs(message));

                        //var messages = OscPacket.GetPackets(_buffer, (int)bytesRead);

                        //foreach (var message in messages)
                        //{
                        //    OscPacketReceived?.Invoke(this, new OscEventArgs(message));
                        //}
                    }
                }
                catch
                {
                    
                }
            }
            while (true);
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

        public void AddPacketHandler(TypedEventHandler<IListener, OscEventArgs> handler)
        {
            OscPacketReceived += handler;
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
