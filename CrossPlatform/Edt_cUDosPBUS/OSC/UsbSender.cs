using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Threading.Tasks;

namespace Edt_cUDosPBUS.OSC
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

        private readonly SerialPort _port;
        private Task _deviceTask;
        
        private State _state = State.Idle;
        
        public UsbSender(SerialPort port)
        {
            Id = port.PortName;

            SetDeviceState(State.Idle);

            _port = port;
            _deviceTask = ConfigureDeviceAsync();
        }

        public string Id { get; private set; }

        private async Task ConfigureDeviceAsync()
        {
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
                    _port.Open();

                    SetDeviceState(State.Running);

                    // we're healthy.
                    return;
                }
                catch (Exception)
                {
                    retries++;
                }

                // give the system some time to recover
                await Task.Delay(1000);

            } while (retries < 3);

            // kill me
            SetDeviceState(State.Broken);
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
                    _port.Write(message, 0, message.Length);
                }
            }
            catch
            {
                SetDeviceState(State.Broken);
                
                try
                {
                    _port.Dispose();
                }
                catch
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

        #region IDisposable Support
        private bool _disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    _port.Dispose();
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
