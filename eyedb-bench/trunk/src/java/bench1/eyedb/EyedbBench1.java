import org.eyedb.*;
import org.eyedb.benchmark.*;

import person.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyedbBench1 extends Benchmark {

    public void prepare()
    {
	String databaseName = getProperties().getProperty( "database");

	String[] args = new String[3];
	int i = 0;
	args[i++] = "--user=" + System.getProperty( "user.name");
	args[i++] = "--dbm=default";
	args[i++] = "--port=" + getProperties().getProperty( "tcp_port");

	org.eyedb.Root.init(databaseName, args);
     
	try {
	    // Initialize the person package
	    person.Database.init();

	    // Open the connection with the backend
	    connection = new org.eyedb.Connection();

	    // Open the database
	    database = new person.Database( databaseName);
	    database.open(connection, org.eyedb.Database.DBRW);
	}
	catch(org.eyedb.Exception e) { // Catch any eyedb exception
	    e.printStackTrace();
	    System.exit(1);
	}
    }

    public void run()
    {
	long nRuns = getProperties().getLongProperty( "runs");
	long nObjectsPerTransaction = getProperties().getLongProperty( "objects_per_transaction", 1000);

	try {
	    for (long count = nRuns; count > 0; count -= nObjectsPerTransaction) {
		database.transactionBegin();

		for (long i = 0; i < nObjectsPerTransaction; i++) {
		    Person p = new Person(database);
		    p.setName("toto"+i);
		    p.setAge((int)(i+1)%42);

		    p.store(org.eyedb.RecMode.FullRecurs);
		}

		database.transactionCommit();
	    }
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }

    public void finish()
    {
	// Close the database
    }

    private org.eyedb.Connection connection;
    private person.Database database;
}
