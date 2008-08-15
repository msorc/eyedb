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
  sockAddr.sun_path[sizeof (sockAddr.sun_path)] = '\0';
  sockAddr.sun_family = AF_LOCAL;
  if (bind (fd, (struct sockaddr *) &sockAddr, SUN_LEN (&sockAddr)))  {
    throwException( env, "java/io/IOException", strerror(errno));
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
