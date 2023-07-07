#include "geowin_mapnik_Feature.h"

#include <mapnik/json/feature_parser.hpp>
#include <mapnik/util/feature_to_geojson.hpp>

#include "globals.hpp"
#include "utils/MapnikValueToJNIVisitor.hpp"

/*
 * Class:     mapnik_Feature
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_Feature_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    auto feature = reinterpret_cast<mapnik::feature_ptr *>(ptr);
    delete feature;
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Feature
 * Method:    fromJSON
 * Signature: (Ljava/lang/String;)Lmapnik/Feature;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_Feature_fromJSON(JNIEnv *env, jclass, jstring geojson) {
    PREAMBLE;
    JNIString json(env, geojson);
    auto feature =
        new mapnik::feature_ptr(mapnik::feature_factory::create(std::make_shared<mapnik::context_type>(), 1));
    if (!mapnik::json::from_geojson(json.get(), **feature)) throw std::runtime_error("Failed to read GeoJSON");
    return createFeatureObj(env, feature);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Feature
 * Method:    attributes
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_Feature_attributes(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    jobject attributes = env->NewObject(CLASS_HASHMAP, CTOR_HASHMAP);
    MapnikValueToJNIVisitor visitor(env);
    for (auto &&[key, value] : **feature) {
        JNIObject keyj(env, env->NewStringUTF(key.c_str()));
        JNIObject valuej(env, mapnik::util::apply_visitor(visitor, value));
        JNIObjectAllowNull(env,
                           env->CallObjectMethod(attributes, METHOD_HASHMAP_PUT, (jstring)keyj.get(), valuej.get()));
    }
    return attributes;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Feature
 * Method:    extent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_Feature_extent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    return box2dFromNative(env, (*feature)->envelope());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Feature
 * Method:    geometry
 * Signature: ()Lmapnik/Geometry;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_Feature_geometry(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    auto geom_feature = new mapnik::feature_ptr(*feature);
    return createGeometryObj(env, geom_feature);
    TRAILER(0);
}

/*
 * Class:     mapnik_Feature
 * Method:    id
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_Feature_id(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    return (*feature)->id();
    TRAILER(0);
}

/*
 * Class:     mapnik_Feature
 * Method:    toJSON
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_Feature_toJSON(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    std::string json;
    if (!mapnik::util::to_geojson(json, **feature)) throw std::runtime_error("Failed to generate GeoJSON");
    return env->NewStringUTF(json.c_str());
    TRAILER(NULL);
}
