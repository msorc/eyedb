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

    public SimpleReporter( PrintStream out)
    {
	this.out = out;

	columnWidth = defaultColumnWidth;
	columnSeparator = defaultColumnSeparator;
    }

    public SimpleReporter( String filename) throws IOException
    {
	this( new PrintStream( filename));
    }

    public SimpleReporter()
    {
	this( System.out);
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
	out.println();
	out.println( "Benchmark: " + benchmark.getName());
	out.println( "Description: " + benchmark.getDescription());
	out.println( "Implementation: " + benchmark.getImplementation());
	
	reportContext( benchmark.getContext());

	reportResult( benchmark.getResult());

	out.println();
    }

    private void reportContext( Context context)
    {
	out.println();
	out.println( "Context: ");

	for ( Map.Entry<String,String> entry: context.entrySet())
	    out.println( "  " + entry.getKey() + " : " + entry.getValue());
    }

    private void reportResult( Result result)
    {
	out.println();
	out.println( "Result: ");

	Formatter fmt = new Formatter( out);

	for (String h : result.getHeaders()) 
 	    fmt.format( "%" + columnWidth + "s%c", h, columnSeparator);

	out.println();

	for ( int i = 0; i < result.getSize(); i++) {
	    for ( Long l : result.getValues(i)) {
		fmt.format( "%" + columnWidth + "d%c", l.longValue(), columnSeparator);
	    }

	    out.println();
	}
    }

    private PrintStream out;
    private int columnWidth;
    private char columnSeparator;
}

