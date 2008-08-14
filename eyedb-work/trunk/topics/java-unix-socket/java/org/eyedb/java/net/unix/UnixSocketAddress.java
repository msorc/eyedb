package org.eyedb.java.net.unix;

import java.net.SocketAddress;

public class UnixSocketAddress extends SocketAddress {

    public UnixSocketAddress( String path)
    {
	this.path = path;
    }

    public String getPath()
    {
	return path;
    }

    private String path;
}
