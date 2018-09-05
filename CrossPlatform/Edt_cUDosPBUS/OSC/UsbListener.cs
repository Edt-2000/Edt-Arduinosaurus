using System;
using System.IO.Ports;
using System.Threading.Tasks;

namespace Edt_cUDosPBUS.OSC
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

        private readonly SerialPort _port;
        private Task _deviceTask;
        private Task _readTask;

        private State _state = State.Idle;
        
        private byte[] _buffer;


        private event OscReceivedEvent OscPacketReceived;

        public UsbListener(SerialPort port)
        {
            Id = port.PortName;

            SetDeviceState(State.Idle);

            _port = port;
            _deviceTask = ConfigureDeviceAsync();
            _buffer = new byte[128];
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
                    _port.DataReceived += _port_DataReceived;

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

        private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var port = (SerialPort)sender;

            var bytesRead = _port.Read(_buffer, 0, port.BytesToRead);

            if (bytesRead > 0)
            {
                var messages = OscPacket.GetPackets(_buffer, bytesRead);

                foreach (var message in messages)
                {
                    OscPacketReceived?.Invoke(this, new OscEventArgs(message));
                }
            }
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

        public void AddPacketHandler(OscReceivedEvent handler)
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
