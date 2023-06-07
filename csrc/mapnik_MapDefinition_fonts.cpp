#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    fonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_fonts(JNIEnv *env, jobject obj) {
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
 * Class:     mapnik_MapDefinition
 * Method:    fontDirectory
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_MapDefinition_fontDirectory(JNIEnv *env, jobject obj);

/*
 * Class:     mapnik_MapDefinition
 * Method:    fontFiles
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_mapnik_MapDefinition_fontFiles(JNIEnv *env, jobject obj);

/*
 * Class:     mapnik_MapDefinition
 * Method:    memoryFonts
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_MapDefinition_memoryFonts(JNIEnv *env, jobject obj);

/*
 * Class:     mapnik_MapDefinition
 * Method:    loadFonts
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_MapDefinition_loadFonts(JNIEnv *env, jobject obj);

/*
 * Class:     mapnik_MapDefinition
 * Method:    registerFonts
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_MapDefinition_registerFonts(JNIEnv *env, jobject obj, jstring pathj,
                                                                   jboolean recurse);
