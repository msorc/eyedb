import java.net.Socket;
import java.io.InputStream;
import java.io.OutputStream;

public class Client extends Thread {

    public Client( Socket clientSocket, int n)
    {
	this.clientSocket = clientSocket;
	this.n = n;
    }

    public void run()
    {
	try {
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
	catch( Exception e) {
	    e.printStackTrace();
	}
    }

    protected Socket clientSocket;
    private int n;
}
