import java.net.ServerSocket;

public class TCPServer extends Server {
    public TCPServer( int port, int n) throws Exception
    {
	super( 	new ServerSocket( port), n);
    }
}
