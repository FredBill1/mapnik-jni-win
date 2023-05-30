/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class mapnik_Image */

#ifndef _Included_mapnik_Image
#define _Included_mapnik_Image
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     mapnik_Image
 * Method:    alloc
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Image_alloc__II
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     mapnik_Image
 * Method:    alloc
 * Signature: (Lmapnik/Image;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Image_alloc__Lmapnik_Image_2
  (JNIEnv *, jclass, jobject);

/*
 * Class:     mapnik_Image
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Image_dealloc
  (JNIEnv *, jobject, jlong);

/*
 * Class:     mapnik_Image
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Image_getWidth
  (JNIEnv *, jobject);

/*
 * Class:     mapnik_Image
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Image_getHeight
  (JNIEnv *, jobject);

/*
 * Class:     mapnik_Image
 * Method:    saveToFile
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Image_saveToFile
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     mapnik_Image
 * Method:    saveToMemory
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_mapnik_Image_saveToMemory
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
