import gnu.java.net.local.LocalServerSocket;
import gnu.java.net.local.LocalSocketAddress;

public class LocalServer extends Server {
    public LocalServer( String path) throws Exception
    {
	super( 	new LocalServerSocket( new LocalSocketAddress( path)));
    }
}
