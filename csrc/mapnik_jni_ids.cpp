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

void mapnik_jni_init_ids(JNIEnv* env) {
    // clang-format off

    // Load NativeObject classes
    init_class(env, "geowin/mapnik/NativeObject", CLASS_NATIVEOBJECT);
    init_class(env, "geowin/mapnik/MapDefinition", CLASS_MAP_DEFINITION);
    init_class(env, "geowin/mapnik/Datasource", CLASS_DATASOURCE);
    init_class(env, "geowin/mapnik/Layer", CLASS_LAYER);
    init_class(env, "geowin/mapnik/FeatureTypeStyle", CLASS_FEATURE_TYPE_STYLE);
    init_class(env, "geowin/mapnik/Projection", CLASS_PROJECTION);
    init_class(env, "geowin/mapnik/Query", CLASS_QUERY);
    init_class(env, "geowin/mapnik/FeatureSet", CLASS_FEATURESET);
    init_class(env, "geowin/mapnik/Feature", CLASS_FEATURE);
    init_class(env, "geowin/mapnik/Geometry", CLASS_GEOMETRY);
    init_class(env, "geowin/mapnik/Image", CLASS_IMAGE);

    // Ptr
    CTOR_NATIVEOBJECT = lookup_method(env, CLASS_NATIVEOBJECT, "<init>", "()V");
    FIELD_PTR = lookup_field(env, CLASS_NATIVEOBJECT, "ptr", "J");

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

    init_class(env, "java/lang/Object", CLASS_OBJECT);
    init_class(env, "[Ljava/lang/Object;", CLASS_OBJECT_ARRAY);

    // Double
    init_class(env, "java/lang/Double", CLASS_DOUBLE);
    METHOD_DOUBLE_VALUEOF = lookup_static_method(env, CLASS_DOUBLE, "valueOf", "(D)Ljava/lang/Double;");
    METHOD_DOUBLE_DOUBLEVALUE = lookup_method(env, CLASS_DOUBLE, "doubleValue", "()D");

    // HashSet
    init_class(env, "java/util/HashSet", CLASS_HASHSET);
    CTOR_HASHSET = lookup_method(env, CLASS_HASHSET, "<init>", "()V");
    METHOD_HASHSET_ADD = lookup_method(env, CLASS_HASHSET, "add", "(Ljava/lang/Object;)Z");

    // HashMap
    init_class(env, "java/util/HashMap", CLASS_HASHMAP);
    CTOR_HASHMAP = lookup_method(env, CLASS_HASHMAP, "<init>", "()V");
    METHOD_HASHMAP_PUT = lookup_method(env, CLASS_HASHMAP, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

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
    init_class(env, "geowin/mapnik/Box2d", CLASS_BOX2D);
    FIELD_BOX2D_MINX = lookup_field(env, CLASS_BOX2D, "minx", "D");
    FIELD_BOX2D_MINY = lookup_field(env, CLASS_BOX2D, "miny", "D");
    FIELD_BOX2D_MAXX = lookup_field(env, CLASS_BOX2D, "maxx", "D");
    FIELD_BOX2D_MAXY = lookup_field(env, CLASS_BOX2D, "maxy", "D");

    // Color
    init_class(env, "geowin/mapnik/Color", CLASS_COLOR);
    FIELD_COLOR_RED = lookup_field(env, CLASS_COLOR, "red", "I");
    FIELD_COLOR_GREEN = lookup_field(env, CLASS_COLOR, "green", "I");
    FIELD_COLOR_BLUE = lookup_field(env, CLASS_COLOR, "blue", "I");
    FIELD_COLOR_ALPHA = lookup_field(env, CLASS_COLOR, "alpha", "I");

    // Coord
    init_class(env, "geowin/mapnik/Coord", CLASS_COORD);
    FIELD_COORD_X = lookup_field(env, CLASS_COORD, "x", "D");
    FIELD_COORD_Y = lookup_field(env, CLASS_COORD, "y", "D");

    // LayerDescriptor
    init_class(env, "geowin/mapnik/LayerDescriptor", CLASS_LAYERDESCRIPTOR);
    CTOR_LAYERDESCRIPTOR = lookup_method(env, CLASS_LAYERDESCRIPTOR, "<init>", "()V");
    FIELD_LAYERDESCRIPTOR_NAME = lookup_field(env, CLASS_LAYERDESCRIPTOR, "name", "Ljava/lang/String;");
    FIELD_LAYERDESCRIPTOR_ENCODING = lookup_field(env, CLASS_LAYERDESCRIPTOR, "encoding", "Ljava/lang/String;");
    METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR = lookup_method(env, CLASS_LAYERDESCRIPTOR, "addDescriptor", "(Lgeowin/mapnik/AttributeDescriptor;)V");

    // AttributeDescriptor
    init_class(env, "geowin/mapnik/AttributeDescriptor", CLASS_ATTRIBUTEDESCRIPTOR);
    CTOR_ATTRIBUTEDESCRIPTOR = lookup_method(env, CLASS_ATTRIBUTEDESCRIPTOR, "<init>", "()V");
    FIELD_ATTRIBUTEDESCRIPTOR_NAME = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "name", "Ljava/lang/String;");
    FIELD_ATTRIBUTEDESCRIPTOR_TYPE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "type", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "primaryKey", "Z");
    FIELD_ATTRIBUTEDESCRIPTOR_SIZE = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "size", "I");
    FIELD_ATTRIBUTEDESCRIPTOR_PRECISION = lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "precision", "I");

    // VectorTile
    init_class(env, "geowin/mapnik/VectorTile$Info", CLASS_VECTOR_TILE_INFO);
    CTOR_VECTOR_TILE_INFO = lookup_method(env, CLASS_VECTOR_TILE_INFO, "<init>", "()V");
    FIELD_VECTOR_TILE_INFO_LAYERS      = lookup_field(env, CLASS_VECTOR_TILE_INFO,       "layers",      "[Lgeowin/mapnik/VectorTile$Info$Layer;");
    FIELD_VECTOR_TILE_INFO_ERRORS      = lookup_field(env, CLASS_VECTOR_TILE_INFO,       "errors",      "Z");
    FIELD_VECTOR_TILE_INFO_TILE_ERRORS = lookup_field(env, CLASS_VECTOR_TILE_INFO, "tile_errors", "[Ljava/lang/String;");

    init_class(env, "geowin/mapnik/VectorTile$Info$Layer", CLASS_VECTOR_TILE_INFO_LAYER);
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

    init_class(env, "geowin/mapnik/VectorTile$JSON", CLASS_VECTOR_TILE_JSON);
    CTOR_VECTOR_TILE_JSON = lookup_method(env, CLASS_VECTOR_TILE_JSON, "<init>", "()V");
    FIELD_VECTOR_TILE_JSON_LAYERS = lookup_field(env, CLASS_VECTOR_TILE_JSON, "layers", "[Lgeowin/mapnik/VectorTile$JSON$Layer;");

    init_class(env, "geowin/mapnik/VectorTile$JSON$Layer", CLASS_VECTOR_TILE_JSON_LAYER);
    CTOR_VECTOR_TILE_JSON_LAYER = lookup_method(env, CLASS_VECTOR_TILE_JSON_LAYER, "<init>", "()V");
    FIELD_VECTOR_TILE_JSON_LAYER_NAME     = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER, "name",     "Ljava/lang/String;");
    FIELD_VECTOR_TILE_JSON_LAYER_EXTENT   = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER, "extent",   "I");
    FIELD_VECTOR_TILE_JSON_LAYER_VERSION  = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER, "version",  "I");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURES = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER, "features", "[Lgeowin/mapnik/VectorTile$JSON$Layer$Feature;");

    init_class(env, "geowin/mapnik/VectorTile$JSON$Layer$Feature", CLASS_VECTOR_TILE_JSON_LAYER_FEATURE);
    CTOR_VECTOR_TILE_JSON_LAYER_FEATURE = lookup_method(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "<init>", "()V");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_ID            = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "id",            "J");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_TYPE          = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "type",          "I");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_RASTER        = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "raster",        "[B");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_PROPERTIES    = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "properties",    "Ljava/util/Map;");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY      = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "geometry",      "[Ljava/lang/Object;");
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY_TYPE = lookup_field(env, CLASS_VECTOR_TILE_JSON_LAYER_FEATURE, "geometry_type", "Ljava/lang/String;");

    init_class(env, "geowin/mapnik/VectorTile$NotSimpleFeature", CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE);
    CTOR_VECTOR_TILE_NOT_SIMPLE_FEATURE = lookup_method(env, CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE, "<init>", "()V");
    FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_LAYER      = lookup_field(env, CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE, "layer",      "Ljava/lang/String;");
    FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_FEATURE_ID = lookup_field(env, CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE, "feature_id", "J");

    init_class(env, "geowin/mapnik/VectorTile$NotValidFeature", CLASS_VECTOR_TILE_NOT_VALID_FEATURE);
    CTOR_VECTOR_TILE_NOT_VALID_FEATURE = lookup_method(env, CLASS_VECTOR_TILE_NOT_VALID_FEATURE, "<init>", "()V");
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_MESSAGE    = lookup_field(env, CLASS_VECTOR_TILE_NOT_VALID_FEATURE, "message",    "Ljava/lang/String;");
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_LAYER      = lookup_field(env, CLASS_VECTOR_TILE_NOT_VALID_FEATURE, "layer",      "Ljava/lang/String;");
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_FEATURE_ID = lookup_field(env, CLASS_VECTOR_TILE_NOT_VALID_FEATURE, "feature_id", "J");
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_GEOJSON    = lookup_field(env, CLASS_VECTOR_TILE_NOT_VALID_FEATURE, "geojson",    "Ljava/lang/String;");

    init_class(env, "geowin/mapnik/VectorTile$QueryResult", CLASS_VECTOR_TILE_QUERY_RESULT);
    FIELD_VECTOR_TILE_QUERY_RESULT_FEATURES = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT, "features", "[Lgeowin/mapnik/VectorTile$QueryResult$QueryFeature;");

    init_class(env, "geowin/mapnik/VectorTile$QueryResult$QueryFeature", CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE);
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_LAYER    = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, "layer",    "Ljava/lang/String;");
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_DISTANCE = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, "distance", "D");
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_XHIT     = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, "x_hit",    "D");
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_YHIT     = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, "y_hit",    "D");
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_FEATURE  = lookup_field(env, CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE, "feature",  "Lgeowin/mapnik/Feature;");

    init_class(env, "geowin/mapnik/VectorTile$QueryManyResult", CLASS_VECTOR_TILE_QUERY_MANY_RESULT);
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_FEATURES = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT, "features", "[Lgeowin/mapnik/VectorTile$QueryManyResult$QueryFeature;");
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_HITS     = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT, "hits",     "[[Lgeowin/mapnik/VectorTile$QueryManyResult$QueryHit;");

    init_class(env, "geowin/mapnik/VectorTile$QueryManyResult$QueryFeature", CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE);
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_LAYER   = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE, "layer",   "Ljava/lang/String;");
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_FEATURE = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE, "feature", "Lgeowin/mapnik/Feature;");

    init_class(env, "geowin/mapnik/VectorTile$QueryManyResult$QueryHit", CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT);
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_DISTANCE   = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT, "distance", "D");
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_FEATURE_ID = lookup_field(env, CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT, "feature_id", "I");
    // clang-format on
}
