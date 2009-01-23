class TestUnix {
    public static void main( String[] args)
    {
	String path = args[0];
	int n = 10000;

	try {
	    n = Integer.parseInt( args[1]);
	}
	catch (NumberFormatException e) {
	    System.out.println( e);
	    System.exit(1);
	}

	try {
	    Server server = new UnixServer( path, n);
	    server.start();

	    Client client = new UnixClient( path, n);
	    client.start();

	    server.join();
	    client.join();
	}
	catch( Exception e) {
	    e.printStackTrace();
	}
    }
}
