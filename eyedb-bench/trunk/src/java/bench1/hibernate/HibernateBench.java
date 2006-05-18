import org.eyedb.benchmark.*;
import java.io.*;
import org.hibernate.*;
import org.hibernate.cfg.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class HibernateBench extends Benchmark {

    public void prepare()
    {
        try {
            // Create the SessionFactory from hibernate.cfg.xml
            sessionFactory = new Configuration().configure().buildSessionFactory();
        } catch (Throwable ex) {
            System.err.println("Initial SessionFactory creation failed." + ex);
	    System.exit(1);
        }
    }

    public void run()
    {
	long nObjects = getProperties().getLongProperty( "objects");
	long nObjectsPerTransaction = getProperties().getLongProperty( "objects_per_transaction", 1000);

	for (long count = nObjects; count > 0; count -= nObjectsPerTransaction) {

	    Session session = sessionFactory.getCurrentSession();

	    session.beginTransaction();

	    for (long i = 0; i < nObjectsPerTransaction; i++) {
		Person p = new Person();
		p.setName("toto" + i);
		p.setAge((int)(i+1)%42);

		session.save(p);
	    }

	    session.getTransaction().commit();
	}
    }

    public void finish()
    {
        sessionFactory.close();
    }

    private SessionFactory sessionFactory;
}
