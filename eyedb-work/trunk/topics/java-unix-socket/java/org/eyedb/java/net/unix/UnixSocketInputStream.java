package org.eyedb.java.net.unix;

import java.io.IOException;
import java.io.InputStream;

class UnixSocketInputStream extends InputStream {

    private UnixSocketImpl impl;

    UnixSocketInputStream( UnixSocketImpl impl)
    {
	this.impl = impl;
    }

    public int read() throws IOException
    {
	byte[] buff = new byte[1];

	int n = read( buff, 0, 1);

	if (n < 0)
	    return -1;

	return (int)buff[0] & 0xff;
    }

    public int read( byte[] buff, int off, int len) throws IOException
    {
	return impl.read( buff, off, len);
    }
}
