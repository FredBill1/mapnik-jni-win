#include "mapnik_Mapnik.h"

#include <mapnik/mapped_memory_cache.hpp>
#include <mapnik/marker_cache.hpp>

#include "globals.hpp"

/// -- Mapnik class
/*
 * Class:     mapnik_Mapnik
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_nativeInit(JNIEnv *env, jclass c) {
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
JNIEXPORT void JNICALL Java_mapnik_Mapnik_initLogger(JNIEnv *env, jclass c, jstring sj, jint level) {
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
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledFontsDir(JNIEnv *env, jclass) {
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
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledInputPluginsDir(JNIEnv *env, jclass) {
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
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_isThreadSafe(JNIEnv *env, jclass c) {
#ifdef MAPNIK_THREADSAFE
    return 1;
#else
    return 0;
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    registerDatasource
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_registerDatasource(JNIEnv *env, jclass, jstring pathj) {
    PREAMBLE;
    JNIString path(env, pathj);
    auto &instance = mapnik::datasource_cache::instance();
    size_t cnt_before = instance.plugin_names().size();
    if (!instance.register_datasource(path.get())) return false;
    return cnt_before < instance.plugin_names().size();
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    registerDatasources
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_registerDatasources(JNIEnv *env, jclass, jstring pathj,
                                                                  jboolean recurse) {
    PREAMBLE;
    JNIString path(env, pathj);
    auto &instance = mapnik::datasource_cache::instance();
    size_t cnt_before = instance.plugin_names().size();
    if (!instance.register_datasources(path.get(), recurse)) return false;
    return cnt_before < instance.plugin_names().size();
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    datasourceRegistered
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_datasourceRegistered(JNIEnv *env, jclass, jstring namej) {
    PREAMBLE;
    JNIString name(env, namej);
    return mapnik::datasource_cache::instance().plugin_registered(name.get());
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    datasourceDirectories
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_datasourceDirectories(JNIEnv *env, jclass) {
    PREAMBLE;
    return env->NewStringUTF(mapnik::datasource_cache::instance().plugin_directories().c_str());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    datasources
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_Mapnik_datasources(JNIEnv *env, jclass) {
    PREAMBLE;
    auto plugin_names = mapnik::datasource_cache::instance().plugin_names();
    jobjectArray arr = env->NewObjectArray(plugin_names.size(), CLASS_STRING, NULL);
    jsize i = 0;
    for (auto &&name : plugin_names) {
        JNIObject namej(env, env->NewStringUTF(name.c_str()));
        env->SetObjectArrayElement(arr, i++, namej.get());
    }
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    registerFont
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_registerFont(JNIEnv *env, jclass, jstring pathj) {
    PREAMBLE;
    JNIString path(env, pathj);
    return mapnik::freetype_engine::register_font(path.get());
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    registerFonts
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_registerFonts(JNIEnv *env, jclass, jstring pathj, jboolean recurse) {
    PREAMBLE;
    JNIString path(env, pathj);
    return mapnik::freetype_engine::register_fonts(path.get(), recurse);
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    isFontFile
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_isFontFile(JNIEnv *env, jclass, jstring pathj) {
    PREAMBLE;
    JNIString path(env, pathj);
    return mapnik::freetype_engine::is_font_file(path.get());
    TRAILER(false);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    fonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_Mapnik_fonts(JNIEnv *env, jclass) {
    PREAMBLE;
    auto names = mapnik::freetype_engine::face_names();
    jobjectArray arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    jsize i = 0;
    for (auto &&name : names) {
        JNIObject namej(env, env->NewStringUTF(name.c_str()));
        env->SetObjectArrayElement(arr, i++, namej.get());
    }
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    fontFiles
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Mapnik_fontFiles(JNIEnv *env, jclass) {
    PREAMBLE;
    auto &mapping = mapnik::freetype_engine::get_mapping();
    jobject mapj = env->NewObject(CLASS_HASHMAP, CTOR_HASHMAP);
    for (auto &&[k, v] : mapping) {
        JNIObject key(env, env->NewStringUTF(k.c_str()));
        JNIObject value(env, env->NewStringUTF(v.second.c_str()));
        JNIObjectAllowNull(env, env->CallObjectMethod(mapj, METHOD_HASHMAP_PUT, key.get(), value.get()));
    }
    return mapj;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    memoryFonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_Mapnik_memoryFonts(JNIEnv *env, jclass) {
    PREAMBLE;
    auto &font_cache = mapnik::freetype_engine::get_cache();
    jobjectArray arr = env->NewObjectArray(font_cache.size(), CLASS_STRING, nullptr);
    int i = 0;
    for (auto &&[k, v] : font_cache) {
        JNIObject key(env, env->NewStringUTF(k.c_str()));
        env->SetObjectArrayElement(arr, i++, key.get());
    }
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Mapnik
 * Method:    clearCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_clearCache(JNIEnv *env, jclass) {
    PREAMBLE;
#if defined(MAPNIK_MEMORY_MAPPED_FILE)
    mapnik::marker_cache::instance().clear();
    mapnik::mapped_memory_cache::instance().clear();
#endif
    TRAILER_VOID;
}