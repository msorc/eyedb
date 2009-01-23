import java.net.Socket;
import java.net.ServerSocket;
import java.io.InputStream;
import java.io.OutputStream;

public class Server extends Thread {

    public Server( ServerSocket serverSocket, int n)
    {
	this.serverSocket = serverSocket;
	this.n = n;
    }

    public void run()
    {
	try {
	    Socket clientSocket = serverSocket.accept();

	    System.out.println( "[server] client connected: socket=" + clientSocket);

	    InputStream is = clientSocket.getInputStream();
	    OutputStream os = clientSocket.getOutputStream();
	    byte[] b = new byte[n];

	    is.read(b);

	    System.out.println( "[server] read " + n + " bytes");

	    os.write(b);
	    os.flush();

	    System.out.println( "[server] writen " + n + " bytes");
	}
	catch( Exception e) {
	    e.printStackTrace();
	}
    }

    protected ServerSocket serverSocket;
    private int n;
}
