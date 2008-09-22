#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "org_eyedb_java_net_unix_UnixSocketImpl.h"

static void
throwException( JNIEnv *env, const char *exceptionName, const char *msg)
{
  jclass exceptionClass;

  exceptionClass = (*env)->FindClass (env, exceptionName);

  (*env)->ThrowNew (env, exceptionClass, msg);
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_accept(JNIEnv *env, jobject this, jobject sockImpl)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;
  int newFd;

  /* Should we check that sockImpl is a UnixSocketImpl? Probably not because the only call to accept
     is done by UnixSocketServer that guarantees to pass a correct sockImpl.
  */

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  newFd = accept( fd, 0, 0);

  if (newFd == -1) {
    throwException( env, "java/io/IOException", strerror( errno));
    return;
  }

  (*env)->SetIntField (env, sockImpl, fdId, (jint)newFd);
}

jint 
Java_org_eyedb_java_net_unix_UnixSocketImpl_available( JNIEnv *env, jobject this)
{
  return (jint)0;
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_bind(JNIEnv *env, jobject this, jobject addr)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;
  jclass socketAddressClass;
  jmethodID getPathId;
  jobject pathStr;
  const char *path;
  struct sockaddr_un sockAddr;

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  /* Get the path from addr argument */
  socketAddressClass = (*env)->GetObjectClass (env, addr);
  getPathId = (*env)->GetMethodID (env, socketAddressClass, "getPath", "()Ljava/lang/String;");
  pathStr = (*env)->CallObjectMethod (env, addr, getPathId);
  path = (*env)->GetStringUTFChars (env, (jstring)pathStr, NULL);

  /* Bind the socket */
  if (strlen (path) > sizeof (sockAddr.sun_path))
    {
      throwException( env, "java/io/IOException", strerror( ENAMETOOLONG));
      return;
    }

  strncpy( sockAddr.sun_path, path, sizeof (sockAddr.sun_path));
  sockAddr.sun_path[sizeof (sockAddr.sun_path) - 1] = '\0';
  sockAddr.sun_family = AF_UNIX;
  if (bind (fd, (struct sockaddr *) &sockAddr, SUN_LEN( &sockAddr)))  {
    throwException( env, "java/io/IOException", strerror(errno));
    return;
  }

  (*env)->ReleaseStringUTFChars (env, (jstring)pathStr, path);
}

void
Java_org_eyedb_java_net_unix_UnixSocketImpl_close( JNIEnv *env, jobject this)
{
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_connect(JNIEnv *env, jobject this, jobject addr, jint timeout)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;
  jclass socketAddressClass;
  jmethodID getPathId;
  jobject pathStr;
  const char *path;
  struct sockaddr_un sockAddr;

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  /* Get the path from addr argument */
  socketAddressClass = (*env)->GetObjectClass (env, addr);
  getPathId = (*env)->GetMethodID (env, socketAddressClass, "getPath", "()Ljava/lang/String;");
  pathStr = (*env)->CallObjectMethod (env, addr, getPathId);
  path = (*env)->GetStringUTFChars (env, (jstring)pathStr, NULL);

  /* Connect the socket */
  if (strlen (path) > sizeof (sockAddr.sun_path))
    {
      throwException( env, "java/io/IOException", strerror( ENAMETOOLONG));
      return;
    }

  strncpy (sockAddr.sun_path, path, sizeof (sockAddr.sun_path));
  sockAddr.sun_path[sizeof (sockAddr.sun_path) - 1] = '\0';
  sockAddr.sun_family = AF_UNIX;

  if (connect (fd, (struct sockaddr *) &sockAddr, SUN_LEN(&sockAddr)) < 0) {
    throwException( env, "java/io/IOException", strerror(errno));
    return;
  }

  (*env)->ReleaseStringUTFChars (env, (jstring)pathStr, path);
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_create( JNIEnv *env, jobject this, jboolean stream)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;

  if (!stream) {
    throwException( env, "java/lang/IllegalArgumentException", "Datagram Unix socket not implemented yet");
    return;
  }

  /* create the socket */
  fd = socket( PF_UNIX, SOCK_STREAM, 0);
  if (fd == -1) {
    throwException( env, "java/io/IOException", strerror( errno));
    return;
  }

  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  (*env)->SetIntField (env, this, fdId, (jint)fd);
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_listen( JNIEnv *env, jobject this, jint backlog)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  /* Listen on the socket */
  if (listen( fd, (int)backlog)) {
    throwException( env, "java/io/IOException", strerror( errno));
    return;
  }
}

jint 
Java_org_eyedb_java_net_unix_UnixSocketImpl_read( JNIEnv *env, jobject this, jbyteArray buff, jint off, jint len)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;
  jbyte *p;
  int ret;

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  /* Read the bytes using read() */
  p = (*env)->GetByteArrayElements( env, buff, NULL);

  ret = read( fd, (void *)(p+off), (int)len);
  if (ret < 0)
    {
      throwException( env, "java/io/IOException", strerror( errno));
    }

  (*env)->ReleaseByteArrayElements( env, buff, p, JNI_ABORT);

  return ret;
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_write( JNIEnv *env, jobject this, jbyteArray buff, jint off, jint len)
{
  int fd;
  jfieldID fdId;
  jclass socketImplClass;
  jbyte *p;

  /* Get the file descriptor */
  socketImplClass = (*env)->GetObjectClass (env, this);
  fdId = (*env)->GetFieldID (env, socketImplClass, "fd", "I");
  fd = (*env)->GetIntField (env, this, fdId);

  /* Write the bytes using write() */
  p = (*env)->GetByteArrayElements( env, buff, NULL);

  if (write( fd, (void *)(p+off), (int)len) < 0)
    {
      throwException( env, "java/io/IOException", strerror( errno));
    }

  (*env)->ReleaseByteArrayElements( env, buff, p, JNI_ABORT);
}
