import java.net.Socket;

public class TCPClient extends Client {
    public TCPClient( int port) throws Exception
    {
	super( new Socket( "localhost", port));
    }
}
