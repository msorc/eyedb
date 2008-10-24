package org.eyedb.benchmark.framework;

import java.io.FileInputStream;
import java.io.IOException;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark {

    public Benchmark()
    {
	stopWatch = new StopWatch();
	properties = new Properties();
    }

    abstract String getName();

    abstract String getDescription();

    abstract public void prepare();

    abstract public void run();

    abstract public void finish();

    public void bench()
    {
	prepare();
	stopWatch.start();
	run();
	stopWatch.stop();
	finish();
	System.out.println( "Total bench time: " + stopWatch.getTotalTime() + "mS");
    }

    public void report()
    {
    }

    protected Properties getProperties()
    {
	return properties;
    }

    public void loadProperties( String filename)
    {
	if (filename == null)
	    return;

	try {
	    properties.load( new FileInputStream( filename));
	}
	catch( IOException e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }

    private StopWatch stopWatch;
    private Properties properties;
}
