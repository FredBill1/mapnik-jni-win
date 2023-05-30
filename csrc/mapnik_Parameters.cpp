#include "mapnik_Parameters.h"

#include "globals.hpp"
//// --- Parameters

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeInt
 * Signature: (JLjava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeInt(JNIEnv *env, jclass c, jlong ptr, jstring namej,
                                                           jint value) {
    PREAMBLE;
    refjavastring name(env, namej);
    mapnik::parameters *params = (mapnik::parameters *)(ptr);
    (*params)[name.stringz] = static_cast<mapnik::value_integer>(value);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeString
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeString(JNIEnv *env, jclass c, jlong ptr, jstring namej,
                                                              jstring valuej) {
    PREAMBLE;
    refjavastring name(env, namej);
    refjavastring value(env, valuej);
    mapnik::parameters *params = (mapnik::parameters *)(ptr);
    (*params)[name.stringz] = value.stringz;
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeDouble
 * Signature: (JLjava/lang/String;D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeDouble(JNIEnv *env, jclass c, jlong ptr, jstring namej,
                                                              jdouble value) {
    PREAMBLE;
    refjavastring name(env, namej);
    mapnik::parameters *params = (mapnik::parameters *)(ptr);
    (*params)[name.stringz] = (double)value;
    TRAILER_VOID;
}
