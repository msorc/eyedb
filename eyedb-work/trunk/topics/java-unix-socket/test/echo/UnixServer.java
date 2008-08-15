import org.eyedb.java.net.unix.UnixServerSocket;
import org.eyedb.java.net.unix.UnixSocketAddress;

public class UnixServer extends Server {
    public UnixServer( String path) throws Exception
    {
	super( new UnixServerSocket());

	System.out.println( serverSocket);

	serverSocket.bind( new UnixSocketAddress( path), 1000);
    }
}
