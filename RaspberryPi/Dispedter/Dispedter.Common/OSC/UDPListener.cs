using System;
using Windows.Foundation;
using Windows.Networking.Sockets;

namespace Dispedter.Common.OSC
{
    public class UdpListener : IListener
    {
        private event TypedEventHandler<IListener, OscEventArgs> OscPacketReceived;

        public int Port { get; private set; }

        public string Id => Port.ToString();
        
        private DatagramSocket _socket;
        private IAsyncAction _socketTask;
        
        public UdpListener(int port)
        {
            Port = port;

            _socket = new DatagramSocket();
            _socket.MessageReceived += SocketMessageReceived;

            _socketTask = _socket.BindServiceNameAsync($"{port}");
        }

        private void SocketMessageReceived(DatagramSocket sender, DatagramSocketMessageReceivedEventArgs args)
        {
            using (var reader = args.GetDataReader())
            {
                var data = new byte[reader.UnconsumedBufferLength];

                reader.ReadBytes(data);

                try
                {
                    var packet = OscPacket.GetPacket(data);

                    if (packet != null)
                    {
                        OscPacketReceived?.Invoke(this, new OscEventArgs(packet));
                    }
                }
                catch (Exception)
                {
                    //  do not care
                }
            }
        }
        public void AddPacketHandler(TypedEventHandler<IListener, OscEventArgs> handler)
        {
            OscPacketReceived += handler;
        }

        public bool IsBroken()
        {
            return false;
        }

        #region IDisposable Support
        private bool _disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    _socket.Dispose();
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
