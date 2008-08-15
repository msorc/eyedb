nativelibdir=$(pkglibdir)/jni
nativelib_LTLIBRARIES=libeyedbunixsocket.la

libeyedbunixsocket_la_SOURCES= org_eyedb_java_net_unix_UnixSocketImpl.c

INCLUDES= -I@JNI_INCLUDE_PATH@ -I@JNI_INCLUDE_PATH@/linux

BUILT_SOURCES=org_eyedb_java_net_unix_UnixSocketImpl.h

org_eyedb_java_net_unix_UnixSocketImpl.h: $(top_builddir)/topics/java-unix-socket/build/org/eyedb/java/net/unix/UnixSocketImpl.class
	javah -classpath $(top_builddir)/topics/java-unix-socket/build org.eyedb.java.net.unix.UnixSocketImpl
