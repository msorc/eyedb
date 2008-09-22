/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_eyedb_java_net_unix_UnixSocketImpl */

#ifndef _Included_org_eyedb_java_net_unix_UnixSocketImpl
#define _Included_org_eyedb_java_net_unix_UnixSocketImpl
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    accept
 * Signature: (Ljava/net/SocketImpl;)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_accept
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    available
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_available
  (JNIEnv *, jobject);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    bind
 * Signature: (Lorg/eyedb/java/net/unix/UnixSocketAddress;)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_bind
  (JNIEnv *, jobject, jobject);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_close
  (JNIEnv *, jobject);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    connect
 * Signature: (Ljava/net/SocketAddress;I)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_connect
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    create
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_create
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    listen
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_listen
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    read
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_read
  (JNIEnv *, jobject, jbyteArray, jint, jint);

/*
 * Class:     org_eyedb_java_net_unix_UnixSocketImpl
 * Method:    write
 * Signature: ([BII)V
 */
JNIEXPORT void JNICALL Java_org_eyedb_java_net_unix_UnixSocketImpl_write
  (JNIEnv *, jobject, jbyteArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
