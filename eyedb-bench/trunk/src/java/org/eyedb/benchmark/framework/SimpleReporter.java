package org.eyedb.benchmark.framework;

import java.util.Formatter;

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
	System.out.println();
	System.out.println( "Bench: " + benchmark.getName());
	System.out.println( benchmark.getDescription());
	System.out.println();
	System.out.println( benchmark.getRunDescription());
	
	reportResult( benchmark.getResult());

	System.out.println();
    }

    private void reportResult( Result result)
    {
 	Formatter fmt = new Formatter( System.out);

	for (String h : result.getHeaders()) 
 	    fmt.format( "%" + columnWidth + "s%c", h, columnSeparator);

	System.out.println();

	for ( int i = 0; i < result.getSize(); i++) {
	    for ( Long l : result.getValues(i)) {
		fmt.format( "%" + columnWidth + "d%c", l.longValue(), columnSeparator);
	    }

	    System.out.println();
	}
    }

    private int columnWidth;
    private char columnSeparator;
}

