using System;
using System.Collections.Generic;
using System.IO.Ports;

namespace Dispedter.Common.OSC
{
    public class UsbSender : ISender
    {
        private readonly SerialPort _serialPort;

        public UsbSender(string portName, uint baudRate)
        {
            _serialPort = new SerialPort(portName, (int)baudRate)
            {
                DtrEnable = true,
                RtsEnable = false,
                WriteTimeout = 10
            };

            _serialPort.DataReceived += DataReceived;

            TryOpen();
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

        private void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

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
    }
}
