#include "mapnik_VectorTile.h"

#include "globals.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    alloc
 * Signature: (JJJII)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_VectorTile_alloc(JNIEnv *env, jclass, jlong x, jlong y, jlong z, jint tileSize,
                                                     jint bufferSize) {
    PREAMBLE;
    auto t = new mapnik::vector_tile_impl::merc_tile(x, y, z, tileSize, bufferSize);
    return FROM_POINTER(t);
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete static_cast<mapnik::vector_tile_impl::merc_tile *>(TO_POINTER(ptr));
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileX
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileX(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->x();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileX
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileX(JNIEnv *env, jobject obj, jint x) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->x(x);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileY
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileY(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->y();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileY
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileY(JNIEnv *env, jobject obj, jint y) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->y(y);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileZ
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileZ(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->z();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileZ
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileZ(JNIEnv *env, jobject obj, jint z) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->z(z);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileSize(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->tile_size();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileSize(JNIEnv *env, jobject obj, jint tile_size) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->tile_size(tile_size);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getBufferSize(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->buffer_size();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setBufferSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setBufferSize(JNIEnv *env, jobject obj, jint buffer_size) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->buffer_size(buffer_size);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    addGeoJSONImpl
 * Signature: (Ljava/lang/String;Ljava/lang/String;DDZZIZ)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_addGeoJSONImpl(JNIEnv *env, jobject obj, jstring geojsonj, jstring namej,
                                                             jdouble area_threshold, jdouble simplify_distance,
                                                             jboolean strictly_simple, jboolean multi_polygon_union,
                                                             jint fill_type, jboolean process_all_rings) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    JNIString geojson_string(env, geojsonj);
    JNIString geojson_name(env, namej);
    mapnik::Map map(tile->tile_size(), tile->tile_size(), "+init=epsg:3857");
    mapnik::parameters p;
    p["type"] = "geojson";
    p["inline"] = geojson_string.get();
    mapnik::layer lyr(geojson_name.get(), "+init=epsg:4326");
    lyr.set_datasource(mapnik::datasource_cache::instance().create(p));
    map.add_layer(lyr);

    mapnik::vector_tile_impl::processor ren(map);
    ren.set_area_threshold(area_threshold);
    ren.set_simplify_distance(simplify_distance);
    ren.set_strictly_simple(strictly_simple);
    ren.set_multi_polygon_union(multi_polygon_union);
    ren.set_fill_type(static_cast<mapnik::vector_tile_impl::polygon_fill_type>(fill_type));
    ren.set_process_all_rings(process_all_rings);
    ren.update_tile(*tile);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    bufferedExtent
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_VectorTile_bufferedExtent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ext = tile->get_buffered_extent();
    jdoubleArray arr = env->NewDoubleArray(4);
    jdouble tmp[4]{ext.minx(), ext.miny(), ext.maxx(), ext.maxy()};
    env->SetDoubleArrayRegion(arr, 0, 4, tmp);
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    empty
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_VectorTile_empty(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->is_empty();
    TRAILER(false);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    emptyLayers
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_emptyLayers(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_empty_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    extent
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_VectorTile_extent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ext = tile->extent();
    jdoubleArray arr = env->NewDoubleArray(4);
    jdouble tmp[4]{ext.minx(), ext.miny(), ext.maxx(), ext.maxy()};
    env->SetDoubleArrayRegion(arr, 0, 4, tmp);
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    layerImpl
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_VectorTile_layerImpl(JNIEnv *env, jobject obj, jstring layer_namej) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::string layer_name = JNIString(env, layer_namej).get();
    if (!tile->has_layer(layer_name)) throw std::exception("layer does not exist in vector tile");

    auto v = new mapnik::vector_tile_impl::merc_tile(tile->x(), tile->y(), tile->z(), tile->tile_size(),
                                                     tile->buffer_size());
    protozero::pbf_reader tile_message(tile->get_reader());
    while (tile_message.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        auto data_pair = tile_message.get_data();
        protozero::pbf_reader layer_message(data_pair);
        if (!layer_message.next(mapnik::vector_tile_impl::Layer_Encoding::NAME)) continue;
        std::string name = layer_message.get_string();
        if (layer_name == name) {
            v->append_layer_buffer(data_pair.first, data_pair.second, layer_name);
            break;
        }
    }
    return FROM_POINTER(v);
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    names
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_names(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    painted
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_VectorTile_painted(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->is_painted();
    TRAILER(false);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    paintedLayers
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_paintedLayers(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_painted_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}
