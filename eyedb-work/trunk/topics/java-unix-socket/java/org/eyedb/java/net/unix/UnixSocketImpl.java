package org.eyedb.java.net.unix;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.SocketImpl;

public class UnixSocketImpl extends SocketImpl {

    static
    {
	try {
	    System.loadLibrary ("eyedbunixsocket");
	}
	catch (Exception x) {
	    x.printStackTrace ();
	}
    }
    
    private int fd;
    private InputStream inputStream;
    private OutputStream outputStream;

    UnixSocketImpl()
    {
	fd = -1;
    }
	
    protected int getFD()
    {
	return fd;
    }

    protected native void accept(SocketImpl s) throws IOException;

    protected native int available() throws IOException;

    protected void bind(InetAddress arg0, int port) throws IOException 
    {
	throw new SocketException ("unsupported method");
    }

    protected native void bind(UnixSocketAddress addr) throws IOException;

    protected native void close() throws IOException;

    protected void connect(InetAddress addr, int port) throws IOException 
    {
	throw new SocketException ("unsupported method");
    }

    protected void connect(String host, int port) throws IOException 
    {
	throw new SocketException ("unsupported method");
    }

    protected native void connect(SocketAddress addr, int timeout) throws IOException;

    protected native void create(boolean stream) throws IOException;
	
    protected InputStream getInputStream() throws IOException 
    {
	if (inputStream == null)
	    inputStream = new UnixSocketInputStream( this);

	return inputStream;
    }

    protected OutputStream getOutputStream() throws IOException 
    {
	if (outputStream == null)
	    outputStream = new UnixSocketOutputStream( this);

	return outputStream;
    }

    protected native void listen(int backlog) throws IOException; 

    protected native int read( byte[] buff, int off, int len) throws IOException;

    protected native void write( byte[] buff, int off, int len) throws IOException;

    protected void sendUrgentData(int arg0) throws IOException
    {
	throw new SocketException ("unsupported method");
    }

    public Object getOption(int arg0) throws SocketException 
    {
	throw new SocketException ("unsupported method");
    }

    public void setOption(int arg0, Object arg1) throws SocketException 
    {
	throw new SocketException ("unsupported method");
    }
}
