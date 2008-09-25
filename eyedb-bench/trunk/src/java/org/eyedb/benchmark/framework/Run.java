package org.eyedb.benchmark.framework;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Run {

    public Run( String benchmarkClassName, String propertiesFileName)
    {
	try {
	    Class benchmarkClass = Class.forName( benchmarkClassName);
	    Benchmark benchmark = (Benchmark)benchmarkClass.newInstance();

	    benchmark.loadProperties( propertiesFileName);

	    benchmark.doIt();
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
