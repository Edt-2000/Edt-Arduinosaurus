using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;

namespace SharpOSC
{
    public class USB
    {
        private SerialPort serialPort;

        public USB(string portName, int baudRate)
        {
            serialPort = new SerialPort(portName, baudRate)
            {
                DtrEnable = true,
                RtsEnable = false,
                ReadTimeout = 100,
                WriteTimeout = 100
            };

            serialPort.DataReceived += SerialPort_DataReceived;

            serialPort.Open();
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if(e.EventType == SerialData.Chars)
            {
                var bytes = new byte[serialPort.BytesToRead];
                serialPort.Read(bytes, 0, bytes.Length);

                try
                {
                    var str = System.Text.Encoding.Default.GetString(bytes);

                    var msg = OscPacket.GetPacket(bytes, null) as OscMessage;

                    Console.WriteLine(msg.Arguments[0]);
                    Console.WriteLine(msg.Arguments[1]);
                }
                catch { }
            }
        }

        public void Send(byte[] message)
        {
            try
            {
                serialPort.Write(message, 0, message.Length);
            }
            catch(Exception)
            {
                serialPort.Close();
                serialPort.Open();
            }
        }

        public void Send(OscPacket packet)
        {
            byte[] data = packet.GetBytes();
            Send(data);
        }
    }
}