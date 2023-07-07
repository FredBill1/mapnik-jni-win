#include "geowin_mapnik_VectorTile.h"
//
#include "globals.hpp"
#include "utils/jni_map_to_mapnik_attributes.hpp"

template <typename Renderer>
static void process_layers(Renderer &ren, mapnik::request const &m_req, mapnik::projection const &map_proj,
                           std::vector<mapnik::layer> const &layers, double scale_denom, std::string const &map_srs,
                           mapnik::vector_tile_impl::merc_tile *tile) {
    for (auto const &lyr : layers) {
        if (lyr.visible(scale_denom)) {
            protozero::pbf_reader layer_msg;
            if (tile->layer_reader(lyr.name(), layer_msg)) {
                mapnik::layer lyr_copy(lyr);
                lyr_copy.set_srs(map_srs);
                std::shared_ptr<mapnik::vector_tile_impl::tile_datasource_pbf> ds =
                    std::make_shared<mapnik::vector_tile_impl::tile_datasource_pbf>(layer_msg, tile->x(), tile->y(),
                                                                                    tile->z());
                ds->set_envelope(m_req.get_buffered_extent());
                lyr_copy.set_datasource(ds);
                std::set<std::string> names;
                ren.apply_to_layer(lyr_copy, ren, map_proj, m_req.scale(), scale_denom, m_req.width(), m_req.height(),
                                   m_req.extent(), m_req.buffer_size(), names);
            }
        }
    }
}

/*
 * Class:     mapnik_VectorTile
 * Method:    renderImpl
 * Signature: (Lgeowin/mapnik/MapDefinition;Lmapnik/Image;[JIDDLjava/util/Map;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_VectorTile_renderImpl(JNIEnv *env, jobject obj, jobject mapj,
                                                                jobject surfacej, jlongArray zxy, jint buffer_size,
                                                                jdouble scale, jdouble scale_denominator,
                                                                jobject variablesj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);

    if (mapj == NULL) throw std::runtime_error("map is null");
    auto &map_in = *LOAD_MAP_POINTER(mapj);

    if (surfacej == NULL) throw std::runtime_error("surface is null");
    auto &im = *LOAD_IMAGE_POINTER(surfacej);

    mapnik::box2d<double> map_extent;
    if (zxy != NULL) {
        if (env->GetArrayLength(zxy) != 3) throw std::runtime_error("zxy must be an array of length 3");
        JNILongArrayElements tmp(env, zxy);
        map_extent = mapnik::vector_tile_impl::tile_mercator_bbox(tmp[1], tmp[2], tmp[0]);
    } else {
        map_extent = mapnik::vector_tile_impl::tile_mercator_bbox(tile->x(), tile->y(), tile->z());
    }

    mapnik::request m_req(im.width(), im.height(), map_extent);
    m_req.set_buffer_size(buffer_size);
    mapnik::projection map_proj(map_in.srs(), true);
    if (scale_denominator <= 0.0)
        scale_denominator = mapnik::scale_denominator(m_req.scale(), map_proj.is_geographic());
    scale_denominator *= scale;
    const auto &layers = map_in.layers();

    mapnik::attributes variables;
    jni_map_to_mapnik_attributes(env, variablesj, variables);
    mapnik::agg_renderer<mapnik::image_rgba8> ren(map_in, m_req, variables, im, scale);
    ren.start_map_processing(map_in);
    process_layers(ren, m_req, map_proj, layers, scale_denominator, map_in.srs(), tile);
    ren.end_map_processing(map_in);

    TRAILER_VOID;
}
