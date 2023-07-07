#include "geowin_mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    fonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_geowin_mapnik_MapDefinition_fonts(JNIEnv *env, jobject obj) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    auto &mapping = map->get_font_file_mapping();
    jobjectArray arr = env->NewObjectArray(mapping.size(), CLASS_STRING, nullptr);
    int i = 0;
    for (auto &&[k, v] : mapping) {
        JNIObject key(env, env->NewStringUTF(k.c_str()));
        env->SetObjectArrayElement(arr, i++, key.get());
    }
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    fontDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_MapDefinition_fontDirectory(JNIEnv *env, jobject obj) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    auto &font_dir = map->font_directory();
    if (!font_dir) return NULL;
    return env->NewStringUTF(font_dir->c_str());
    TRAILER(NULL);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    fontFiles
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_fontFiles(JNIEnv *env, jobject obj) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    auto &mapping = map->get_font_file_mapping();
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
 * Class:     geowin_mapnik_MapDefinition
 * Method:    memoryFonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_geowin_mapnik_MapDefinition_memoryFonts(JNIEnv *env, jobject obj) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    auto &font_cache = map->get_font_memory_cache();
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
 * Class:     geowin_mapnik_MapDefinition
 * Method:    loadFonts
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_geowin_mapnik_MapDefinition_loadFonts(JNIEnv *env, jobject obj) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    return map->load_fonts();
    TRAILER(false);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    registerFonts
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_geowin_mapnik_MapDefinition_registerFonts(JNIEnv *env, jobject obj, jstring pathj,
                                                                   jboolean recurse) {
    PREAMBLE
    auto map = LOAD_MAP_POINTER(obj);
    JNIString path(env, pathj);
    return map->register_fonts(path.get(), recurse);
    TRAILER(false);
}
