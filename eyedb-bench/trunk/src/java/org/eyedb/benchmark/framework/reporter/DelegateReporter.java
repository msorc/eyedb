package org.eyedb.benchmark.framework.reporter;

import java.util.ArrayList;
import java.util.Collection;

import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.Reporter;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

class DelegateReporter implements Reporter {

    DelegateReporter()
    {
	this.reporters = new ArrayList<Reporter>();
    }
    
    DelegateReporter( Collection<Reporter> reporters)
    {
	this.reporters = new ArrayList<Reporter>( reporters);
    }

    public void report(Benchmark benchmark)
    {
	for ( Reporter r: reporters)
	    r.report(benchmark);
    }

    private Collection<Reporter> reporters;
}
