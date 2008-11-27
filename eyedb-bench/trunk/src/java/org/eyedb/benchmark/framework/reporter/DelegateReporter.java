package org.eyedb.benchmark.framework.reporter;

import java.util.ArrayList;
import java.util.Collection;

import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.Reporter;

public class DelegateReporter implements Reporter {

    public DelegateReporter()
    {
	this.reporters = new ArrayList<Reporter>();
    }

    public void report(Benchmark benchmark)
    {
	for ( Reporter r: reporters)
	    r.report(benchmark);
    }

    private Collection<Reporter> reporters;
}
