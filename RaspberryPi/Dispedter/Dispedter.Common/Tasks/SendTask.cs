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

        public Task _sendTask = Task.FromResult(0);

        public SendTask(ISender sender)
        {
            _sender = sender;

        }

        public string SenderId => _sender?.Id;

        public void AddMessage(OscPacket packet)
        {
            _queue.Enqueue(packet);

            TrySend();
        }

        public void AddMessage(IEnumerable<OscPacket> packets)
        {
            foreach (var packet in packets)
            {
                _queue.Enqueue(packet);
            }

            TrySend();
        }

        public async Task<SendTask> KeepAliveAsync()
        {
            do
            {
                // TODO: do stuff to keep it working

                // TODO: break when queue grows too much

                if (_sender.IsBroken())
                {
                    return this;
                }

                await Task.Delay(1000);

            } while (true);
        }

        private void TrySend()
        {
            if(!_sendTask.IsCompleted)
            {
                _queue.Clear();

                return;
            }

            var list = new List<Task>();

            while (_queue.Count > 0 && _queue.TryDequeue(out var packet))
            {
                list.Add(_sender.SendAsync(packet));
            }

            _sendTask = Task.WhenAll(list);
        }
    }
}
