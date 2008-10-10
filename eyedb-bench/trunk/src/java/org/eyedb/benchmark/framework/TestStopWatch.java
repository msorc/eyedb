package org.eyedb.benchmark.framework;

class TestStopWatch {

    void sleep( int seconds)
    {
	try {
	    Thread.sleep( seconds * 1000);
	}
	catch (InterruptedException e) {
	    e.printStackTrace();
	}
    }

    int activeWait( int n)
    {
	int s = 0;
	for (int i = 0; i < n; i++)
	    s += i * i;
	
	return s;
    }

    int test( int n)
    {
	StopWatch w = new StopWatch();
	int r = 0;

	w.start();

	sleep( 1);
	w.lap("ONE");

	for (int i = 0; i < n; i++)
	    r = activeWait(i);
	w.lap("TWO");

	for (int i = 0; i < 2*n; i++)
	    r = activeWait(i);
	w.lap("THREE");

	w.stop();

	System.out.println( "Total " + w.getTotalTime());

	for ( int i = 0; i < w.getLapCount(); i++)
	    System.out.println( "lap[" + i + "] " + w.getLapName(i) + " " + w.getLapTime( i));

	return r;
    }

    public static void main( String[] args)
    {
	new TestStopWatch().test( 20000);
    }
}