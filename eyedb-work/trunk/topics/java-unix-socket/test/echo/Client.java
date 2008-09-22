import java.net.Socket;
import java.io.InputStream;
import java.io.OutputStream;

public class Client {
    public Client( Socket clientSocket)
    {
	this.clientSocket = clientSocket;
    }

    public void run( int n) throws Exception
    {
	System.out.println( "[client] connected: socket=" + clientSocket);

	OutputStream os = clientSocket.getOutputStream();
	InputStream is = clientSocket.getInputStream();
	byte[] b = new byte[n];

	os.write(b);
	os.flush();

	System.out.println( "[client] writen " + n + " bytes");

	is.read(b);

	System.out.println( "[client] read " + n + " bytes");
    }

    protected Socket clientSocket;
}
