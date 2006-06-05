import org.eyedb.benchmark.*;
import org.hibernate.*;
import org.hibernate.cfg.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark extends org.eyedb.benchmark.Benchmark {

    public void prepare()
    {
        try {
            // Create the SessionFactory from hibernate.cfg.xml
            sessionFactory = new Configuration().configure().buildSessionFactory();
        } catch (Throwable ex) {
            System.err.println("Initial SessionFactory creation failed." + ex);
	    ex.printStackTrace();
	    System.exit(1);
        }
    }

    public void finish()
    {
        sessionFactory.close();
    }

    protected SessionFactory sessionFactory;
}

