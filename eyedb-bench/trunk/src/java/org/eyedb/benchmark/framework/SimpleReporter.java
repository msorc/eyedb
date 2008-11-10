package org.eyedb.benchmark.framework;

import java.io.IOException;
import java.io.PrintStream;
import java.util.Formatter;

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
	out.println( "Bench: " + benchmark.getName());
	out.println( benchmark.getDescription());
	out.println();
	out.println( benchmark.getRunDescription());
	
	reportResult( benchmark.getResult());

	out.println();
    }

    private void reportResult( Result result)
    {
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

