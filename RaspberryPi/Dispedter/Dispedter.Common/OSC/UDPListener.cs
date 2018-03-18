using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Windows.Foundation;
using Windows.Networking.Sockets;

namespace Dispedter.Common.OSC
{
    public class UdpListener : IDisposable
    {
        public int Port { get; private set; }

        public event TypedEventHandler<UdpListener, ByteEventArgs> OnOscPacketReceive;
        public event TypedEventHandler<UdpListener, OscEventArgs> OnBytePacketReceive;

        private DatagramSocket _socket;

        UdpClient _receivingUdpClient;
        
        
        public UdpListener(int port)
        {
            Port = port;

            _socket = new DatagramSocket();

            _socket.add_MessageReceived(OnOscPacketReceive)


            //_queue = new Queue<byte[]>();
            //_closingEvent = new ManualResetEvent(false);
            //_callbackLock = new object();

            // try to open the port 10 times, else fail
            for (int i = 0; i < 10; i++)
            {
                try
                {
                    _receivingUdpClient = new UdpClient(port);
                    break;
                }
                catch (Exception)
                {
                    // Failed in ten tries, throw the exception and give up
                    if (i >= 9)
                    {
                        throw;
                    }

                    Thread.Sleep(5);
                }
            }
            _remoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);

            // setup first async event
            AsyncCallback callBack = new AsyncCallback(ReceiveCallback);
            _receivingUdpClient.BeginReceive(callBack, null);
        }

        private void SocketMessageReceived(DatagramSocket sender, DatagramSocketMessageReceivedEventArgs args)
        {

        }

        void ReceiveCallback(IAsyncResult result)
        {
            Monitor.Enter(_callbackLock);
            Byte[] bytes = null;

            IPEndPoint remoteEP = null;

            try
            {
                bytes = _receivingUdpClient.EndReceive(result, ref remoteEP);
            }
            catch (ObjectDisposedException e)
            {
                // Ignore if disposed. This happens when closing the listener
            }

            // Process bytes
            if (bytes != null && bytes.Length > 0)
            {
                OnBytePacketReceive?.Invoke(this, new ByteEventArgs(bytes));

                OscPacket packet = null;
                try
                {
                    packet = OscPacket.GetPacket(bytes, remoteEP);
                    OnOscPacketReceive?.Invoke(this, new OscEventArgs(packet));
                }
                catch (Exception)
                {
                    // If there is an error reading the packet, null is sent to the callback
                }

                if(OnBytePacketReceive == null && OnOscPacketReceive == null)
                {
                    lock (_queue)
                    {
                        _queue.Enqueue(bytes);
                    }
                }   
            }

            if (_closing)
            {
                _closingEvent.Set();
            }
            else
            {
                // Setup next async event
                AsyncCallback callBack = new AsyncCallback(ReceiveCallback);
                _receivingUdpClient.BeginReceive(callBack, remoteEP);
            }
            Monitor.Exit(_callbackLock);
        }

        public void Close()
        {
            lock (_callbackLock)
            {
                _closingEvent.Reset();
                _closing = true;
                _receivingUdpClient.Close();
            }
            _closingEvent.WaitOne();

        }

        public void Dispose()
        {
            Close();
        }

        public OscPacket Receive()
        {
            if (_closing) throw new Exception("UDPListener has been closed.");

            lock (_queue)
            {
                if (_queue.Count() > 0)
                {
                    byte[] bytes = _queue.Dequeue();
                    var packet = OscPacket.GetPacket(bytes, null);
                    return packet;
                }
                else
                    return null;
            }
        }

        public byte[] ReceiveBytes()
        {
            if (_closing) throw new Exception("UDPListener has been closed.");

            lock (_queue)
            {
                if (_queue.Count() > 0)
                {
                    byte[] bytes = _queue.Dequeue();
                    return bytes;
                }
                else
                    return null;
            }
        }

    }
}
