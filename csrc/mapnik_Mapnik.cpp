#include "mapnik_Mapnik.h"

#include "globals.hpp"

static void init_class(JNIEnv* env, const char* name, jclass& classref) {
    classref = (jclass)env->NewGlobalRef(env->FindClass(name));
    if (!classref) {
        std::string message("Unable to initialize class: ");
        throw std::runtime_error(message + name);
    }
}

static jfieldID lookup_field(JNIEnv* env, jclass c, const char* name, const char* sig) {
    jfieldID ret = env->GetFieldID(c, name, sig);
    if (!ret) {
        std::string message("Unable to find field: ");
        throw std::runtime_error(message + name + ", Signature: " + sig);
    }
    return ret;
}

static jmethodID lookup_method(JNIEnv* env, jclass c, const char* name, const char* sig) {
    jmethodID ret = env->GetMethodID(c, name, sig);
    if (!ret) {
        std::string message("Unable to find method: ");
        throw std::runtime_error(message + name + ", Signature: " + sig);
    }
    return ret;
}

static jmethodID lookup_static_method(JNIEnv* env, jclass c, const char* name, const char* sig) {
    jmethodID ret = env->GetStaticMethodID(c, name, sig);
    if (!ret) {
        std::string message("Unable to find method: ");
        throw std::runtime_error(message + name + ", Signature: " + sig);
    }
    return ret;
}

bool init_ids(JNIEnv* env) {
    mapnik::setup();

    // clang-format off

    // Load NativeObject classes
    init_class(env, "mapnik/NativeObject", CLASS_NATIVEOBJECT);
    init_class(env, "mapnik/MapDefinition", CLASS_MAP_DEFINITION);
    init_class(env, "mapnik/Datasource", CLASS_DATASOURCE);
    init_class(env, "mapnik/DatasourceCache", CLASS_DATASOURCE_CACHE);
    init_class(env, "mapnik/Layer", CLASS_LAYER);
    init_class(env, "mapnik/FeatureTypeStyle", CLASS_FEATURE_TYPE_STYLE);
    init_class(env, "mapnik/Projection", CLASS_PROJECTION);
    init_class(env, "mapnik/Query", CLASS_QUERY);
    init_class(env, "mapnik/FeatureSet", CLASS_FEATURESET);
    init_class(env, "mapnik/Geometry", CLASS_GEOMETRY);
    init_class(env, "mapnik/Image", CLASS_IMAGE);

    // Ptr
    CTOR_NATIVEOBJECT = lookup_method(env, CLASS_NATIVEOBJECT, "<init>", "()V");
    FIELD_PTR = lookup_field(env, CLASS_NATIVEOBJECT, "ptr", "J");

    // FeatureSet
    FIELD_FEATURESET_FEATURE_PTR = lookup_field(env, CLASS_FEATURESET, "feature_ptr", "J");

    // String
    init_class(env, "java/lang/String", CLASS_STRING);

    // Integer
    init_class(env, "java/lang/Integer", CLASS_INTEGER);
    METHOD_INTEGER_VALUEOF = lookup_static_method(env, CLASS_INTEGER, "valueOf", "(I)Ljava/lang/Integer;");
    METHOD_INTEGER_INTVALUE = lookup_method(env, CLASS_INTEGER, "intValue", "()I");

    init_class(env, "java/lang/Boolean", CLASS_BOOLEAN);
    METHOD_BOOLEAN_VALUEOF = lookup_static_method(env, CLASS_BOOLEAN, "valueOf", "(Z)Ljava/lang/Boolean;");
    METHOD_BOOLEAN_BOOLEANVALUE = lookup_method(env, CLASS_BOOLEAN, "booleanValue", "()Z");

    init_class(env, "java/lang/Long", CLASS_LONG);
    METHOD_LONG_VALUEOF = lookup_static_method(env, CLASS_LONG, "valueOf", "(J)Ljava/lang/Long;");
    METHOD_LONG_LONGVALUE = lookup_method(env, CLASS_LONG, "longValue", "()J");

    // Double
    init_class(env, "java/lang/Double", CLASS_DOUBLE);
    METHOD_DOUBLE_VALUEOF = lookup_static_method(env, CLASS_DOUBLE, "valueOf", "(D)Ljava/lang/Double;");
    METHOD_DOUBLE_DOUBLEVALUE = lookup_method(env, CLASS_DOUBLE, "doubleValue", "()D");

    // Parameters
    init_class(env, "mapnik/Parameters", CLASS_PARAMETERS);
    CTOR_PARAMETERS = lookup_method(env, CLASS_PARAMETERS, "<init>", "()V");
    METHOD_PARAMETERS_SET_STRING = lookup_method(env, CLASS_PARAMETERS, "setString", "(Ljava/lang/String;Ljava/lang/String;)V");
    METHOD_PARAMETERS_SET_BOOLEAN = lookup_method(env, CLASS_PARAMETERS, "setBool", "(Ljava/lang/String;Z)V");
    METHOD_PARAMETERS_SET_INT = lookup_method(env, CLASS_PARAMETERS, "setInt", "(Ljava/lang/String;I)V");
    METHOD_PARAMETERS_SET_LONG = lookup_method(env, CLASS_PARAMETERS, "setLong", "(Ljava/lang/String;J)V");
    METHOD_PARAMETERS_SET_DOUBLE = lookup_method(env, CLASS_PARAMETERS, "setDouble", "(Ljava/lang/String;D)V");
    METHOD_PARAMETERS_COPY_TO_NATIVE = lookup_method(env, CLASS_PARAMETERS, "copyToNative", "(J)V");

    // HashSet
    init_class(env, "java/util/HashSet", CLASS_HASHSET);
    CTOR_HASHSET = lookup_method(env, CLASS_HASHSET, "<init>", "()V");
    METHOD_HASHSET_ADD = lookup_method(env, CLASS_HASHSET, "add", "(Ljava/lang/Object;)Z");

    // Map
    init_class(env, "java/util/Map", CLASS_MAP);
    init_class(env, "java/util/Set", CLASS_SET);
    init_class(env, "java/util/Iterator", CLASS_ITERATOR);
    init_class(env, "java/util/Map$Entry", CLASS_MAP_ENTRY);
    METHOD_MAP_ENTRYSET = lookup_method(env, CLASS_MAP, "entrySet", "()Ljava/util/Set;");
    METHOD_SET_ITERATOR = lookup_method(env, CLASS_SET, "iterator", "()Ljava/util/Iterator;");
    METHOD_ITERATOR_HASNEXT = lookup_method(env, CLASS_ITERATOR, "hasNext", "()Z");
    METHOD_ITERATOR_NEXT = lookup_method(env, CLASS_ITERATOR, "next", "()Ljava/lang/Object;");
    METHOD_MAP_ENTRY_GETKEY = lookup_method(env, CLASS_MAP_ENTRY, "getKey", "()Ljava/lang/Object;");
    METHOD_MAP_ENTRY_GETVALUE = lookup_method(env, CLASS_MAP_ENTRY, "getValue", "()Ljava/lang/Object;");

    // Box2d
    init_class(env, "mapnik/Box2d", CLASS_BOX2D);
    FIELD_BOX2D_MINX = lookup_field(env, CLASS_BOX2D, "minx", "D");
    FIELD_BOX2D_MINY = lookup_field(env, CLASS_BOX2D, "miny", "D");
    FIELD_BOX2D_MAXX = lookup_field(env, CLASS_BOX2D, "maxx", "D");
    FIELD_BOX2D_MAXY = lookup_field(env, CLASS_BOX2D, "maxy", "D");

    // Color
    init_class(env, "mapnik/Color", CLASS_COLOR);
    FIELD_COLOR_RED = lookup_field(env, CLASS_COLOR, "red", "I");
    FIELD_COLOR_GREEN = lookup_field(env, CLASS_COLOR, "green", "I");
    FIELD_COLOR_BLUE = lookup_field(env, CLASS_COLOR, "blue", "I");
    FIELD_COLOR_ALPHA = lookup_field(env, CLASS_COLOR, "alpha", "I");

    // Coord
    init_class(env, "mapnik/Coord", CLASS_COORD);
    FIELD_COORD_X = lookup_field(env, CLASS_COORD, "x", "D");
    FIELD_COORD_Y = lookup_field(env, CLASS_COORD, "y", "D");

    // LayerDescriptor
    init_class(env, "mapnik/LayerDescriptor", CLASS_LAYERDESCRIPTOR);
    CTOR_LAYERDESCRIPTOR = lookup_method(env, CLASS_LAYERDESCRIPTOR, "<init>", "()V");
    FIELD_LAYERDESCRIPTOR_NAME = lookup_field(env, CLASS_LAYERDESCRIPTOR, "name", "Ljava/lang/String;");
    FIELD_LAYERDESCRIPTOR_ENCODING = lookup_field(env, CLASS_LAYERDESCRIPTOR, "encoding", "Ljava/lang/String;");
    METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR = lookup_method(env, CLASS_LAYERDESCRIPTOR, "addDescriptor", "(Lmapnik/AttributeDescriptor;)V");

    // AttributeDescriptor
    init_class(env, "mapnik/AttributeDescriptor", CLASS_ATTRIBUTEDESCRIPTOR);
    CTOR_ATTRIBUTEDESCRIPTOR = lookup_method(env, CLASS_ATTRIBUTEDESCRIPTOR, "<init>", "()V");
    FIELD_ATTRIBUTEDESCRIPTOR_NAME = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "name", "Ljava/lang/String;");
    FIELD_ATTRIBUTEDESCRIPTOR_TYPE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "type", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "primaryKey", "Z");
    FIELD_ATTRIBUTEDESCRIPTOR_SIZE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "size", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRECISION = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "precision", "I");

    // VectorTile
    init_class(env, "mapnik/VectorTile$Info", CLASS_VECTOR_TILE_INFO);
    CTOR_VECTOR_TILE_INFO = lookup_method(env, CLASS_VECTOR_TILE_INFO, "<init>", "()V");
    FIELD_VECTOR_TILE_INFO_LAYERS      = lookup_field(env, CLASS_VECTOR_TILE_INFO,       "layers",      "[Lmapnik/VectorTile$Info$Layer;");
    FIELD_VECTOR_TILE_INFO_ERRORS      = lookup_field(env, CLASS_VECTOR_TILE_INFO,       "errors",      "Z");
    FIELD_VECTOR_TILE_INFO_TILE_ERRORS = lookup_field(env, CLASS_VECTOR_TILE_INFO, "tile_errors", "[Ljava/lang/String;");

    init_class(env, "mapnik/VectorTile$Info$Layer", CLASS_VECTOR_TILE_INFO_LAYER);
    CTOR_VECTOR_TILE_INFO_LAYER = lookup_method(env, CLASS_VECTOR_TILE_INFO_LAYER, "<init>", "()V");
    FIELD_VECTOR_TILE_INFO_LAYER_NAME                = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "name",                "Ljava/lang/String;");
    FIELD_VECTOR_TILE_INFO_LAYER_FEATURES            = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "features",            "J");
    FIELD_VECTOR_TILE_INFO_LAYER_POINT_FEATURES      = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "point_features",      "J");
    FIELD_VECTOR_TILE_INFO_LAYER_LINESTRING_FEATURES = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "linestring_features", "J");
    FIELD_VECTOR_TILE_INFO_LAYER_POLYGON_FEATURES    = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "polygon_features",    "J");
    FIELD_VECTOR_TILE_INFO_LAYER_UNKNOWN_FEATURES    = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "unknown_features",    "J");
    FIELD_VECTOR_TILE_INFO_LAYER_RASTER_FEATURES     = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "raster_features",     "J");
    FIELD_VECTOR_TILE_INFO_LAYER_VERSION             = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "version",             "I");
    FIELD_VECTOR_TILE_INFO_LAYER_ERRORS              = lookup_field(env, CLASS_VECTOR_TILE_INFO_LAYER, "errors",              "[Ljava/lang/String;");

    // clang-format on
    return true;
}

/// -- Mapnik class
/*
 * Class:     mapnik_Mapnik
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_nativeInit(JNIEnv* env, jclass c) {
    PREAMBLE;

    if (initialized) { return; }

    if (init_ids(env)) { initialized = true; }

    TRAILER_VOID;
}

/*
 * Class:     mapnik_Mapnik
 * Method:    initLogger
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_initLogger(JNIEnv* env, jclass c, jstring sj, jint level) {
    PREAMBLE;
    refjavastring path(env, sj);
#if MAPNIK_VERSION >= 200200
    mapnik::logger::instance().use_file(path.stringz);
    mapnik::logger::instance().set_severity(static_cast<mapnik::logger::severity_type>(level));
#else
    mapnik::logger::instance()->use_file(path.stringz);
    mapnik::logger::instance()->set_severity(static_cast<mapnik::logger::severity_type>(level));
#endif
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledFontsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledFontsDir(JNIEnv* env, jclass) {
#ifndef MAPNIK_FONTS_DIR
    return 0;
#else
    return env->NewStringUTF(MAPNIK_FONTS_DIR);
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledInputPluginsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledInputPluginsDir(JNIEnv* env, jclass) {
#ifndef MAPNIK_INPUT_PLUGINS_DIR
    return 0;
#else
    return env->NewStringUTF(MAPNIK_INPUT_PLUGINS_DIR);
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    isThreadSafe
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_isThreadSafe(JNIEnv* env, jclass c) {
#ifdef MAPNIK_THREADSAFE
    return 1;
#else
    return 0;
#endif
}
