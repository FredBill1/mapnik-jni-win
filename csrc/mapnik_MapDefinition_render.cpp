#include "mapnik_MapDefinition.h"
//
#include "globals.hpp"
#include "utils/jni_map_to_mapnik_attributes.hpp"

/*
 * Class:     mapnik_MapDefinition
 * Method:    renderVectorTileImpl
 * Signature: (Lmapnik/VectorTile;DDIIIIDZZIIDLjava/util/Map;Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_MapDefinition_renderVectorTileImpl(
    JNIEnv* env, jobject mapj, jobject tilej, jdouble scale, jdouble scale_denominator, jint offset_x, jint offset_y,
    jint image_scaling, jint image_format, jdouble area_threshold, jboolean strictly_simple,
    jboolean multi_polygon_union, jint fill_type, jint threading_mode, jdouble simplify_distance, jobject variablesj,
    jboolean process_all_rings) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapj);
    auto tile = LOAD_VECTOR_TILE_POINTER(tilej);
    mapnik::attributes variables;
    jni_map_to_mapnik_attributes(env, variablesj, variables);
    mapnik::vector_tile_impl::processor ren(*map, variables);
    ren.set_simplify_distance(simplify_distance);
    ren.set_multi_polygon_union(multi_polygon_union);
    ren.set_fill_type(static_cast<mapnik::vector_tile_impl::polygon_fill_type>(fill_type));
    ren.set_process_all_rings(process_all_rings);
    ren.set_scale_factor(scale);
    ren.set_strictly_simple(strictly_simple);
    ren.set_image_format(image_format_names[image_format]);
    ren.set_scaling_method(static_cast<mapnik::scaling_method_e>(image_scaling));
    ren.set_area_threshold(area_threshold);
    ren.set_threading_mode(threading_modes[threading_mode]);
    ren.update_tile(*tile, scale_denominator, offset_x, offset_y);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_MapDefinition
 * Method:    renderImageImpl
 * Signature: (Lmapnik/Image;IDDIILjava/util/Map;)V
 */
JNIEXPORT void JNICALL Java_mapnik_MapDefinition_renderImageImpl(JNIEnv* env, jobject mapj, jobject imagej,
                                                                 jint buffer_size, jdouble scale,
                                                                 jdouble scale_denominator, jint offset_x,
                                                                 jint offset_y, jobject variablesj) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapj);
    auto image = LOAD_IMAGE_POINTER(imagej);
    mapnik::attributes variables;
    jni_map_to_mapnik_attributes(env, variablesj, variables);
    mapnik::request req(image->width(), image->height(), map->get_current_extent());
    req.set_buffer_size(buffer_size);
    mapnik::agg_renderer<mapnik::image_rgba8> ren(*map, req, variables, *image, scale, offset_x, offset_y);
    ren.apply(scale_denominator);
    TRAILER_VOID;
}
