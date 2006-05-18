import com.db4o.*;
import com.db4o.messaging.*;
import org.eyedb.benchmark.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Db4oBenchLocal extends Benchmark {

    public void prepare()
    {
	String file = getProperties().getProperty("file");
	db = Db4o.openFile( file);
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

		db.set( p);
	    }

	    db.commit();
	}
    }

    public void finish()
    {
	db.close();
    }

    private ObjectContainer db;
}

