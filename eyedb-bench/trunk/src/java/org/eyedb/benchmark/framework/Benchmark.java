package org.eyedb.benchmark.framework;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Formatter;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark {

    private static final int defaultColumnWidth = 10;
    private static final char defaultColumnSeparator = ',';

    public Benchmark()
    {
	stopWatch = new StopWatch();

	properties = new Properties();

	columnHeaders = new ArrayList<String>();
	rowHeader = "";
	columnWidth = defaultColumnWidth;
	columnSeparator = defaultColumnSeparator;
	reportLapsDone = false;
	reportColumnHeadersDone = false; 
    }

    public abstract String getName();

    public abstract String getDescription();

    public abstract String getRunDescription();

    public abstract void prepare();

    public abstract void run();

    public abstract void finish();

    public void bench()
    {
	reportBegin();
	prepare();
	stopWatch.start();
	run();
	stopWatch.stop();
	finish();
	reportEnd();
    }

    public void setColumnWidth( int width)
    { 
	columnWidth = width; 
    }

    public void setColumnSeparator( char separator)
    {
	columnSeparator = separator;
    }

    public void addColumnHeader( String header)
    { 
	columnHeaders.add( header); 
    }

    public void setRowHeader( String header)
    { 
	rowHeader = header;
    }

    public void reportBegin()
    {
	System.out.println();
	System.out.println( "Bench: " + getName());
	System.out.println( getDescription());
	System.out.println();
	System.out.println( getRunDescription());
    }

    public void reportLaps()
    {
	Formatter fmt = new Formatter( System.out);

	reportLapsDone = true;

	if (!reportColumnHeadersDone) {
	    reportColumnHeadersDone = true;

	    if (columnHeaders.size() != 0) {

		for ( int i = 0; i < columnHeaders.size(); i++) {
		    fmt.format( "%" + columnWidth + "s", columnHeaders.get(i));

		    if (i != columnHeaders.size() - 1)
			System.out.print( columnSeparator);
		}

		System.out.println();
	    }
	}

	if (rowHeader.length() != 0)
	    fmt.format( "%" + columnWidth + "s%c", rowHeader, columnSeparator);

	for ( int i = 0; i < getStopWatch().getLapCount(); i++) {
	    fmt.format( "%" + columnWidth + "d%c", getStopWatch().getLapTime( i), columnSeparator);
	}

	fmt.format( "%" + columnWidth + "d\n", getStopWatch().getTotalTime());
    }

    public void reportEnd()
    {
	if (!reportLapsDone)
	    reportLaps();
	
	System.out.println();
    }

    public StopWatch getStopWatch()
    {
	return stopWatch;
    }

    protected Properties getProperties()
    {
	return properties;
    }

    public void loadProperties( String filename)
    {
	if (filename == null)
	    return;

	try {
	    properties.load( new FileInputStream( filename));
	}
	catch( IOException e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }

    private StopWatch stopWatch;
    private Properties properties;

    private List<String> columnHeaders;
    private String rowHeader;
    private int columnWidth;
    private char columnSeparator;
    private boolean reportLapsDone;
    private boolean reportColumnHeadersDone;
}
