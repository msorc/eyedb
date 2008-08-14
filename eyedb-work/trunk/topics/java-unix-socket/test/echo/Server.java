import java.net.Socket;
import java.net.ServerSocket;
import java.io.InputStream;
import java.io.OutputStream;

public class Server {
    public Server( ServerSocket serverSocket)
    {
	this.serverSocket = serverSocket;
    }

    public void run( int n) throws Exception
    {
	Socket clientSocket = serverSocket.accept();

	System.out.println( "Client connected: socket=" + clientSocket);

	InputStream is = clientSocket.getInputStream();
	OutputStream os = clientSocket.getOutputStream();
	byte[] b = new byte[n];

	is.read(b);
	os.write(b);
	os.flush();
    }

    protected ServerSocket serverSocket;
}
