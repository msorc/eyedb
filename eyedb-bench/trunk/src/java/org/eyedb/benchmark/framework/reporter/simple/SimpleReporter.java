package org.eyedb.benchmark.framework.reporter.simple;

import java.io.IOException;
import java.io.PrintStream;
import java.util.Formatter;
import java.util.Map;
import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.Context;
import org.eyedb.benchmark.framework.Reporter;
import org.eyedb.benchmark.framework.Result;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class SimpleReporter implements Reporter {

    private static final int defaultColumnWidth = 10;
    private static final char defaultColumnSeparator = ',';

    public SimpleReporter()
    {
	columnWidth = defaultColumnWidth;
	columnSeparator = defaultColumnSeparator;
    }

    public void setColumnWidth( int width)
    { 
	columnWidth = width; 
    }

    public void setColumnSeparator( char separator)
    {
	columnSeparator = separator;
    }

    public void report( Benchmark benchmark)
    {
	out =  System.out;
	    
	int w = benchmark.getProperties().getIntProperty("reporter.simple.column_width",-1);
	if (w > 0)
	    setColumnWidth( w);

	String c = null;
	c = benchmark.getProperties().getStringProperty("reporter.simple.column_separator", c);
	if (c != null)
	    setColumnSeparator( c.charAt(0));
		
	out.println();
	out.println( "Benchmark: " + benchmark.getName());
	out.println( "Description: " + benchmark.getDescription());
	reportContext( benchmark.getContext());
		
	out.println();
	out.println( "Implementation: " + benchmark.getImplementation());

	reportResult( benchmark.getResult());

	out.println();
    }

    private void reportContext( Context context)
    {
	out.println();

	for ( Map.Entry<String,String> entry: context.entrySet())
	    out.println( "  " + entry.getKey() + " : " + entry.getValue());
    }

    private void reportResult( Result result)
    {
	out.println();
	out.println( "Result: ");

	Formatter fmt = new Formatter( out);

	if (result.getSize() >= 1) {
	    for ( String h : result.getHeaders())
		fmt.format( "%" + columnWidth + "s%c", h, columnSeparator);

	    out.println();
	}
		
	for ( int i = 0; i < result.getSize(); i++) {
	    for ( Long v : result.getValues(i))
		fmt.format( "%" + columnWidth + "d%c", v.longValue(), columnSeparator);

	    out.println();
	}
    }

    private PrintStream out;
    private int columnWidth;
    private char columnSeparator;
}

