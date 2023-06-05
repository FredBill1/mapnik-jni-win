#include "mapnik_Mapnik.h"

#include "globals.hpp"

/// -- Mapnik class
/*
 * Class:     mapnik_Mapnik
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_nativeInit(JNIEnv* env, jclass c) {
    PREAMBLE;
    if (initialized) return;

    mapnik_jni_init_ids(env);

    mapnik::setup();
    initialized = true;
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Mapnik
 * Method:    initLogger
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_initLogger(JNIEnv* env, jclass c, jstring sj, jint level) {
    PREAMBLE;
    JNIString path(env, sj);
#if MAPNIK_VERSION >= 200200
    mapnik::logger::instance().use_file(path.get());
    mapnik::logger::instance().set_severity(static_cast<mapnik::logger::severity_type>(level));
#else
    mapnik::logger::instance()->use_file(path.get());
    mapnik::logger::instance()->set_severity(static_cast<mapnik::logger::severity_type>(level));
#endif
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledFontsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledFontsDir(JNIEnv* env, jclass) {
#ifndef MAPNIK_FONTS_DIR
    return 0;
#else
    return env->NewStringUTF(MAPNIK_FONTS_DIR);
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledInputPluginsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledInputPluginsDir(JNIEnv* env, jclass) {
#ifndef MAPNIK_INPUT_PLUGINS_DIR
    return 0;
#else
    return env->NewStringUTF(MAPNIK_INPUT_PLUGINS_DIR);
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    isThreadSafe
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_isThreadSafe(JNIEnv* env, jclass c) {
#ifdef MAPNIK_THREADSAFE
    return 1;
#else
    return 0;
#endif
}
