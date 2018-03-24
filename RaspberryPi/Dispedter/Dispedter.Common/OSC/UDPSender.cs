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

            _sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            var addresses = Dns.GetHostAddresses(address);
            if (addresses.Length == 0)
            {
                throw new Exception("Unable to find IP address for " + address);
            }

            _remoteIpEndPoint = new IPEndPoint(addresses[0], port);
        }

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
    }
}
