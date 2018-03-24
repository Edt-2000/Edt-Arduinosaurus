using Dispedter.Common.OSC;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dispedter.Common.Tasks
{
    public class SendTask
    {
        private readonly ISender _sender;

        public Queue<OscPacket> _queue = new Queue<OscPacket>();

        public SendTask(ISender sender)
        {
            _sender = sender;

        }

        public void AddMessage(OscPacket packet)
        {
            _queue.Enqueue(packet);

            TrySend();
        }

        public async Task KeepAliveAsync()
        {
            do
            {
                // do stuff to keep it working

                await Task.Delay(1000);

            } while (true);
        }

        private void TrySend()
        {
            while (_queue.Count > 0 && _queue.TryDequeue(out var packet))
            {
                _sender.Send(packet);
            }
        }
    }
}
