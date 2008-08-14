import org.eyedb.java.net.unix.UnixSocket;
import org.eyedb.java.net.unix.UnixSocketAddress;

public class UnixClient extends Client {
    public UnixClient( String path) throws Exception
    {
	super( new UnixSocket( new UnixSocketAddress( path)));
    }
}
