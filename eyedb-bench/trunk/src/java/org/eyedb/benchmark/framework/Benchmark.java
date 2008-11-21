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

    public abstract String getName();

    public abstract String getDescription();

    public abstract String getRunDescription();

    public abstract void prepare();

    public abstract void run();

    public abstract void finish();

    public void bench()
    {
	result = new Result();

	prepare();
	stopWatch.start();
	run();
	stopWatch.stop();
	finish();
    }

    public StopWatch getStopWatch()
    {
	return stopWatch;
    }

    protected Properties getProperties()
    {
	return properties;
    }

    public Result getResult()
    {
	return result;
    }

    private StopWatch stopWatch;
    private Properties properties;
    private Result result;
}
