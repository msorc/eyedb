import gnu.java.net.local.LocalSocket;
import gnu.java.net.local.LocalSocketAddress;

public class LocalClient extends Client {
    public LocalClient( String path) throws Exception
    {
	super( new LocalSocket( new LocalSocketAddress( path)));
    }
}
