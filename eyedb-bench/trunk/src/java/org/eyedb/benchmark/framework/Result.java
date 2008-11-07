package org.eyedb.benchmark.framework;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Result {

    public Result()
    {
	headers = new ArrayList<String>();
	rows = new ArrayList<List<Long>>();
	rows.add( new ArrayList<Long>());
	current = 0;
	lapHeadersAdded = false;
    }

    public void addHeader( String header)
    {
	headers.add( header);
    }

    public void addValue( long value)
    {
	rows.get(current).add( new Long(value));
    }

    public void addLaps( List<StopWatch.Lap> laps)
    {
	if (!lapHeadersAdded) {
	    lapHeadersAdded = true;

	    for (StopWatch.Lap l : laps) {
		addHeader( l.getName());
	    }
	}

	for (StopWatch.Lap l : laps) 
	    addValue( l.getTime());
    }

    public void next()
    {
	current++;

	rows.add( new ArrayList<Long>());
    }

    public List<String> getHeaders()
    {
	return headers;
    }

    public List<Long> getValues( int i)
    {
	return rows.get(i);
    }

    public int getSize()
    {
	return current;
    }

    private List<String> headers;
    private List<List<Long>> rows;
    private int current;
    private boolean lapHeadersAdded;
}
