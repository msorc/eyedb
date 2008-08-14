package org.eyedb.java.net.unix;

import java.io.IOException;
import java.net.Socket;

public class UnixSocket extends Socket {
	
	public UnixSocket() throws IOException
	{
		super( new UnixSocketImpl());
	}
}
