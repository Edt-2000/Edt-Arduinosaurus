//using Dispedter.Common.OSC;
//using System;
//using System.Collections.Generic;
//using System.Diagnostics;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

//namespace Dispedter.Common.Tasks
//{
//    public class SendTask
//    {
//        private readonly ISender _sender;
        
//        public Task _sendTask = Task.FromResult(0);

//        public SendTask(ISender sender)
//        {
//            _sender = sender;

//        }

//        public string SenderId => _sender?.Id;

//        public void AddMessage(OscPacket packet)
//        {
//            TrySend(packet);
//        }

//        public void AddMessage(IEnumerable<OscPacket> packets)
//        {
//            foreach (var packet in packets)
//            {
//                TrySend(packet);
//            }
//        }

//        public async Task<SendTask> KeepAliveAsync()
//        {
//            do
//            {
//                await _sender.DeviceBreaksAsync();

//                // try reconnecting once, otherwise, kill this task
//                if (!await _sender.ReconnectAsync())
//                {
//                    return this;
//                }
//            }
//            while (true);
//        }

//        private void TrySend(OscPacket packet)
//        {
//            // TODO: is this the best option for when it is busy?
//            if (!_sendTask.IsCompleted)
//            {
//                return;
//            }

//            _sendTask = _sender.SendAsync(packet);
//        }
//    }
//}
