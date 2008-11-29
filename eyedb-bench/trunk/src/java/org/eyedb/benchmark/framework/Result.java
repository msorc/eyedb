package org.eyedb.benchmark.framework;

import java.util.ArrayList;
import java.util.Collection;
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
		
		public Value( Value v)
		{
			this.label = v.label;
			this.value = v.value;
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
		rows = new ArrayList<List<Value>>();
		rows.add( new ArrayList<Value>());
		current = 0;
	}

	public void add( String label, long value)
	{
		rows.get(current).add( new Value(label, value));
	}

	public void add( Collection<Result.Value>  values)
	{
		for( Value v: values)
			rows.get(current).add( new Value( v));	
	}
	
	public void next()
	{
		current++;

		rows.add( new ArrayList<Value>());
	}

	public List<Result.Value> getValues( int i)
	{
		return rows.get(i);
	}

	public int getSize()
	{
		return current;
	}

	private List<List<Value>> rows;
	private int current;
}
