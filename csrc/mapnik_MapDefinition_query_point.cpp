#include "geowin_mapnik_MapDefinition.h"
//
#include "globals.hpp"

namespace {

jobject query_point_layer_impl(JNIEnv *env, mapnik::Map *map, unsigned int idx, double x, double y, bool geo_coords) {
    auto fs = new mapnik::featureset_ptr(geo_coords ? map->query_point(idx, x, y) : map->query_map_point(idx, x, y));
    return creatrFeatureSetObj(env, fs);
}

jobject query_point_all_impl(JNIEnv *env, jobject obj, jdouble x, jdouble y, bool geo_coords) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    auto &layers = map->layers();
    jobject out = env->NewObject(CLASS_HASHMAP, CTOR_HASHMAP);
    unsigned int idx = 0;
    for (auto &&layer : layers) {
        JNIObject name(env, env->NewStringUTF(layer.name().c_str()));
        JNIObject fs(env, query_point_layer_impl(env, map, idx++, x, y, geo_coords));
        JNIObjectAllowNull(env, env->CallObjectMethod(out, METHOD_HASHMAP_PUT, name.get(), fs.get()));
    }
    return out;
    TRAILER(NULL);
}

jobject query_point_layer_impl(JNIEnv *env, jobject obj, jdouble x, jdouble y, jstring layer_namej, bool geo_coords) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    auto &layers = map->layers();
    std::string name = JNIString(env, layer_namej).get();
    auto it = std::find_if(layers.begin(), layers.end(), [&](auto &&x) { return x.name() == name; });
    if (it == layers.end()) {
        std::ostringstream ss;
        ss << "layer with name `" << name << "` not found";
        throw_runtime_exception(env, ss.str().c_str());
        return NULL;
    }
    return query_point_layer_impl(env, map, distance(layers.begin(), it), x, y, geo_coords);
    TRAILER(NULL);
}

jobject query_point_layer_impl(JNIEnv *env, jobject obj, jdouble x, jdouble y, jint layer_idx, bool geo_coords) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    auto &layers = map->layers();
    if (layer_idx < 0 || layer_idx >= layers.size()) throw std::out_of_range("layer index out of range");
    return query_point_layer_impl(env, map, layer_idx, x, y, geo_coords);
    TRAILER(NULL);
}

}  // namespace

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DDLjava/lang/String;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryMapPoint__DDLjava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                         jdouble x, jdouble y,
                                                                                         jstring layer_namej) {
    return query_point_layer_impl(env, obj, x, y, layer_namej, false);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DDI)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryMapPoint__DDI(JNIEnv *env, jobject obj, jdouble x, jdouble y,
                                                                       jint layer_idx) {
    return query_point_layer_impl(env, obj, x, y, layer_idx, false);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryMapPoint
 * Signature: (DD)Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryMapPoint__DD(JNIEnv *env, jobject obj, jdouble x, jdouble y) {
    return query_point_all_impl(env, obj, x, y, false);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DDLjava/lang/String;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryPoint__DDLjava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                      jdouble x, jdouble y,
                                                                                      jstring layer_namej) {
    return query_point_layer_impl(env, obj, x, y, layer_namej, true);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DDI)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryPoint__DDI(JNIEnv *env, jobject obj, jdouble x, jdouble y,
                                                                    jint layer_idx) {
    return query_point_layer_impl(env, obj, x, y, layer_idx, true);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    queryPoint
 * Signature: (DD)Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_queryPoint__DD(JNIEnv *env, jobject obj, jdouble x, jdouble y) {
    return query_point_all_impl(env, obj, x, y, true);
}
