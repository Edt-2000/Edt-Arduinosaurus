using System;
using System.Net;
using System.Net.Sockets;

namespace Edt_cUDosPBUS.OSC
{
    public class UdpListener : IListener
    {
        private int _messages = 0;

        private event OscReceivedEvent OscPacketReceived;

        public int Port { get; private set; }
        private object _lock = new object();

        public string Id => Port.ToString();

        private UdpClient _client;

        public UdpListener(int port)
        {
            Port = port;

            _client = new UdpClient(12345);

            // setup first async event
            AsyncCallback callBack = new AsyncCallback(ReceiveCallback);
            _client.BeginReceive(callBack, null);
        }

        void ReceiveCallback(IAsyncResult result)
        {
            lock (_lock)
            {
                Console.WriteLine($"Message ({++_messages}) received.");

                Byte[] bytes = null;

                IPEndPoint remoteEP = null;

                try
                {
                    bytes = _client.EndReceive(result, ref remoteEP);
                }
                catch (ObjectDisposedException e)
                {
                    // Ignore if disposed. This happens when closing the listener
                }

                // Process bytes
                if (bytes != null && bytes.Length > 0)
                {
                    OscPacket packet = null;
                    try
                    {
                        packet = OscPacket.GetPacket(bytes);
                    }
                    catch (Exception e)
                    {
                        // If there is an error reading the packet, null is sent to the callback
                    }

                    OscPacketReceived?.Invoke(this, new OscEventArgs(packet));
                }

                // Setup next async event
                AsyncCallback callBack = new AsyncCallback(ReceiveCallback);
                _client.BeginReceive(callBack, remoteEP);
            }
        }

        //private void SocketMessageReceived(DatagramSocket sender, DatagramSocketMessageReceivedEventArgs args)
        //{
        //    using (var reader = args.GetDataReader())
        //    {
        //        var data = new byte[reader.UnconsumedBufferLength];

        //        reader.ReadBytes(data);

        //        try
        //        {
        //            var packet = OscPacket.GetPacket(data);

        //            if (packet != null)
        //            {
        //                OscPacketReceived?.Invoke(this, new OscEventArgs(packet));
        //            }
        //        }
        //        catch (Exception)
        //        {
        //            //  do not care
        //        }
        //    }
        //}
        public void AddPacketHandler(OscReceivedEvent handler)
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
                    _client.Dispose();
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
