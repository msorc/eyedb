noinst_JAVA= \
Client.java \
Server.java \
TCPClient.java \
TCPServer.java \
TestTCP.java \
TestUnix.java \
UnixClient.java \
UnixServer.java 

N=10000

JAVAROOT=$(top_builddir)/topics/java-unix-socket/build
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/topics/java-unix-socket/test/echo:$(top_builddir)/topics/java-unix-socket/build

run-unix:
	rm -rf /var/tmp/java-unix-socket
	$(JAVA) -Djava.library.path=$(top_builddir)/topics/java-unix-socket/native/.libs -cp $(JAVAROOT):$(top_builddir)/topics/java-unix-socket/build TestUnix /var/tmp/java-unix-socket $(N)

run-tcp:
	$(JAVA) -cp $(JAVAROOT):$(top_builddir)/topics/java-unix-socket/build TestTCP 4444 $(N)
