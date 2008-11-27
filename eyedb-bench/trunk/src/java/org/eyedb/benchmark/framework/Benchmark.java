package org.eyedb.benchmark.framework;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark {

    public Benchmark()
    {
	stopWatch = new StopWatch();
	properties = new Properties();
	result = new Result();
	context = new Context();
    }

    public abstract String getName();

    public abstract String getDescription();

    public abstract String getImplementation();

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

    public void setStopWatch(StopWatch stopWatch)
    {
        this.stopWatch = stopWatch;
    }

    protected Properties getProperties()
    {
	return properties;
    }

    protected void setProperties(Properties properties)
    {
        this.properties = properties;
    }

    public Result getResult()
    {
	return result;
    }

    public void setResult(Result result)
    {
        this.result = result;
    }

    public Context getContext()
    {
	return context;
    }

    public void setContext(Context context)
    {
        this.context = context;
    }

    private StopWatch stopWatch;
    private Properties properties;
    private Result result;
    private Context context;
 }
