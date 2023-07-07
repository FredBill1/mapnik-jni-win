#include "geowin_mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    fromStringImpl
 * Signature: (Ljava/lang/String;ZLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_fromStringImpl(JNIEnv* env, jobject obj, jstring stylesheet,
                                                                jboolean strict, jstring base) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    JNIString xml(env, stylesheet);
    JNIString basepath(env, base);
    mapnik::load_map_string(*map, xml.get(), strict, basepath.get());
    TRAILER_VOID;
}