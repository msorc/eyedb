package org.eyedb.benchmark.framework;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class StopWatch {

    public StopWatch()
    {
	running = false;
	startTime = totalTime = lapTime = 0L;
	laps = new ArrayList<Map.Entry<String,Long>>();
    }

    public final void start()
    {
	running = true;
	startTime = systemTime();
	lapTime = 0;
    }

    public long stop()
    {
	if (running) {
	    running = false;
	    totalTime = time();
	} else 
	    totalTime = 0;

	return totalTime;
    }

    public long lap( String name)
    {
	long t = time();
	long delta = t - lapTime;

	Map.Entry e = new AbstractMap.SimpleEntry<String,Long>( name, new Long(delta));

	laps.add( e);
	lapTime = t;

	return delta;
    }

    public void reset()
    {
	running = false;
	startTime = totalTime = lapTime = 0L;
	laps.clear();
    }

    public long getTotalTime()
    {
	return totalTime;
    }

    public List< Map.Entry<String,Long> > getLaps()
    {
	return laps;
    }

    private long systemTime()
    {
	return System.currentTimeMillis();
    }

    private long time()
    {
	return systemTime() - startTime;
    }

    private boolean running;
    private long startTime;
    private long totalTime;
    private long lapTime;
    private List< Map.Entry<String,Long> > laps;
}

