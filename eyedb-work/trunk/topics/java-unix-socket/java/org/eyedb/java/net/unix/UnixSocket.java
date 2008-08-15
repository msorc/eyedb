package org.eyedb.java.net.unix;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketImpl;
import java.net.SocketException;

public class UnixSocket extends Socket {
	
    private UnixSocketImpl impl;

    public UnixSocket() throws IOException
    {
	impl = new UnixSocketImpl();
	impl.create( true);
    }

    public UnixSocket( UnixSocketAddress socketAddress) throws IOException
    {
	this();
	connect( socketAddress);
    }

    protected SocketImpl getImpl() throws SocketException
    {
	return impl;
    }

    public String toString()
    {
	return  UnixSocket.class.getName() + " [ fd=" + impl.getFD() + " ]";
    }
}
