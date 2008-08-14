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
	OutputStream os = clientSocket.getOutputStream();
	InputStream is = clientSocket.getInputStream();
	byte[] b = new byte[n];

	os.write(b);
	os.flush();
	is.read(b);
    }

    protected Socket clientSocket;
}
