#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    loadMap
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_MapDefinition_loadMap(JNIEnv* env, jobject mapobject, jstring filenamej,
                                                         jboolean strict) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString filename(env, filenamej);
    mapnik::load_map(*map, filename.get(), (bool)strict);
    TRAILER_VOID;
}
