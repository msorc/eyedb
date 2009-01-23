import org.eyedb.java.net.unix.UnixSocket;
import org.eyedb.java.net.unix.UnixSocketAddress;

public class UnixClient extends Client {
    public UnixClient( String path, int n) throws Exception
    {
	super( new UnixSocket(), n);
	clientSocket.connect( new UnixSocketAddress( path));
    }
}
