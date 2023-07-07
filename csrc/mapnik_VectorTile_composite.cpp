#include "geowin_mapnik_VectorTile.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    compositeImpl
 * Signature: ([Lmapnik/VectorTile;DIIDZZIDZ[DDZIII)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_VectorTile_compositeImpl(
    JNIEnv *env, jobject obj, jobjectArray array, jdouble scale_factor, jint offset_x, jint offset_y,
    jdouble area_threshold, jboolean strictly_simple, jboolean multi_polygon_union, jint fill_type,
    jdouble scale_denominator, jboolean reencode, jdoubleArray max_extent, jdouble simplify_distance,
    jboolean process_all_rings, jint image_format, jint scaling_method, jint threading_mode) {
    PREAMBLE;
    if (array == NULL) throw std::runtime_error("input array is null");
    jsize array_len = env->GetArrayLength(array);
    if (array_len == 0) throw std::runtime_error("input array is empty");

    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    mapnik::Map map(tile->tile_size(), tile->tile_size(), "epsg:3857");
    if (max_extent != NULL) {
        if (env->GetArrayLength(max_extent) != 4)
            throw std::invalid_argument("max_extent value must be an array of [minx,miny,maxx,maxy]");
        JNIDoubleArrayElements tmp(env, max_extent);
        mapnik::box2d<double> ext(tmp[0], tmp[1], tmp[2], tmp[3]);
        map.set_maximum_extent(ext);
    }

    std::vector<mapnik::vector_tile_impl::merc_tile_ptr> merc_vtiles;
    for (jsize i = 0; i < array_len; ++i) {
        JNIObject obj(env, env->GetObjectArrayElement(array, i));
        auto tile = LOAD_VECTOR_TILE_POINTER(obj.get());
        // prevent shared_ptr from deleting the tile objects by creating a no-op deleter
        merc_vtiles.emplace_back(tile, [](auto &&) {});
    }

    mapnik::vector_tile_impl::processor ren(map);
    ren.set_fill_type(static_cast<mapnik::vector_tile_impl::polygon_fill_type>(fill_type));
    ren.set_simplify_distance(simplify_distance);
    ren.set_process_all_rings(process_all_rings);
    ren.set_multi_polygon_union(multi_polygon_union);
    ren.set_strictly_simple(strictly_simple);
    ren.set_area_threshold(area_threshold);
    ren.set_scale_factor(scale_factor);
    ren.set_scaling_method(static_cast<mapnik::scaling_method_e>(scaling_method));
    ren.set_image_format(image_format_names[image_format]);

    ren.set_threading_mode(threading_modes[threading_mode]);
    mapnik::vector_tile_impl::composite(*tile, merc_vtiles, map, ren, scale_denominator, offset_x, offset_y, reencode);
    TRAILER_VOID;
}
