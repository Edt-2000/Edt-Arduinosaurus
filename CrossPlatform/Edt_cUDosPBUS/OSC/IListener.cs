namespace Edt_cUDosPBUS.OSC
{
    public delegate void OscReceivedEvent(IListener listener, OscEventArgs args);

    public interface IListener
    {

        void AddPacketHandler(OscReceivedEvent handler);

        bool IsBroken();
        string Id { get; }
    }
}
