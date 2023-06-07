#include "mapnik_VectorTile.h"
//
#include <mapnik/geometry/is_simple.hpp>
#include <mapnik/geometry/is_valid.hpp>

#include "globals.hpp"

namespace {
// LCOV_EXCL_START
struct not_simple_feature {
    not_simple_feature(std::string const& layer_, std::int64_t feature_id_) : layer(layer_), feature_id(feature_id_) {}
    std::string const layer;
    std::int64_t const feature_id;
};
// LCOV_EXCL_STOP

struct not_valid_feature {
    not_valid_feature(std::string const& message_, std::string const& layer_, std::int64_t feature_id_,
                      std::string const& geojson_)
        : message(message_), layer(layer_), feature_id(feature_id_), geojson(geojson_) {}
    std::string const message;
    std::string const layer;
    std::int64_t const feature_id;
    std::string const geojson;
};

void layer_not_simple(protozero::pbf_reader const& layer_msg, unsigned x, unsigned y, unsigned z,
                      std::vector<not_simple_feature>& errors) {
    mapnik::vector_tile_impl::tile_datasource_pbf ds(layer_msg, x, y, z);
    mapnik::query q(mapnik::box2d<double>(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                                          std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
    mapnik::layer_descriptor ld = ds.get_descriptor();
    for (auto const& item : ld.get_descriptors()) { q.add_property_name(item.get_name()); }
    mapnik::featureset_ptr fs = ds.features(q);
    if (fs && mapnik::is_valid(fs)) {
        mapnik::feature_ptr feature;
        while ((feature = fs->next())) {
            if (!mapnik::geometry::is_simple(feature->get_geometry()))  // NOLINT
            {
                // Right now we don't have an obvious way of bypassing our validation
                // process in JS, so let's skip testing this line
                // LCOV_EXCL_START
                errors.emplace_back(ds.get_name(), feature->id());
                // LCOV_EXCL_STOP
            }
        }
    }
}

struct visitor_geom_valid {
    std::vector<not_valid_feature>& errors;
    mapnik::feature_ptr& feature;
    std::string const& layer_name;
    bool split_multi_features;

    visitor_geom_valid(std::vector<not_valid_feature>& errors_, mapnik::feature_ptr& feature_,
                       std::string const& layer_name_, bool split_multi_features_)
        : errors(errors_), feature(feature_), layer_name(layer_name_), split_multi_features(split_multi_features_) {}

    void operator()(mapnik::geometry::geometry_empty const&) {}

    template <typename T> void operator()(mapnik::geometry::point<T> const& geom) {
        std::string message;
        if (!mapnik::geometry::is_valid(geom, message))  // NOLINT
        {
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            std::string result;
            std::string feature_str;
            result += "{\"type\":\"FeatureCollection\",\"features\":[";
            feature_new.set_data(feature->get_data());
            feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
            result += "]}";
            errors.emplace_back(message, layer_name, feature->id(), result);
        }
    }

    template <typename T> void operator()(mapnik::geometry::multi_point<T> const& geom) {
        std::string message;
        if (!mapnik::geometry::is_valid(geom, message))  // NOLINT
        {
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            std::string result;
            std::string feature_str;
            result += "{\"type\":\"FeatureCollection\",\"features\":[";
            feature_new.set_data(feature->get_data());
            feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
            result += "]}";
            errors.emplace_back(message, layer_name, feature->id(), result);
        }
    }

    template <typename T> void operator()(mapnik::geometry::line_string<T> const& geom) {
        std::string message;
        if (!mapnik::geometry::is_valid(geom, message))  // NOLINT
        {
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            std::string result;
            std::string feature_str;
            result += "{\"type\":\"FeatureCollection\",\"features\":[";
            feature_new.set_data(feature->get_data());
            feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
            result += "]}";
            errors.emplace_back(message, layer_name, feature->id(), result);
        }
    }

    template <typename T> void operator()(mapnik::geometry::multi_line_string<T> const& geom) {
        if (split_multi_features) {
            for (auto const& ls : geom) {
                std::string message;
                if (!mapnik::geometry::is_valid(ls, message))  // NOLINT
                {
                    mapnik::feature_impl feature_new(feature->context(), feature->id());
                    std::string result;
                    std::string feature_str;
                    result += "{\"type\":\"FeatureCollection\",\"features\":[";
                    feature_new.set_data(feature->get_data());
                    feature_new.set_geometry(mapnik::geometry::geometry<T>(ls));
                    if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                        // LCOV_EXCL_START
                        throw std::runtime_error("Failed to generate GeoJSON geometry");
                        // LCOV_EXCL_STOP
                    }
                    result += feature_str;
                    result += "]}";
                    errors.emplace_back(message, layer_name, feature->id(), result);
                }
            }
        } else {
            std::string message;
            if (!mapnik::geometry::is_valid(geom, message))  // NOLINT
            {
                mapnik::feature_impl feature_new(feature->context(), feature->id());
                std::string result;
                std::string feature_str;
                result += "{\"type\":\"FeatureCollection\",\"features\":[";
                feature_new.set_data(feature->get_data());
                feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
                if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                    // LCOV_EXCL_START
                    throw std::runtime_error("Failed to generate GeoJSON geometry");
                    // LCOV_EXCL_STOP
                }
                result += feature_str;
                result += "]}";
                errors.emplace_back(message, layer_name, feature->id(), result);
            }
        }
    }

    template <typename T> void operator()(mapnik::geometry::polygon<T> const& geom) {
        std::string message;
        if (!mapnik::geometry::is_valid(geom, message))  // NOLINT
        {
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            std::string result;
            std::string feature_str;
            result += "{\"type\":\"FeatureCollection\",\"features\":[";
            feature_new.set_data(feature->get_data());
            feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
            result += "]}";
            errors.emplace_back(message, layer_name, feature->id(), result);
        }
    }

    template <typename T> void operator()(mapnik::geometry::multi_polygon<T> const& geom) {
        if (split_multi_features) {
            for (auto const& poly : geom) {
                std::string message;
                if (!mapnik::geometry::is_valid(poly, message)) {
                    mapnik::feature_impl feature_new(feature->context(), feature->id());
                    std::string result;
                    std::string feature_str;
                    result += "{\"type\":\"FeatureCollection\",\"features\":[";
                    feature_new.set_data(feature->get_data());
                    feature_new.set_geometry(mapnik::geometry::geometry<T>(poly));
                    if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                        // LCOV_EXCL_START
                        throw std::runtime_error("Failed to generate GeoJSON geometry");
                        // LCOV_EXCL_STOP
                    }
                    result += feature_str;
                    result += "]}";
                    errors.emplace_back(message, layer_name, feature->id(), result);
                }
            }
        } else {
            std::string message;
            if (!mapnik::geometry::is_valid(geom, message)) {
                mapnik::feature_impl feature_new(feature->context(), feature->id());
                std::string result;
                std::string feature_str;
                result += "{\"type\":\"FeatureCollection\",\"features\":[";
                feature_new.set_data(feature->get_data());
                feature_new.set_geometry(mapnik::geometry::geometry<T>(geom));
                if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                    // LCOV_EXCL_START
                    throw std::runtime_error("Failed to generate GeoJSON geometry");
                    // LCOV_EXCL_STOP
                }
                result += feature_str;
                result += "]}";
                errors.emplace_back(message, layer_name, feature->id(), result);
            }
        }
    }

    template <typename T> void operator()(mapnik::geometry::geometry_collection<T> const& geom) {
        // This should never be able to be reached.
        // LCOV_EXCL_START
        for (auto const& g : geom) { mapnik::util::apply_visitor((*this), g); }
        // LCOV_EXCL_STOP
    }
};

void layer_not_valid(protozero::pbf_reader& layer_msg, unsigned x, unsigned y, unsigned z,
                     std::vector<not_valid_feature>& errors, bool split_multi_features = false, bool lat_lon = false,
                     bool web_merc = false) {
    if (web_merc || lat_lon) {
        mapnik::vector_tile_impl::tile_datasource_pbf ds(layer_msg, x, y, z);
        mapnik::query q(mapnik::box2d<double>(std::numeric_limits<double>::lowest(),
                                              std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(),
                                              std::numeric_limits<double>::max()));
        mapnik::layer_descriptor ld = ds.get_descriptor();
        for (auto const& item : ld.get_descriptors()) { q.add_property_name(item.get_name()); }
        mapnik::featureset_ptr fs = ds.features(q);
        if (fs && mapnik::is_valid(fs)) {
            mapnik::feature_ptr feature;
            while ((feature = fs->next())) {
                if (lat_lon) {
                    mapnik::projection wgs84("+init=epsg:4326", true);
                    mapnik::projection merc("+init=epsg:3857", true);
                    mapnik::proj_transform prj_trans(merc, wgs84);
                    unsigned int n_err = 0;
                    mapnik::util::apply_visitor(
                        visitor_geom_valid(errors, feature, ds.get_name(), split_multi_features),
                        mapnik::geometry::reproject_copy(feature->get_geometry(), prj_trans, n_err));
                } else {
                    mapnik::util::apply_visitor(
                        visitor_geom_valid(errors, feature, ds.get_name(), split_multi_features),
                        feature->get_geometry());
                }
            }
        }
    } else {
        std::vector<protozero::pbf_reader> layer_features;
        std::uint32_t version = 1;
        std::string layer_name;
        while (layer_msg.next()) {
            switch (layer_msg.tag()) {
            case mapnik::vector_tile_impl::Layer_Encoding::NAME: layer_name = layer_msg.get_string(); break;
            case mapnik::vector_tile_impl::Layer_Encoding::FEATURES:
                layer_features.push_back(layer_msg.get_message());
                break;
            case mapnik::vector_tile_impl::Layer_Encoding::VERSION: version = layer_msg.get_uint32(); break;
            default: layer_msg.skip(); break;
            }
        }
        for (auto feature_msg : layer_features) {
            mapnik::vector_tile_impl::GeometryPBF::pbf_itr geom_itr;
            bool has_geom = false;
            bool has_geom_type = false;
            std::int32_t geom_type_enum = 0;
            // std::uint64_t feature_id = 0;
            while (feature_msg.next()) {
                switch (feature_msg.tag()) {
                case mapnik::vector_tile_impl::Feature_Encoding::TYPE:
                    geom_type_enum = feature_msg.get_enum();
                    has_geom_type = true;
                    break;
                case mapnik::vector_tile_impl::Feature_Encoding::GEOMETRY:
                    geom_itr = feature_msg.get_packed_uint32();
                    has_geom = true;
                    break;
                default: feature_msg.skip(); break;
                }
            }
            if (has_geom && has_geom_type) {
                // Decode the geometry first into an int64_t mapnik geometry
                mapnik::context_ptr ctx = std::make_shared<mapnik::context_type>();
                mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx, 1));
                mapnik::vector_tile_impl::GeometryPBF geoms(geom_itr);
                feature->set_geometry(mapnik::vector_tile_impl::decode_geometry<double>(geoms, geom_type_enum, version,
                                                                                        0.0, 0.0, 1.0, 1.0));
                mapnik::util::apply_visitor(visitor_geom_valid(errors, feature, layer_name, split_multi_features),
                                            feature->get_geometry());
            }
        }
    }
}

void vector_tile_not_simple(const mapnik::vector_tile_impl::merc_tile* tile, std::vector<not_simple_feature>& errors) {
    protozero::pbf_reader tile_msg(tile->get_reader());
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        protozero::pbf_reader layer_msg(tile_msg.get_message());
        layer_not_simple(layer_msg, tile->x(), tile->y(), tile->z(), errors);
    }
}

jobjectArray make_not_simple_array(JNIEnv* env, std::vector<not_simple_feature>& errors) {
    jobjectArray array = env->NewObjectArray(errors.size(), CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE, nullptr);
    jsize idx = 0;
    for (auto const& error : errors) {
        JNIObject obj(env, env->NewObject(CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE, CTOR_VECTOR_TILE_NOT_SIMPLE_FEATURE));
        JNIObject layer(env, env->NewStringUTF(error.layer.c_str()));
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_LAYER, layer.get());
        env->SetLongField(obj.get(), FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_FEATURE_ID, error.feature_id);
        env->SetObjectArrayElement(array, idx++, obj.get());
    }
    return array;
}

void vector_tile_not_valid(const mapnik::vector_tile_impl::merc_tile* tile, std::vector<not_valid_feature>& errors,
                           bool split_multi_features = false, bool lat_lon = false, bool web_merc = false) {
    protozero::pbf_reader tile_msg(tile->get_reader());
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        protozero::pbf_reader layer_msg(tile_msg.get_message());
        layer_not_valid(layer_msg, tile->x(), tile->y(), tile->z(), errors, split_multi_features, lat_lon, web_merc);
    }
}

jobjectArray make_not_valid_array(JNIEnv* env, std::vector<not_valid_feature>& errors) {
    jobjectArray array = env->NewObjectArray(errors.size(), CLASS_VECTOR_TILE_NOT_VALID_FEATURE, nullptr);
    jsize idx = 0;
    for (auto const& error : errors) {
        JNIObject obj(env, env->NewObject(CLASS_VECTOR_TILE_NOT_VALID_FEATURE, CTOR_VECTOR_TILE_NOT_VALID_FEATURE));
        JNIObject layer(env, env->NewStringUTF(error.layer.c_str()));
        JNIObject message(env, env->NewStringUTF(error.message.c_str()));
        JNIObject geojson(env, env->NewStringUTF(error.geojson.c_str()));
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_NOT_VALID_FEATURE_LAYER, layer.get());
        env->SetLongField(obj.get(), FIELD_VECTOR_TILE_NOT_VALID_FEATURE_FEATURE_ID, error.feature_id);
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_NOT_VALID_FEATURE_MESSAGE, message.get());
        env->SetObjectField(obj.get(), FIELD_VECTOR_TILE_NOT_VALID_FEATURE_GEOJSON, geojson.get());
        env->SetObjectArrayElement(array, idx++, obj.get());
    }
    return array;
}

}  // namespace
/*
 * Class:     mapnik_VectorTile
 * Method:    reportGeometrySimplicity
 * Signature: ()[Lmapnik/VectorTile/NotSimpleFeature;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_reportGeometrySimplicity(JNIEnv* env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::vector<not_simple_feature> result;
    vector_tile_not_simple(tile, result);
    return make_not_simple_array(env, result);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    reportGeometryValidityImpl
 * Signature: (ZZZ)[Lmapnik/VectorTile/NotValidFeature;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_reportGeometryValidityImpl(JNIEnv* env, jobject obj,
                                                                                 jboolean split_multi_features,
                                                                                 jboolean lat_lon, jboolean web_merc) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::vector<not_valid_feature> result;
    vector_tile_not_valid(tile, result, split_multi_features, lat_lon, web_merc);
    return make_not_valid_array(env, result);
    TRAILER(NULL);
}
