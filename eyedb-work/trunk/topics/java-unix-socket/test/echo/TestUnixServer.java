class TestUnixServer {
    public static void main( String[] args)
    {
	try {
	    UnixServer server = new UnixServer( "/var/tmp/classpath-socket");
	    server.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

