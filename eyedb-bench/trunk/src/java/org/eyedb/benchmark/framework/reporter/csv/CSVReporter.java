package org.eyedb.benchmark.framework.reporter.csv;

import java.io.FileOutputStream;
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

public class CSVReporter implements Reporter {

    private static final char defaultColumnSeparator = ',';

    public CSVReporter()
    {
	columnSeparator = defaultColumnSeparator;
    }

    public void setColumnSeparator( char separator)
    {
	columnSeparator = separator;
    }

    public void report( Benchmark benchmark)
    {
	String c = benchmark.getProperties().getStringProperty("reporter.csv.column_separator");
	if (c != null)
	    setColumnSeparator( c.charAt(0));
		
	String filename = benchmark.getProperties().getStringProperty("reporter.csv.filename");
	if (filename == null)
	    filename = "/var/tmp/eyedb-benchmark.csv";

	boolean append = benchmark.getProperties().getBooleanProperty("reporter.csv.append", false);

	long size = -1;
	try {
	    FileOutputStream fout = new FileOutputStream( filename, append);
	    out = new PrintStream( fout, true);
	    size = fout.getChannel().size();
	}
	catch( IOException e) {
	    System.err.println( e);
	    return;
	}

	System.err.println( "append " + append + " size " + size);

	if (!append || size == 0) {
	    out.println( "\"Benchmark:\"" + columnSeparator + "\"" + benchmark.getName() + "\"");
	    out.println( "\"" + benchmark.getDescription() + "\"");
	    out.println();
	    reportContext( benchmark.getContext());
	    out.println();
	}
		
	out.println( "\"" + benchmark.getImplementation() + "\"");
	reportResult( benchmark.getResult());
	out.println();

	out.close();
    }

    private void reportContext( Context context)
    {
	for ( Map.Entry<String,String> entry: context.entrySet())
	    out.println( "\"" + entry.getKey() + "\"" + columnSeparator + "\"" + entry.getValue() + "\"");
    }

    private void reportResult( Result result)
    {
	Formatter fmt = new Formatter( out);

	if (result.getSize() >= 1) {
	    for ( String h : result.getHeaders())
		fmt.format( "\"%s\"%c", h, columnSeparator);

	    out.println();
	}
		
	for ( int i = 0; i < result.getSize(); i++) {
	    for ( Long v : result.getValues(i))
		fmt.format( "\"%d\"%c", v.longValue(), columnSeparator);

	    out.println();
	}
    }

    private PrintStream out;
    private char columnSeparator;
}

