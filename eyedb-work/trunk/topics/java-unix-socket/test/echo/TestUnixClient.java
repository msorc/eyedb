class TestUnixClient {
    public static void main( String[] args)
    {
	try {
	    Client client = new UnixClient( "/var/tmp/classpath-socket");
	    client.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

