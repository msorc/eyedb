#include "org_eyedb_java_net_unix_UnixSocketImpl.h"

jint 
Java_org_eyedb_java_net_unix_UnixSocketImpl_available( JNIEnv *env, jobject this)
{
  return (jint)0;
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_bind(JNIEnv *env, jobject this, jobject addr, jint backlog)
{
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
}

void 
Java_org_eyedb_java_net_unix_UnixSocketImpl_listen( JNIEnv *env, jobject this, jint backlog)
{
}
