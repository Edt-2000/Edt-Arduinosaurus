using Dispedter.Common.Managers;
using System;
using System.Net;
using System.Threading.Tasks;

namespace Dispedter.Proxy
{
    class Program
    {
        private readonly ListenerManager _listenerManager = new ListenerManager(detectUsb: false);
        private readonly SenderManager _senderManager = new SenderManager(detectUsb: true);

        static async Task Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }
    }
}
