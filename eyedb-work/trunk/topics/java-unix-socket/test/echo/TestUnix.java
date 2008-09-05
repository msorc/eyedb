class TestUnix {
    class UnixClientThread extends Thread {
	UnixClientThread( String path, int size) throws Exception
	{
	    this.client = new UnixClient(path);
	    this.size = size;
	}

	public void run()
	{
	    try {
		client.run( size);
	    }
	    catch( Exception e) {
		e.printStackTrace();
	    }
	}

	private UnixClient client;
	private int size;
    }

    class UnixServerThread extends Thread {
	UnixServerThread( String path, int size) throws Exception
	{
	    this.server = new UnixServer(path);
	    this.size = size;
	}

	public void run()
	{
	    try {
		server.run( size);
	    }
	    catch( Exception e) {
		e.printStackTrace();
	    }
	}

	private UnixServer server;
	private int size;
    }

    TestUnix()
    {
	try {
	    Thread serverThread = new UnixServerThread( "/var/tmp/java-unix-socket", 1000);
	    serverThread.start();

	    Thread clientThread = new UnixClientThread( "/var/tmp/java-unix-socket", 1000);
	    clientThread.start();

	    serverThread.join();
	    clientThread.join();
	}
	catch( Exception e) {
	    e.printStackTrace();
	}
    }

    public static void main( String[] args)
    {
	new TestUnix();
    }
}
