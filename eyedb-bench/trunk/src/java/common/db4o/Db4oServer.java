import com.db4o.*;
import com.db4o.messaging.*;
import org.eyedb.benchmark.Properties;
import java.io.*;

public class Db4oServer implements MessageRecipient {
  
    public static void main(String[] args) 
    {
	Db4oServer server = new Db4oServer();

	if (args.length >= 1)
	    server.loadProperties( args[0]);

	server.runServer();
    } 
  
    public Db4oServer()
    {
	properties = new Properties();
    }

    public void loadProperties( String filename)
    {
	try {
	    properties.load( new FileInputStream( filename));
	}
	catch( IOException e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }


    public void runServer()
    {
	String file = properties.getProperty( "file");
	int port = properties.getIntProperty( "port");
	String user = properties.getProperty( "user");
	String pass = properties.getProperty( "pass");

	synchronized(this) {
	    
	    ObjectServer db4oServer = Db4o.openServer(file, port);

	    db4oServer.grantAccess(user, pass);
      
	    db4oServer.ext().configure().clientServer().setMessageRecipient(this);
      
	    Thread.currentThread().setName(this.getClass().getName());
      
	    Thread.currentThread().setPriority(Thread.MIN_PRIORITY);
	    try {
		if(! stop) {
		    // wait forever for notify() from close()
		    this.wait(Long.MAX_VALUE);   
		}
	    }
	    catch (Exception e) {
		e.printStackTrace();
	    }
	    db4oServer.close();
	}
    }
  
    public void processMessage( MessageContext con, Object message)
    {
// 	if (message instanceof StopServer) {
// 	    close();
// 	}
    }
  
    public void close()
    {
	synchronized(this) {
	    stop = true;
	    this.notify();
	}
    }

    private boolean stop = false;
    private Properties properties;
}
