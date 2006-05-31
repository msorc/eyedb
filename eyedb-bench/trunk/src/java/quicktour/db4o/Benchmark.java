import com.db4o.*;
import com.db4o.messaging.*;
import org.eyedb.benchmark.*;
import java.io.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark extends org.eyedb.benchmark.Benchmark {

    public void prepare()
    {
	String host = getProperties().getProperty("host");
	int port = getProperties().getIntProperty("port");
	String user = getProperties().getProperty("user");
	String password = getProperties().getProperty("pass");
	
	try {
	    client = Db4o.openClient( host, port, user, password);
	}
	catch( IOException e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }

    public void finish()
    {
	client.close();
    }

    private ObjectContainer client;
}

