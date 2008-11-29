package org.eyedb.benchmark.framework;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Result {

	public static class Value {
		public Value( String label, long value)
		{
			this.label = label;
			this.value = value;
		}

		public String getLabel()
		{
			return label;
		}

		public long getValue()
		{
			return value;
		}

		private String label;
		private long value;
	}

	public Result()
	{
		headers = new ArrayList<String>();
		rows = new ArrayList<List<Value>>();
		rows.add( new ArrayList<Value>());
		current = 0;
		lapHeadersAdded = false;
	}

	public void addHeader( String header)
	{
		headers.add( header);
	}

	public void addValue( String label, long value)
	{
		rows.get(current).add( new Value(label, value));
	}

	/*
      @deprecated
	 */
	public void addValue( long value)
	{
		addValue( "", value);
	}

	public void addLaps( List<StopWatch.Lap> laps)
	{
		if (!lapHeadersAdded) {
			lapHeadersAdded = true;

			for (StopWatch.Lap l : laps) {
				addHeader( l.getLabel());
			}
		}

		for (StopWatch.Lap l : laps) 
			addValue( l.getTime());
	}

	public void next()
	{
		current++;

		rows.add( new ArrayList<Value>());
	}

	public List<String> getHeaders()
	{
		return headers;
	}

	public List<Result.Value> getValues( int i)
	{
		return rows.get(i);
	}

	public int getSize()
	{
		return current;
	}

	private List<String> headers;
	private List<List<Value>> rows;
	private int current;
	private boolean lapHeadersAdded;
}
