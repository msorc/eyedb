package org.eyedb.java.net.unix;

import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;
import java.io.IOException;

public class UnixServerSocket extends ServerSocket {

    private UnixSocketImpl impl;

    public UnixServerSocket() throws IOException
    {
	impl = new UnixSocketImpl();
	impl.create( true);
    }

    public Socket accept () throws IOException
    {
	UnixSocket s = new UnixSocket();

	//	impl.accept (s);

	return s;
    }

    public void bind(SocketAddress endpoint, int backlog) throws IOException 
    {
        if (!(endpoint instanceof UnixSocketAddress))
            throw new IllegalArgumentException("Unsupported address type");
	
	impl.bind( (UnixSocketAddress)endpoint);
	impl.listen( backlog);
    }

    public String toString()
    {
	return  UnixServerSocket.class.getName() + " [ fd=" + impl.getFD() + " ]";
    }
}
