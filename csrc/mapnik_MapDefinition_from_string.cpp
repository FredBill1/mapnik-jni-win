#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    loadMapString
 * Signature: (Ljava/lang/String;ZLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_MapDefinition_loadMapString(JNIEnv* env, jobject mapobject, jstring strj,
                                                               jboolean strict, jstring basepathj) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString str(env, strj);
    JNIString basepath(env, basepathj);
    mapnik::load_map_string(*map, str.get(), (bool)strict, basepath.get());
    TRAILER_VOID;
}