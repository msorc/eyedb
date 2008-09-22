package org.eyedb.java.net.unix;

import java.io.IOException;
import java.io.OutputStream;

class UnixSocketOutputStream extends OutputStream {

    private UnixSocketImpl impl;

    UnixSocketOutputStream( UnixSocketImpl impl)
    {
	this.impl = impl;
    }

    public void write( int b) throws IOException
    {
	byte[] buff = new byte[1];

	buff[0] = (byte)(b & 0xff);

	write( buff, 0, 1);
    }

    public void write( byte[] buff, int off, int len) throws IOException
    {
	impl.write( buff, off, len);
    }
}
