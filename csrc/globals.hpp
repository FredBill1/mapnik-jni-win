#ifndef _MAPNIK_JNI_GLOBALS_HPP
#define _MAPNIK_JNI_GLOBALS_HPP

#include <jni.h>

#include <boost/variant/static_visitor.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/attribute_collector.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/geometry/geometry_type.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/mapnik.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/version.hpp>
#include <mapnik/vertex_processor.hpp>

//// -- Globals

// clang-format off
inline bool initialized = false;
inline jclass
	CLASS_NATIVEOBJECT,
	CLASS_MAP,
	CLASS_LAYER,
	CLASS_DATASOURCE,
	CLASS_DATASOURCE_CACHE,
	CLASS_FEATURE_TYPE_STYLE,
	CLASS_PROJECTION,
	CLASS_QUERY,
	CLASS_FEATURESET,
	CLASS_GEOMETRY,
	CLASS_IMAGE;
inline jclass
	CLASS_STRING,
	CLASS_DOUBLE,
	CLASS_INTEGER,
	CLASS_BOOLEAN,
	CLASS_LONG,
	CLASS_HASHSET,
	CLASS_PARAMETERS,
	CLASS_BOX2D,
	CLASS_COLOR,
	CLASS_COORD,
	CLASS_LAYERDESCRIPTOR,
	CLASS_ATTRIBUTEDESCRIPTOR;
inline jmethodID
	CTOR_NATIVEOBJECT,
	CTOR_PARAMETERS,
	METHOD_PARAMETERS_SET_BOOLEAN,
	METHOD_PARAMETERS_SET_STRING,
	METHOD_PARAMETERS_SET_INT,
	METHOD_PARAMETERS_SET_LONG,
	METHOD_PARAMETERS_SET_DOUBLE,
	METHOD_PARAMETERS_COPY_TO_NATIVE,
	METHOD_DOUBLE_VALUEOF,
	METHOD_INTEGER_VALUEOF,
	METHOD_BOOLEAN_VALUEOF,
	METHOD_LONG_VALUEOF,
	CTOR_HASHSET,
	METHOD_HASHSET_ADD,
	CTOR_LAYERDESCRIPTOR,
	METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR,
	CTOR_ATTRIBUTEDESCRIPTOR;
inline jfieldID
	FIELD_PTR,
	FIELD_BOX2D_MINX,
	FIELD_BOX2D_MINY,
	FIELD_BOX2D_MAXX,
	FIELD_BOX2D_MAXY,

	FIELD_COLOR_RED,
	FIELD_COLOR_GREEN,
	FIELD_COLOR_BLUE,
	FIELD_COLOR_ALPHA,

	FIELD_COORD_X,
	FIELD_COORD_Y,

	FIELD_LAYERDESCRIPTOR_NAME,
	FIELD_LAYERDESCRIPTOR_ENCODING,

	FIELD_ATTRIBUTEDESCRIPTOR_NAME,
	FIELD_ATTRIBUTEDESCRIPTOR_TYPE,
	FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY,
	FIELD_ATTRIBUTEDESCRIPTOR_SIZE,
	FIELD_ATTRIBUTEDESCRIPTOR_PRECISION,

	FIELD_FEATURESET_FEATURE_PTR;
// clang-format on

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
#define LOAD_OBJECT_POINTER(object) ((void*)(ASSERT_LONG_POINTER(env->GetLongField(object, FIELD_PTR))))
#define LOAD_MAP_POINTER(object) (static_cast<mapnik::Map*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_LAYER_POINTER(object) (static_cast<mapnik::layer*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_DATASOURCE_POINTER(object) (static_cast<mapnik::datasource_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_TYPE_STYLE_POINTER(object) (static_cast<mapnik::feature_type_style*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_PROJECTION_POINTER(object) (static_cast<mapnik::projection*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_QUERY_POINTER(object) (static_cast<mapnik::query*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURESET_POINTER(object) (static_cast<mapnik::featureset_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_POINTER(object) \
    (static_cast<mapnik::feature_ptr*>(TO_POINTER(env->GetLongField(object, FIELD_FEATURESET_FEATURE_PTR))))
#define LOAD_GEOMETRY_POINTER(object) (static_cast<mapnik::geometry::geometry<double>*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_IMAGE_POINTER(object) (static_cast<mapnik::image_rgba8*>(LOAD_OBJECT_POINTER(object)))

class refjavastring {
    JNIEnv* env;
    jstring js;
    jboolean isCopy;

 public:
    const char* stringz;
    bool isnull;

    refjavastring(JNIEnv* env, jstring js) {
        this->env = env;
        this->js = js;
        if (js) {
            stringz = env->GetStringUTFChars(js, &isCopy);
            isnull = false;
        } else {
            stringz = "";
            isnull = true;
            isCopy = 0;
        }
    }
    ~refjavastring() {
        if (isCopy) { env->ReleaseStringUTFChars(js, stringz); }
    }
};

inline jobject box2dFromNative(JNIEnv* env, mapnik::box2d<double> const& box) {
    jobject ret = env->AllocObject(CLASS_BOX2D);
    env->SetDoubleField(ret, FIELD_BOX2D_MINX, box.minx());
    env->SetDoubleField(ret, FIELD_BOX2D_MINY, box.miny());
    env->SetDoubleField(ret, FIELD_BOX2D_MAXX, box.maxx());
    env->SetDoubleField(ret, FIELD_BOX2D_MAXY, box.maxy());
    return ret;
}

inline mapnik::box2d<double> box2dToNative(JNIEnv* env, jobject box) {
    mapnik::box2d<double> ret(env->GetDoubleField(box, FIELD_BOX2D_MINX), env->GetDoubleField(box, FIELD_BOX2D_MINY),
                              env->GetDoubleField(box, FIELD_BOX2D_MAXX), env->GetDoubleField(box, FIELD_BOX2D_MAXY));
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

#endif  // _MAPNIK_JNI_GLOBALS_HPP