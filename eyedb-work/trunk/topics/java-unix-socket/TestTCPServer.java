class TestTCPServer {
    public static void main( String[] args)
    {
	try {
	    Server server = new TCPServer( 4444);
	    server.run( 1000);
	}
	catch( Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}

