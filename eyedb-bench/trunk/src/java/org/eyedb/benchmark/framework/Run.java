package org.eyedb.benchmark.framework;

import org.eyedb.benchmark.framework.reporter.ReporterFactory;


/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Run {

    public Run( String[] args)
    {
	try {
	    String benchmarkClassName = args[0];

	    Class benchmarkClass = Class.forName( benchmarkClassName);
	    Benchmark benchmark = (Benchmark)benchmarkClass.newInstance();

	    String propertiesFileName = (args.length >= 2) ? args[1] : null;

	    benchmark.getProperties().load( propertiesFileName);
	    benchmark.getProperties().load( args);

	    benchmark.bench();

	    Reporter reporter = ReporterFactory.newInstance().newReporter();
	    reporter.report( benchmark);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit( 1);
	}
    }

    public static void main( String[] args)
    {
	new Run( args);
    }
}
