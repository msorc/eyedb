import com.db4o.*;
import com.db4o.messaging.*;
import org.eyedb.benchmark.*;
import java.io.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Db4oBench extends Benchmark {

    public void prepare()
    {
	host = getProperties().getProperty("host");
	port = getProperties().getIntProperty("port");
	user = getProperties().getProperty("user");
	password = getProperties().getProperty("pass");
	
	try {
	    client = Db4o.openClient( host, port, user, password);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }

    public void run()
    {
	long nObjects = getProperties().getLongProperty( "objects");
	long nObjectsPerTransaction = getProperties().getLongProperty( "objects_per_transaction", 1000);

	for (long count = nObjects; count > 0; count -= nObjectsPerTransaction) {

	    for (long i = 0; i < nObjectsPerTransaction; i++) {
		Person p = new Person();
		p.setName("toto" + i);
		p.setAge((int)(i+1)%42);

		client.set( p);
	    }

	    client.commit();
	}
    }

    public void finish()
    {
	client.close();
    }

    private ObjectContainer client;
    private String host;
    private int port;
    private String user;
    private String password;
}

