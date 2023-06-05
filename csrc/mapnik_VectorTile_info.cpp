#include "mapnik_VectorTile.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    info
 * Signature: ([B)Lmapnik/VectorTile/Info;
 */
JNIEXPORT jobject JNICALL Java_mapnik_VectorTile_info(JNIEnv *env, jclass, jbyteArray infoj) {
    PREAMBLE;
    if (infoj == NULL) throw std::exception("info is null");
    auto size = env->GetArrayLength(infoj);
    JNIByteArrayElements dataj(env, infoj);
    auto data = reinterpret_cast<const char *>(dataj.data());
    std::set<mapnik::vector_tile_impl::validity_error> errors;
    bool has_errors = false;
    std::size_t layers_size = 0;
    bool first_layer = true;
    std::set<std::string> layer_names_set;
    std::uint32_t version = 1;
    protozero::pbf_reader tile_msg;
    std::string decompressed;
    jobject out = env->NewObject(CLASS_VECTOR_TILE_INFO, CTOR_VECTOR_TILE_INFO);
    std::vector<jobject> layers;
    try {
        if (mapnik::vector_tile_impl::is_gzip_compressed(data, size) ||
            mapnik::vector_tile_impl::is_zlib_compressed(data, size)) {
            mapnik::vector_tile_impl::zlib_decompress(data, size, decompressed);
            tile_msg = protozero::pbf_reader(decompressed);
        } else {
            tile_msg = protozero::pbf_reader(data, size);
        }
        while (tile_msg.next()) {
            switch (tile_msg.tag()) {
            case mapnik::vector_tile_impl::Tile_Encoding::LAYERS: {
                jobject layer_obj = env->NewObject(CLASS_VECTOR_TILE_INFO_LAYER, CTOR_VECTOR_TILE_INFO_LAYER);
                std::uint64_t point_feature_count = 0;
                std::uint64_t line_feature_count = 0;
                std::uint64_t polygon_feature_count = 0;
                std::uint64_t unknown_feature_count = 0;
                std::uint64_t raster_feature_count = 0;
                auto layer_view = tile_msg.get_view();
                protozero::pbf_reader layer_props_msg(layer_view);
                auto layer_info = mapnik::vector_tile_impl::get_layer_name_and_version(layer_props_msg);
                std::string const &layer_name = layer_info.first;
                std::uint32_t layer_version = layer_info.second;
                std::set<mapnik::vector_tile_impl::validity_error> layer_errors;
                if (version > 2 || version < 1) {
                    layer_errors.insert(mapnik::vector_tile_impl::LAYER_HAS_UNSUPPORTED_VERSION);
                }
                protozero::pbf_reader layer_msg(layer_view);
                mapnik::vector_tile_impl::layer_is_valid(layer_msg, layer_errors, point_feature_count,
                                                         line_feature_count, polygon_feature_count,
                                                         unknown_feature_count, raster_feature_count);
                std::uint64_t feature_count = point_feature_count + line_feature_count + polygon_feature_count +
                                              unknown_feature_count + raster_feature_count;
                if (!layer_name.empty()) {
                    auto p = layer_names_set.insert(layer_name);
                    if (!p.second) errors.insert(mapnik::vector_tile_impl::TILE_REPEATED_LAYER_NAMES);
                    // layer_obj.Set("name", layer_name);
                    env->SetObjectField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_NAME,
                                        env->NewStringUTF(layer_name.c_str()));
                }
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_FEATURES, feature_count);
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_POINT_FEATURES, point_feature_count);
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_LINESTRING_FEATURES, line_feature_count);
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_POLYGON_FEATURES, polygon_feature_count);
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_UNKNOWN_FEATURES, unknown_feature_count);
                env->SetLongField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_RASTER_FEATURES, raster_feature_count);
                env->SetIntField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_VERSION, layer_version);
                if (!layer_errors.empty()) {
                    has_errors = true;
                    jobjectArray err_arr = env->NewObjectArray(layer_errors.size(), CLASS_STRING, NULL);
                    std::size_t i = 0;
                    for (auto const &e : layer_errors)
                        env->SetObjectArrayElement(
                            err_arr, i++,
                            env->NewStringUTF(mapnik::vector_tile_impl::validity_error_to_string(e).c_str()));
                    env->SetObjectField(layer_obj, FIELD_VECTOR_TILE_INFO_LAYER_ERRORS, err_arr);
                }
                if (first_layer) {
                    version = layer_version;
                } else {
                    if (version != layer_version) {
                        errors.insert(mapnik::vector_tile_impl::TILE_HAS_DIFFERENT_VERSIONS);
                    }
                }
                first_layer = false;
                layers.push_back(layer_obj);
            } break;
            default:
                errors.insert(mapnik::vector_tile_impl::TILE_HAS_UNKNOWN_TAG);
                tile_msg.skip();
                break;
            }
        }
    } catch (...) { errors.insert(mapnik::vector_tile_impl::INVALID_PBF_BUFFER); }
    jobjectArray layersj = env->NewObjectArray(layers.size(), CLASS_VECTOR_TILE_INFO_LAYER, NULL);
    std::size_t i = 0;
    for (jobject layer : layers) env->SetObjectArrayElement(layersj, i++, layer);
    env->SetObjectField(out, FIELD_VECTOR_TILE_INFO_LAYERS, layersj);
    env->SetBooleanField(out, FIELD_VECTOR_TILE_INFO_ERRORS, has_errors);
    if (!errors.empty()) {
        jobjectArray err_arr = env->NewObjectArray(errors.size(), CLASS_STRING, NULL);
        i = 0;
        for (auto const &e : errors) {
            env->SetObjectArrayElement(
                err_arr, i++, env->NewStringUTF(mapnik::vector_tile_impl::validity_error_to_string(e).c_str()));
        }
        env->SetObjectField(out, FIELD_VECTOR_TILE_INFO_TILE_ERRORS, err_arr);
    }
    return out;
    TRAILER(NULL);
}
