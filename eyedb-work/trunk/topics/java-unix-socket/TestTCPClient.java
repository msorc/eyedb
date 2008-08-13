class TestTCPClient {
    public static void main( String[] args)
    {
	try {
	    Client client = new TCPClient( 4444);
	    client.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

