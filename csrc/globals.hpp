#ifndef _MAPNIK_JNI_GLOBALS_HPP
#define _MAPNIK_JNI_GLOBALS_HPP

#include <jni.h>

#include <boost/variant/static_visitor.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/attribute_collector.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/feature_factory.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/geometry/geometry_type.hpp>
#include <mapnik/geometry/reprojection.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/mapnik.hpp>
#include <mapnik/memory_datasource.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/raster.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/scale_denominator.hpp>
#include <mapnik/util/feature_to_geojson.hpp>
#include <mapnik/version.hpp>
#include <mapnik/vertex_processor.hpp>

#include "mapnik_jni_ids.hpp"
#include "utils/JNIObjectHolders.hpp"
#include "vector_tile_composite.hpp"
#include "vector_tile_config.hpp"
#include "vector_tile_geometry_decoder.hpp"
#include "vector_tile_load_tile.hpp"
#include "vector_tile_merc_tile.hpp"
#include "vector_tile_projection.hpp"
// -- Globals

inline void throw_error(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("java/lang/Error");
    env->ThrowNew(clazz, msg);
}

inline void throw_runtime_exception(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("java/lang/RuntimeException");
    env->ThrowNew(clazz, msg);
}

inline void throw_java_exception(JNIEnv* env, std::exception& e) {
    jclass clazz = env->FindClass("java/lang/RuntimeException");
    env->ThrowNew(clazz, e.what());
}

#define ASSERT_INITIALIZED \
    if (!initialized) throw_error(env, "Library not initialized")
#define TO_POINTER(ptr) ((void*)ptr)
#define FROM_POINTER(ptr) ((jlong)ptr)

inline jlong ASSERT_LONG_POINTER(jlong ptr) {
    if (!ptr) { throw std::runtime_error("Object is no longer valid"); }
    return ptr;
}

// Exception handling
#define PREAMBLE try {
#define TRAILER(v)                    \
    }                                 \
    catch (std::exception & e) {      \
        throw_java_exception(env, e); \
        return v;                     \
    }
#define TRAILER_VOID                  \
    }                                 \
    catch (std::exception & e) {      \
        throw_java_exception(env, e); \
        return;                       \
    }

// Pointer access macros
// clang-format off
#define LOAD_OBJECT_POINTER(object) ((void*)(ASSERT_LONG_POINTER(env->GetLongField(object, FIELD_PTR))))
#define LOAD_MAP_POINTER(object) (static_cast<mapnik::Map*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_LAYER_POINTER(object) (static_cast<mapnik::layer*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_DATASOURCE_POINTER(object) (static_cast<mapnik::datasource_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_TYPE_STYLE_POINTER(object) (static_cast<mapnik::feature_type_style*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_PROJECTION_POINTER(object) (static_cast<mapnik::projection*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_PROJ_TRANSFORM_POINTER(object) (static_cast<mapnik::proj_transform*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_QUERY_POINTER(object) (static_cast<mapnik::query*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_IMAGE_POINTER(object) (static_cast<mapnik::image_rgba8*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_VECTOR_TILE_POINTER(object) (static_cast<mapnik::vector_tile_impl::merc_tile*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_POINTER(object) (static_cast<mapnik::feature_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURESET_POINTER(object) (static_cast<mapnik::featureset_ptr*>(LOAD_OBJECT_POINTER(object)))

// clang-format on

inline jobject box2dFromNative(JNIEnv* env, mapnik::box2d<double> const& box) {
    jobject ret = env->AllocObject(CLASS_BOX2D);
    env->SetDoubleField(ret, FIELD_BOX2D_MINX, box.minx());
    env->SetDoubleField(ret, FIELD_BOX2D_MINY, box.miny());
    env->SetDoubleField(ret, FIELD_BOX2D_MAXX, box.maxx());
    env->SetDoubleField(ret, FIELD_BOX2D_MAXY, box.maxy());
    return ret;
}

inline mapnik::box2d<double> box2dToNative(JNIEnv* env, jobject box) {
    if (!box) throw std::runtime_error("Box2d is null");
    mapnik::box2d<double> ret(env->GetDoubleField(box, FIELD_BOX2D_MINX), env->GetDoubleField(box, FIELD_BOX2D_MINY),
                              env->GetDoubleField(box, FIELD_BOX2D_MAXX), env->GetDoubleField(box, FIELD_BOX2D_MAXY));
    return ret;
}

inline jobject coordFromNative(JNIEnv* env, mapnik::coord2d const& coord) {
    jobject ret = env->AllocObject(CLASS_COORD);
    env->SetDoubleField(ret, FIELD_COORD_X, coord.x);
    env->SetDoubleField(ret, FIELD_COORD_Y, coord.y);
    return ret;
}

inline mapnik::coord2d coordToNative(JNIEnv* env, jobject coord) {
    mapnik::coord2d ret(env->GetDoubleField(coord, FIELD_COORD_X), env->GetDoubleField(coord, FIELD_COORD_Y));
    return ret;
}

inline mapnik::color colorToNative(JNIEnv* env, jobject c) {
    return mapnik::color(env->GetIntField(c, FIELD_COLOR_RED), env->GetIntField(c, FIELD_COLOR_GREEN),
                         env->GetIntField(c, FIELD_COLOR_BLUE), env->GetIntField(c, FIELD_COLOR_ALPHA));
}

inline jobject colorFromNative(JNIEnv* env, mapnik::color const& c) {
    jobject ret = env->AllocObject(CLASS_COLOR);
    env->SetIntField(ret, FIELD_COLOR_RED, c.red());
    env->SetIntField(ret, FIELD_COLOR_GREEN, c.green());
    env->SetIntField(ret, FIELD_COLOR_BLUE, c.blue());
    env->SetIntField(ret, FIELD_COLOR_ALPHA, c.alpha());
    return ret;
}

inline jobject createDatasourceObj(JNIEnv* env, mapnik::datasource_ptr* ds) {
    jobject ret = env->NewObject(CLASS_DATASOURCE, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(ds));
    return ret;
}

inline jobject creatrFeatureSetObj(JNIEnv* env, mapnik::featureset_ptr* featureset) {
    jobject ret = env->NewObject(CLASS_FEATURESET, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(featureset));
    return ret;
}

inline jobject createFeatureObj(JNIEnv* env, mapnik::feature_ptr* feature) {
    jobject ret = env->NewObject(CLASS_FEATURE, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(feature));
    return ret;
}

inline jobject createGeometryObj(JNIEnv* env, mapnik::feature_ptr* feature) {
    jobject ret = env->NewObject(CLASS_GEOMETRY, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(feature));
    return ret;
}

inline jobject createLayerObj(JNIEnv* env, mapnik::layer* layer) {
    jobject ret = env->NewObject(CLASS_LAYER, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(layer));
    return ret;
}

constexpr char const* const image_format_names[]{"webp", "jpeg", "png", "tiff"};
constexpr std::launch threading_modes[]{std::launch::async, std::launch::deferred};

#endif  // _MAPNIK_JNI_GLOBALS_HPP