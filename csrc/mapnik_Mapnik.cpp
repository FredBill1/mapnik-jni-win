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

    // Load NativeObject classes
    init_class(env, "mapnik/NativeObject", CLASS_NATIVEOBJECT);
    init_class(env, "mapnik/MapDefinition", CLASS_MAP);
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

    init_class(env, "java/lang/Boolean", CLASS_BOOLEAN);
    METHOD_BOOLEAN_VALUEOF = lookup_static_method(env, CLASS_BOOLEAN, "valueOf", "(Z)Ljava/lang/Boolean;");

    init_class(env, "java/lang/Long", CLASS_LONG);
    METHOD_LONG_VALUEOF = lookup_static_method(env, CLASS_LONG, "valueOf", "(J)Ljava/lang/Long;");

    // Double
    init_class(env, "java/lang/Double", CLASS_DOUBLE);
    METHOD_DOUBLE_VALUEOF = lookup_static_method(env, CLASS_DOUBLE, "valueOf", "(D)Ljava/lang/Double;");

    // Parameters
    init_class(env, "mapnik/Parameters", CLASS_PARAMETERS);
    CTOR_PARAMETERS = lookup_method(env, CLASS_PARAMETERS, "<init>", "()V");
    METHOD_PARAMETERS_SET_STRING =
        lookup_method(env, CLASS_PARAMETERS, "setString", "(Ljava/lang/String;Ljava/lang/String;)V");
    METHOD_PARAMETERS_SET_BOOLEAN = lookup_method(env, CLASS_PARAMETERS, "setBool", "(Ljava/lang/String;Z)V");
    METHOD_PARAMETERS_SET_INT = lookup_method(env, CLASS_PARAMETERS, "setInt", "(Ljava/lang/String;I)V");
    METHOD_PARAMETERS_SET_LONG = lookup_method(env, CLASS_PARAMETERS, "setLong", "(Ljava/lang/String;J)V");
    METHOD_PARAMETERS_SET_DOUBLE = lookup_method(env, CLASS_PARAMETERS, "setDouble", "(Ljava/lang/String;D)V");
    METHOD_PARAMETERS_COPY_TO_NATIVE = lookup_method(env, CLASS_PARAMETERS, "copyToNative", "(J)V");

    // HashSet
    init_class(env, "java/util/HashSet", CLASS_HASHSET);
    CTOR_HASHSET = lookup_method(env, CLASS_HASHSET, "<init>", "()V");
    METHOD_HASHSET_ADD = lookup_method(env, CLASS_HASHSET, "add", "(Ljava/lang/Object;)Z");

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
    METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR =
        lookup_method(env, CLASS_LAYERDESCRIPTOR, "addDescriptor", "(Lmapnik/AttributeDescriptor;)V");

    // AttributeDescriptor
    init_class(env, "mapnik/AttributeDescriptor", CLASS_ATTRIBUTEDESCRIPTOR);
    CTOR_ATTRIBUTEDESCRIPTOR = lookup_method(env, CLASS_ATTRIBUTEDESCRIPTOR, "<init>", "()V");
    FIELD_ATTRIBUTEDESCRIPTOR_NAME = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "name", "Ljava/lang/String;");
    FIELD_ATTRIBUTEDESCRIPTOR_TYPE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "type", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "primaryKey", "Z");
    FIELD_ATTRIBUTEDESCRIPTOR_SIZE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "size", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRECISION = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "precision", "I");

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
