package org.eyedb.benchmark.framework;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

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
    }

    public void addHeader( String header)
    {
	headers.add( header);
    }

    public void add( long value)
    {
	rows.get(current).add( new Long(value));
    }

    public void add( List< Map.Entry<String,Long> > values)
    {
	for( Map.Entry<String,Long> e: values)
	    rows.get(current).add( e.getValue());
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
}
