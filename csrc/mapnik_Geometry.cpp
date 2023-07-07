#include "geowin_mapnik_Geometry.h"
//
#include <mapnik/geometry/geometry_type.hpp>
#include <mapnik/util/geometry_to_geojson.hpp>
#include <mapnik/util/geometry_to_wkb.hpp>
#include <mapnik/util/geometry_to_wkt.hpp>

#include "globals.hpp"

static bool to_geojson_projected(std::string &json, mapnik::geometry::geometry<double> const &geom,
                                 mapnik::proj_transform const &prj_trans) {
    unsigned int n_err = 0;
    mapnik::geometry::geometry<double> projected_geom = mapnik::geometry::reproject_copy(geom, prj_trans, n_err);
    if (n_err > 0) return false;
    return mapnik::util::to_geojson(json, projected_geom);
}

/*
 * Class:     mapnik_Geometry
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_Geometry_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete reinterpret_cast<mapnik::feature_ptr *>(ptr);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Geometry
 * Method:    extent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_Geometry_extent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto feature = LOAD_FEATURE_POINTER(obj);
    return box2dFromNative(env, (*feature)->envelope());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Geometry
 * Method:    toJSON
 * Signature: (Lgeowin/mapnik/ProjTransform;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_Geometry_toJSON(JNIEnv *env, jobject obj, jobject transform) {
    PREAMBLE;
    const auto &geom = (*LOAD_FEATURE_POINTER(obj))->get_geometry();
    std::string json;
    if (transform == NULL) {
        if (!mapnik::util::to_geojson(json, geom)) throw std::runtime_error("Failed to generate GeoJSON");
    } else {
        auto tr = LOAD_PROJ_TRANSFORM_POINTER(transform);
        if (!to_geojson_projected(json, geom, *tr)) throw std::runtime_error("Failed to generate GeoJSON");
    }
    return env->NewStringUTF(json.c_str());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Geometry
 * Method:    toWKB
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_geowin_mapnik_Geometry_toWKB(JNIEnv *env, jobject obj) {
    PREAMBLE;
    const auto &geom = (*LOAD_FEATURE_POINTER(obj))->get_geometry();
    mapnik::util::wkb_buffer_ptr wkb = mapnik::util::to_wkb(geom, mapnik::wkbNDR);
    if (!wkb) throw std::runtime_error("Failed to generate WKB - geometry likely null");
    jbyteArray ret = env->NewByteArray(wkb->size());
    env->SetByteArrayRegion(ret, 0, wkb->size(), reinterpret_cast<const jbyte *>(wkb->buffer()));
    return ret;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Geometry
 * Method:    toWKT
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_Geometry_toWKT(JNIEnv *env, jobject obj) {
    PREAMBLE;
    const auto &geom = (*LOAD_FEATURE_POINTER(obj))->get_geometry();
    std::string wkt;
    if (!mapnik::util::to_wkt(wkt, geom)) throw std::runtime_error("Failed to generate WKT");
    return env->NewStringUTF(wkt.c_str());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Geometry
 * Method:    typeImpl
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_Geometry_typeImpl(JNIEnv *env, jobject obj) {
    PREAMBLE;
    const auto &geom = (*LOAD_FEATURE_POINTER(obj))->get_geometry();
    return mapnik::geometry::geometry_type(geom);
    TRAILER(0);
}
