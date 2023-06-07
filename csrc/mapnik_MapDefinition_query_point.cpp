#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DDLjava/lang/String;)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryMapPoint__DDLjava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                              jdouble x, jdouble y,
                                                                                              jstring layer_namej);

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DDI)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryMapPoint__DDI(JNIEnv *env, jobject obj, jdouble x,
                                                                            jdouble y, jint layer_idx);

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DD)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryMapPoint__DD(JNIEnv *env, jobject obj, jdouble x,
                                                                           jdouble y);

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DDLjava/lang/String;)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryPoint__DDLjava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                           jdouble x, jdouble y,
                                                                                           jstring layer_namej);

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DDI)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryPoint__DDI(JNIEnv *env, jobject obj, jdouble x, jdouble y,
                                                                         jint layer_idx);

/*
 * Class:     mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DD)[Lmapnik/FeatureSet;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_queryPoint__DD(JNIEnv *env, jobject obj, jdouble x, jdouble y);
