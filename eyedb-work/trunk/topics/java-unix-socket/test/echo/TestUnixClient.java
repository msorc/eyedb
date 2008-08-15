class TestUnixClient {
    public static void main( String[] args)
    {
	try {
	    Client client = new UnixClient( "/var/tmp/java-unix-socket");
	    client.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

