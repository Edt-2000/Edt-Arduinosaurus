﻿using System;
using Windows.Foundation;
using Windows.Networking.Sockets;

namespace Dispedter.OSC
{
    internal class UdpListener : IDisposable
    {
        public int Port { get; private set; }

        public event TypedEventHandler<UdpListener, ByteEventArgs> BytePacketReceived;
        public event TypedEventHandler<UdpListener, OscEventArgs> OscPacketReceived;

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

                BytePacketReceived?.Invoke(this, new ByteEventArgs(data));

                try
                {
                    var packet = OscPacket.GetPacket(data);
                    OscPacketReceived?.Invoke(this, new OscEventArgs(packet));
                }
                catch (Exception ex)
                {
                    //  do not care
                }
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
