package org.eyedb.java.net.unix;

import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

public class UnixServerSocket extends ServerSocket {

	// TODO
	// - create implementation
	// - methods: bind, accept, 

	public UnixServerSocket() throws IOException
	{
		super();
	}

	public Socket accept () throws IOException
	{
		UnixSocket s = new UnixSocket ();

		implAccept (s);

		return s;
	}
}
