class TestLocalServer {
    public static void main( String[] args)
    {
	try {
	    LocalServer server = new LocalServer( "/var/tmp/classpath-socket");
	    server.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

