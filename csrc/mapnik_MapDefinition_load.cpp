#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    loadImpl
 * Signature: (Ljava/lang/String;ZLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_MapDefinition_loadImpl(JNIEnv* env, jobject obj, jstring stylesheet, jboolean strict,
                                                          jstring base) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    JNIString filename(env, stylesheet);
    JNIString basepath(env, base);
    mapnik::load_map(*map, filename.get(), strict, basepath.get());
    TRAILER_VOID;
}