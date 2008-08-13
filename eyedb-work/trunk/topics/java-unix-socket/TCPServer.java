import java.net.ServerSocket;

public class TCPServer extends Server {
    public TCPServer( int port) throws Exception
    {
	super( 	new ServerSocket( port));
    }
}
