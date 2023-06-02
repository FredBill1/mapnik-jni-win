#include "mapnik_VectorTile.h"
//
#include "globals.hpp"

static bool layer_to_geojson(protozero::pbf_reader const &layer, std::string &result, unsigned x, unsigned y,
                             unsigned z) {
    mapnik::vector_tile_impl::tile_datasource_pbf ds(layer, x, y, z);
    mapnik::projection wgs84("epsg:4326", true);
    mapnik::projection merc("epsg:3857", true);
    mapnik::proj_transform prj_trans(merc, wgs84);
    // This mega box ensures we capture all features, including those
    // outside the tile extent. Geometries outside the tile extent are
    // likely when the vtile was created by clipping to a buffered extent
    mapnik::query q(mapnik::box2d<double>(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                                          std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
    mapnik::layer_descriptor ld = ds.get_descriptor();
    for (auto const &item : ld.get_descriptors()) { q.add_property_name(item.get_name()); }
    mapnik::featureset_ptr fs = ds.features(q);
    bool first = true;
    if (fs && mapnik::is_valid(fs)) {
        mapnik::feature_ptr feature;
        while ((feature = fs->next())) {
            if (first) {
                first = false;
            } else {
                result += "\n,";
            }
            std::string feature_str;
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            feature_new.set_data(feature->get_data());
            unsigned int n_err = 0;
            feature_new.set_geometry(mapnik::geometry::reproject_copy(feature->get_geometry(), prj_trans, n_err));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
        }
    }
    return !first;
}

static void write_geojson_array(std::string &result, mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader tile_msg = v->get_reader();
    result += "[";
    bool first = true;
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        if (first) {
            first = false;
        } else {
            result += ",";
        }
        auto pair_data = tile_msg.get_data();
        protozero::pbf_reader layer_msg(pair_data);
        protozero::pbf_reader name_msg(pair_data);
        std::string layer_name;
        if (name_msg.next(mapnik::vector_tile_impl::Layer_Encoding::NAME)) { layer_name = name_msg.get_string(); }
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + layer_name + "\",\"features\":[";
        std::string features;
        bool hit = layer_to_geojson(layer_msg, features, v->x(), v->y(), v->z());
        if (hit) { result += features; }
        result += "]}";
    }
    result += "]";
}

static void write_geojson_all(std::string &result, mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader tile_msg = v->get_reader();
    result += "{\"type\":\"FeatureCollection\",\"features\":[";
    bool first = true;
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        protozero::pbf_reader layer_msg(tile_msg.get_message());
        std::string features;
        bool hit = layer_to_geojson(layer_msg, features, v->x(), v->y(), v->z());
        if (hit) {
            if (first) {
                first = false;
            } else {
                result += ",";
            }
            result += features;
        }
    }
    result += "]}";
}

static bool write_geojson_layer_index(std::string &result, std::size_t layer_idx,
                                      mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader layer_msg;
    if (v->layer_reader(layer_idx, layer_msg) && v->get_layers().size() > layer_idx) {
        std::string layer_name = v->get_layers()[layer_idx];
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + layer_name + "\",\"features\":[";
        layer_to_geojson(layer_msg, result, v->x(), v->y(), v->z());
        result += "]}";
        return true;
    }
    // LCOV_EXCL_START
    return false;
    // LCOV_EXCL_STOP
}

static bool write_geojson_layer_name(std::string &result, std::string const &name,
                                     mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader layer_msg;
    if (v->layer_reader(name, layer_msg)) {
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + name + "\",\"features\":[";
        layer_to_geojson(layer_msg, result, v->x(), v->y(), v->z());
        result += "]}";
        return true;
    }
    return false;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    toGeoJSON
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_VectorTile_toGeoJSON__Ljava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                jstring layer_namej) {
    PREAMBLE;
    if (layer_namej == NULL) throw std::exception("layer name is null");
    auto layer_name_c = env->GetStringUTFChars(layer_namej, 0);
    std::string layer_name = layer_name_c;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::string result;
    if (layer_name == "__array__") {
        write_geojson_array(result, tile);
    } else if (layer_name == "__all__") {
        write_geojson_all(result, tile);
    } else {
        if (!write_geojson_layer_name(result, layer_name, tile))
            throw std::exception("layer does not exist in vector tile");
    }
    env->ReleaseStringUTFChars(layer_namej, layer_name_c);
    return env->NewStringUTF(result.c_str());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    toGeoJSON
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_VectorTile_toGeoJSON__J(JNIEnv *env, jobject obj, jlong layer_idx) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::string result;
    if (!write_geojson_layer_index(result, layer_idx, tile))
        throw std::exception("layer does not exist in vector tile");
    return env->NewStringUTF(result.c_str());
    TRAILER(NULL);
}

struct jni_value_visitor {
    jni_value_visitor(JNIEnv *env) : env_(env) {}
    jobject operator()(std::string const &val) { return env_->NewStringUTF(val.c_str()); }
    jobject operator()(bool const &val) {
        return env_->CallStaticObjectMethod(CLASS_BOOLEAN, METHOD_BOOLEAN_VALUEOF, val);
    }
    jobject operator()(int64_t const &val) {
        return env_->CallStaticObjectMethod(CLASS_LONG, METHOD_LONG_VALUEOF, val);
    }
    jobject operator()(uint64_t const &val) { return (*this)(static_cast<int64_t>(val)); }
    jobject operator()(double const &val) {
        return env_->CallStaticObjectMethod(CLASS_DOUBLE, METHOD_DOUBLE_VALUEOF, val);
    }
    jobject operator()(float const &val) { return (*this)(static_cast<double>(val)); }
    JNIEnv *env_;
};

struct geometry_array_visitor {
    geometry_array_visitor(JNIEnv *env) : env_(env) {}
    jobjectArray operator()() { return env_->NewObjectArray(0, CLASS_OBJECT, NULL); }
    jobjectArray operator()(mapnik::geometry::geometry_empty const &) { return (*this)(); }
    template <typename T> jobjectArray operator()(mapnik::geometry::point<T> const &geom) {
        jobjectArray arr = env_->NewObjectArray(2, CLASS_OBJECT, NULL);
        jni_value_visitor visitor(env_);
        env_->SetObjectArrayElement(arr, 0, visitor(geom.x));
        env_->SetObjectArrayElement(arr, 1, visitor(geom.y));
        return arr;
    }
    template <typename T> jobjectArray operator()(T &&geom) {
        if (geom.empty()) return (*this)();
        jobjectArray arr = env_->NewObjectArray(geom.size(), CLASS_OBJECT, NULL);
        std::uint32_t c = 0;
        for (auto const &pt : geom) env_->SetObjectArrayElement(arr, c++, (*this)(pt));
        return arr;
    }
    template <typename T> jobjectArray operator()(mapnik::geometry::geometry<T> const &geom) {
        return mapnik::util::apply_visitor((*this), geom);
    }

 private:
    JNIEnv *env_;
};

template <typename T> jobjectArray geometry_to_array(JNIEnv *env, mapnik::geometry::geometry<T> const &geom) {
    return mapnik::util::apply_visitor(geometry_array_visitor(env), geom);
}

struct geometry_type_name {
    template <typename T> const char *operator()(T const &geom) const {
        return mapnik::util::apply_visitor(*this, geom);
    }
    const char *operator()(mapnik::geometry::geometry_empty const &) const { return "Empty"; }
    template <typename T> const char *operator()(mapnik::geometry::point<T> const &) const { return "Point"; }
    template <typename T> const char *operator()(mapnik::geometry::line_string<T> const &) const {
        return "LineString";
    }
    template <typename T> const char *operator()(mapnik::geometry::polygon<T> const &) const { return "Polygon"; }
    template <typename T> const char *operator()(mapnik::geometry::multi_point<T> const &) const {
        return "MultiPoint";
    }
    template <typename T> const char *operator()(mapnik::geometry::multi_line_string<T> const &) const {
        return "MultiLineString";
    }
    template <typename T> const char *operator()(mapnik::geometry::multi_polygon<T> const &) const {
        return "MultiPolygon";
    }
    template <typename T> const char *operator()(mapnik::geometry::geometry_collection<T> const &) const {
        return "GeometryCollection";
    }
};

struct json_value_visitor {
    json_value_visitor(JNIEnv *env, jobject att_obj, const char *name)
        : env_(env), att_obj_(att_obj), key_(env->NewStringUTF(name)) {}
    void put(jobject val) { env_->CallObjectMethod(att_obj_, METHOD_HASHMAP_PUT, key_, val); }
    void operator()(std::string const &val) { put(env_->NewStringUTF(val.c_str())); }
    void operator()(bool const &val) { put(env_->CallStaticObjectMethod(CLASS_BOOLEAN, METHOD_BOOLEAN_VALUEOF, val)); }
    void operator()(int64_t const &val) { put(env_->CallStaticObjectMethod(CLASS_LONG, METHOD_LONG_VALUEOF, val)); }
    void operator()(uint64_t const &val) { (*this)(static_cast<int64_t>(val)); }
    void operator()(double const &val) { put(env_->CallStaticObjectMethod(CLASS_DOUBLE, METHOD_DOUBLE_VALUEOF, val)); }
    void operator()(float const &val) { (*this)(static_cast<double>(val)); }
    JNIEnv *env_;
    jobject att_obj_;
    jstring key_;
};

/*
 * Class:     mapnik_VectorTile
 * Method:    toJSON
 * Signature: (Z)Lmapnik/VectorTile/JSON;
 */
JNIEXPORT jobject JNICALL Java_mapnik_VectorTile_toJSON(JNIEnv *env, jobject obj, jboolean decode_geometry) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    jobject out = env->NewObject(CLASS_VECTOR_TILE_JSON, CTOR_VECTOR_TILE_JSON);

    protozero::pbf_reader tile_msg = tile->get_reader();
    jobjectArray arr = env->NewObjectArray(tile->get_layers().size(), CLASS_VECTOR_TILE_JSON_LAYER, NULL);
    std::size_t l_idx = 0;
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        protozero::pbf_reader layer_msg = tile_msg.get_message();
        jobject layer_obj = env->NewObject(CLASS_VECTOR_TILE_JSON_LAYER, CTOR_VECTOR_TILE_JSON_LAYER);
        std::vector<std::string> layer_keys;
        mapnik::vector_tile_impl::layer_pbf_attr_type layer_values;
        std::vector<protozero::pbf_reader> layer_features;
        protozero::pbf_reader val_msg;
        std::uint32_t version = 1;
        while (layer_msg.next()) {
            switch (layer_msg.tag()) {
            case mapnik::vector_tile_impl::Layer_Encoding::NAME:
                env->SetObjectField(layer_obj, FIELD_VECTOR_TILE_JSON_LAYER_NAME,
                                    env->NewStringUTF(layer_msg.get_string().c_str()));
                break;
            case mapnik::vector_tile_impl::Layer_Encoding::FEATURES:
                layer_features.push_back(layer_msg.get_message());
                break;
            case mapnik::vector_tile_impl::Layer_Encoding::KEYS: layer_keys.push_back(layer_msg.get_string()); break;
            case mapnik::vector_tile_impl::Layer_Encoding::VALUES:
                val_msg = layer_msg.get_message();
                while (val_msg.next()) {
                    switch (val_msg.tag()) {
                    case mapnik::vector_tile_impl::Value_Encoding::STRING:
                        layer_values.push_back(val_msg.get_string());
                        break;
                    case mapnik::vector_tile_impl::Value_Encoding::FLOAT:
                        layer_values.push_back(val_msg.get_float());
                        break;
                    case mapnik::vector_tile_impl::Value_Encoding::DOUBLE:
                        layer_values.push_back(val_msg.get_double());
                        break;
                    case mapnik::vector_tile_impl::Value_Encoding::INT:
                        layer_values.push_back(val_msg.get_int64());
                        break;
                    case mapnik::vector_tile_impl::Value_Encoding::UINT:
                        // LCOV_EXCL_START
                        layer_values.push_back(val_msg.get_uint64());
                        break;
                        // LCOV_EXCL_STOP
                    case mapnik::vector_tile_impl::Value_Encoding::SINT:
                        // LCOV_EXCL_START
                        layer_values.push_back(val_msg.get_sint64());
                        break;
                        // LCOV_EXCL_STOP
                    case mapnik::vector_tile_impl::Value_Encoding::BOOL:
                        layer_values.push_back(val_msg.get_bool());
                        break;
                    default:
                        // LCOV_EXCL_START
                        val_msg.skip();
                        break;
                        // LCOV_EXCL_STOP
                    }
                }
                break;
            case mapnik::vector_tile_impl::Layer_Encoding::EXTENT:
                env->SetIntField(layer_obj, FIELD_VECTOR_TILE_JSON_LAYER_EXTENT, layer_msg.get_uint32());
                break;
            case mapnik::vector_tile_impl::Layer_Encoding::VERSION:
                version = layer_msg.get_uint32();
                env->SetIntField(layer_obj, FIELD_VECTOR_TILE_JSON_LAYER_VERSION, version);
                break;
            default:
                // LCOV_EXCL_START
                layer_msg.skip();
                break;
                // LCOV_EXCL_STOP
            }
        }
        jobjectArray f_arr = env->NewObjectArray(layer_features.size(), CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, NULL);
        jsize f_idx = 0;
        for (auto feature_msg : layer_features) {
            jobject feature_obj =
                env->NewObject(CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, CTOR_VECTOR_TILE_JSON_LAYER_FEATURE);
            mapnik::vector_tile_impl::GeometryPBF::pbf_itr geom_itr;
            mapnik::vector_tile_impl::GeometryPBF::pbf_itr tag_itr;
            bool has_geom = false;
            bool has_geom_type = false;
            bool has_tags = false;
            std::int32_t geom_type_enum = 0;
            while (feature_msg.next()) {
                switch (feature_msg.tag()) {
                case mapnik::vector_tile_impl::Feature_Encoding::ID:
                    env->SetLongField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_ID, feature_msg.get_uint64());
                    break;
                case mapnik::vector_tile_impl::Feature_Encoding::TAGS:
                    tag_itr = feature_msg.get_packed_uint32();
                    has_tags = true;
                    break;
                case mapnik::vector_tile_impl::Feature_Encoding::TYPE:
                    geom_type_enum = feature_msg.get_enum();
                    has_geom_type = true;
                    env->SetIntField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_TYPE, geom_type_enum);
                    break;
                case mapnik::vector_tile_impl::Feature_Encoding::GEOMETRY:
                    geom_itr = feature_msg.get_packed_uint32();
                    has_geom = true;
                    break;
                case mapnik::vector_tile_impl::Feature_Encoding::RASTER: {
                    auto im_buffer = feature_msg.get_view();
                    jbyteArray im_bufferj = env->NewByteArray(im_buffer.size());
                    env->SetByteArrayRegion(im_bufferj, 0, im_buffer.size(),
                                            reinterpret_cast<const jbyte *>(im_buffer.data()));
                    env->SetObjectField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_RASTER, im_bufferj);
                    break;
                }
                default:
                    // LCOV_EXCL_START
                    feature_msg.skip();
                    break;
                    // LCOV_EXCL_STOP
                }
            }
            jobject att_obj = env->NewObject(CLASS_HASHMAP, CTOR_HASHMAP);
            if (has_tags) {
                for (auto _i = tag_itr.begin(); _i != tag_itr.end();) {
                    std::size_t key_name = *(_i++);
                    if (_i == tag_itr.end()) { break; }
                    std::size_t key_value = *(_i++);
                    if (key_name < layer_keys.size() && key_value < layer_values.size()) {
                        std::string const &name = layer_keys.at(key_name);
                        mapnik::vector_tile_impl::pbf_attr_value_type val = layer_values.at(key_value);
                        json_value_visitor vv(env, att_obj, name.c_str());
                        mapnik::util::apply_visitor(vv, val);
                    }
                }
            }
            env->SetObjectField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_PROPERTIES, att_obj);
            if (has_geom && has_geom_type) {
                if (decode_geometry) {
                    // Decode the geometry first into an int64_t mapnik geometry
                    mapnik::vector_tile_impl::GeometryPBF geoms(geom_itr);
                    mapnik::geometry::geometry<std::int64_t> geom =
                        mapnik::vector_tile_impl::decode_geometry<std::int64_t>(geoms, geom_type_enum, version, 0, 0,
                                                                                1.0, 1.0);
                    jobjectArray g_arr = geometry_to_array<std::int64_t>(env, geom);
                    env->SetObjectField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY, g_arr);
                    env->SetObjectField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY_TYPE,
                                        env->NewStringUTF(geometry_type_name()(geom)));
                } else {
                    std::vector<std::uint32_t> geom_vec;
                    for (auto _i = geom_itr.begin(); _i != geom_itr.end(); ++_i) geom_vec.push_back(*_i);
                    jobjectArray g_arr = env->NewObjectArray(geom_vec.size(), CLASS_OBJECT, NULL);
                    for (std::size_t k = 0; k < geom_vec.size(); ++k) {
                        env->SetObjectArrayElement(
                            g_arr, k, env->CallStaticObjectMethod(CLASS_INTEGER, METHOD_INTEGER_VALUEOF, geom_vec[k]));
                    }
                    env->SetObjectField(feature_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY, g_arr);
                }
            }
            env->SetObjectArrayElement(f_arr, f_idx++, feature_obj);
        }
        env->SetObjectField(layer_obj, FIELD_VECTOR_TILE_JSON_LAYER_FEATURES, f_arr);
        env->SetObjectArrayElement(arr, l_idx++, layer_obj);
    }
    env->SetObjectField(out, FIELD_VECTOR_TILE_JSON_LAYERS, arr);
    return out;

    TRAILER(NULL);
}