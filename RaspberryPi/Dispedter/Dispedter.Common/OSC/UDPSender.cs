using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Dispedter.Common.OSC
{
    public class UdpSender : ISender
    {
        enum State
        {
            Idle,
            Running,
            Broken
        }

        private IPEndPoint _remoteIpEndPoint;
        private Socket _socket;

        public int Port { get; }
        public string Address { get; }

        private State _state = State.Idle;

        public UdpSender(string address, int port)
        {
            Port = port;
            Address = address;

            Id = CreateId(address, port);

            SetDeviceState(State.Idle);

            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            var addresses = Dns.GetHostAddresses(address);
            if (addresses.Length == 0)
            {
                throw new Exception("Unable to find IP address for " + address);
            }

            _remoteIpEndPoint = new IPEndPoint(addresses[0], port);

            SetDeviceState(State.Running);
        }

        public static string CreateId(string address, int port) => $"{address}-{port.ToString()}";

        public string Id { get; private set; }

        //public void Send(byte[] message)
        //{
        //    _socket.SendTo(message, _remoteIpEndPoint);
        //}

        //public void Send(OscPacket packet)
        //{
        //    var data = packet.GetBytes();
        //    Send(data);
        //}

        //public void Send(IEnumerable<OscPacket> packets)
        //{
        //    foreach (var packet in packets)
        //    {
        //        Send(packet);
        //    }
        //}

        public async Task SendAsync(byte[] message)
        {
            try
            {
                if (_state == State.Running)
                {
                    await _socket.SendToAsync(message, SocketFlags.None, _remoteIpEndPoint);
                }
            }
            catch
            {
                SetDeviceState(State.Broken);
            }
        }

        public Task SendAsync(OscPacket packet)
        {
            return SendAsync(packet.GetBytes());
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
            return _state == State.Broken;
        }

        private void SetDeviceState(State newState)
        {
            _state = newState;

            switch (newState)
            {
                case State.Idle:
                    break;
                case State.Running:
                    break;
                case State.Broken:
                    Dispose(true);
                    break;
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
                    _socket?.Dispose();
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
