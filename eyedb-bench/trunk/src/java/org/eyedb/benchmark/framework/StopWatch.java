package org.eyedb.benchmark.framework;

import java.util.List;
import java.util.ArrayList;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class StopWatch {

    public class Lap {
	Lap( String name, long time)
	{
	    this.name = name;
	    this.time = time;
	}

	long getTime()
	{
	    return time; 
	}

	String getName()
	{
	    return name; 
	}

	private String name;
	private long time;
    }


    public StopWatch()
    {
	running = false;
	startTime = totalTime = lapTime = 0L;
	laps = new ArrayList<Lap>();
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
	laps.add( new Lap( name, delta));
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

    public List<Lap> getLaps()
    {
	return laps;
    }

    public int getLapCount()
    {
	return laps.size();
    }

    public String getLapName( int i)
    {
	return laps.get(i).getName();
    }

    public long getLapTime( int i)
    {
	return laps.get(i).getTime();
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
    private List<Lap> laps;
}

