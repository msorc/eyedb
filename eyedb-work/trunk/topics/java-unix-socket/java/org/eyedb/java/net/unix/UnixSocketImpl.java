package org.eyedb.java.net.unix;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.SocketImpl;

public class UnixSocketImpl extends SocketImpl {

	UnixSocketImpl()
	{
	}
	
	protected void bind(InetAddress arg0, int arg1) throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected void close() throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected void connect(InetAddress arg0, int arg1) throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected void connect(SocketAddress arg0, int arg1) throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected void connect(String arg0, int arg1) throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected void create(boolean arg0) throws IOException 
	{
		// TODO Auto-generated method stub

	}

	protected InputStream getInputStream() throws IOException 
	{
		// TODO Auto-generated method stub
		return null;
	}

	protected OutputStream getOutputStream() throws IOException 
	{
		// TODO Auto-generated method stub
		return null;
	}

	protected native void listen(int backlog) throws IOException; 

	protected void sendUrgentData(int arg0) throws IOException {
		// TODO Auto-generated method stub

	}

	protected void accept(SocketImpl s) throws IOException
	{
	}

	protected int available() throws IOException
	{
		return 0;
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
