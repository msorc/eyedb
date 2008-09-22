noinst_JAVA= \
Client.java \
Server.java \
TCPClient.java \
TCPServer.java \
TestTCPClient.java \
TestTCPServer.java \
TestUnix.java \
UnixClient.java \
UnixServer.java 

JAVAROOT=$(top_builddir)/topics/java-unix-socket/build
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/topics/java-unix-socket/test/echo:$(top_builddir)/topics/java-unix-socket/build

run:
	rm -rf /var/tmp/java-unix-socket
	$(JAVA) -Djava.library.path=$(top_builddir)/topics/java-unix-socket/native/.libs -cp $(JAVAROOT):$(top_builddir)/topics/java-unix-socket/build TestUnix /var/tmp/java-unix-socket
