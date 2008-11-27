package org.eyedb.benchmark.framework.reporter;

import java.util.ArrayList;
import java.util.Collection;

import org.eyedb.benchmark.framework.Reporter;
import org.eyedb.benchmark.framework.reporter.simple.SimpleReporter;

class DefaultReporterFactory extends ReporterFactory {
	protected DefaultReporterFactory()
	{
	}
	
	public Reporter newReporter()
	{
	    Collection<Reporter> reporters = new ArrayList<Reporter>();
	    reporters.add( new SimpleReporter());
	    
	    return new DelegateReporter(reporters);
	}
}

public abstract class ReporterFactory {

    protected ReporterFactory()
    {
    }
    
    public static ReporterFactory newInstance()
    {
	return new DefaultReporterFactory();
    }
    
    public abstract Reporter newReporter();
}
