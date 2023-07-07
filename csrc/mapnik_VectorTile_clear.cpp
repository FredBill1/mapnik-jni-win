#include "geowin_mapnik_VectorTile.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_VectorTile_clear(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->clear();
    TRAILER_VOID;
}
