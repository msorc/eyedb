package org.eyedb.benchmark.framework;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class StopWatch {

    public StopWatch()
    {
	reset();
    }

    public final void start()
    {
	startTime = System.currentTimeMillis();
	running = true;
    }

    public final void stop()
    {
	if (running) {
	    long currentTime = System.currentTimeMillis();

	    totalTime = currentTime - startTime;

	    running = false;
	}
    }

    public long getTotalTime()
    {
	return totalTime;
    }

    public final void reset()
    {
	running = false;
	startTime = totalTime = 0L;
    }

    private long startTime, totalTime;
    private boolean running;
}

