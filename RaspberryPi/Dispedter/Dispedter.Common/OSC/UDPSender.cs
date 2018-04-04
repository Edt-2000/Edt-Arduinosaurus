using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace Dispedter.Common.OSC
{
    public class UdpSender : ISender
    {
        private IPEndPoint _remoteIpEndPoint;
        private Socket _sock;

        public int Port { get; }
        public string Address { get; }

        public UdpSender(string address, int port)
        {
            Port = port;
            Address = address;

            Id = $"{address}-{port.ToString()}";

            _sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            var addresses = Dns.GetHostAddresses(address);
            if (addresses.Length == 0)
            {
                throw new Exception("Unable to find IP address for " + address);
            }

            _remoteIpEndPoint = new IPEndPoint(addresses[0], port);
        }

        public string Id { get; private set; }

        public void Send(byte[] message)
        {
            _sock.SendTo(message, _remoteIpEndPoint);
        }

        public void Send(OscPacket packet)
        {
            var data = packet.GetBytes();
            Send(data);
        }

        public void Send(IEnumerable<OscPacket> packets)
        {
            foreach (var packet in packets)
            {
                Send(packet);
            }
        }

        public Task SendAsync(byte[] message)
        {
            throw new NotImplementedException();
        }

        public Task SendAsync(OscPacket packet)
        {
            throw new NotImplementedException();
        }

        public Task SendAsync(IEnumerable<OscPacket> packets)
        {
            throw new NotImplementedException();
        }

        public void Close()
        {
            _sock.Close();
        }

        public bool IsBroken()
        {
            throw new NotImplementedException();
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~UdpSender() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
