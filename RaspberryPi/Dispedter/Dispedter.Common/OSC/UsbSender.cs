using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Devices.SerialCommunication;
using Windows.Storage.Streams;

namespace Dispedter.Common.OSC
{
    public class UsbSender : ISender, IDisposable
    {
        private readonly SerialPort _serialPort = null;
        private readonly SerialDevice _device = null;

        private DataWriter _serialPortStream;

        public UsbSender(string portName, uint baudRate)
        {
            _serialPort = new SerialPort(portName, (int)baudRate)
            {
                DtrEnable = true,
                RtsEnable = false,
                WriteTimeout = 10
            };

            TryOpen();
        }

        public UsbSender(SerialDevice device)
        {
            _device = device;

            _device.BaudRate = 9600;

            _device.WriteTimeout = TimeSpan.FromMilliseconds(2000);

            _device.IsDataTerminalReadyEnabled = true;
            _device.IsRequestToSendEnabled = false;
            _device.Parity = SerialParity.None;
            _device.DataBits = 8;
            _device.StopBits = SerialStopBitCount.One;
            _device.Handshake = SerialHandshake.None;

            _serialPortStream = new DataWriter(_device.OutputStream);
        }

        public void TryOpen()
        {
            var i = 0;

            do
            {
                try
                {
                    _serialPort.Close();
                    _serialPort.Open();

                    return;
                }
                catch (Exception)
                {

                }
            }
            while (++i < 10);
        }

        public void Send(byte[] message)
        {
            try
            {
                _serialPort.Write(message, 0, message.Length);
            }
            catch (Exception)
            {
                TryOpen();
            }
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

        public async Task SendAsync(byte[] message)
        {
            try
            {
                _serialPortStream.WriteBytes(message);
                await _serialPortStream.StoreAsync();
            }
            catch (Exception ex)
            {

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
                    if(_serialPort != null)
                    {
                        _serialPort.Close();
                    }

                    if(_serialPortStream != null)
                    {
                        _serialPortStream.Dispose();
                        _device.Dispose();
                    }
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
