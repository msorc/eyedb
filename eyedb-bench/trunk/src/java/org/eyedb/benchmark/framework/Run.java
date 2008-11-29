package org.eyedb.benchmark.framework;

import org.eyedb.benchmark.framework.reporter.ReporterFactory;


/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Run {

    public Run( String benchmarkClassName, String propertiesFileName)
    {
	try {
	    Class benchmarkClass = Class.forName( benchmarkClassName);
	    Benchmark benchmark = (Benchmark)benchmarkClass.newInstance();

	    benchmark.getProperties().load( propertiesFileName);

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
	String propertiesFileName = (args.length >= 2) ? args[1] : null;

	new Run( args[0], propertiesFileName);
    }
}
