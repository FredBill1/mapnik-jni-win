#ifndef MAPNIK_JNI_IDS_HPP
#define MAPNIK_JNI_IDS_HPP

#include <jni.h>

// clang-format off
inline bool initialized = false;
inline jclass
    CLASS_NATIVEOBJECT,
    CLASS_MAP_DEFINITION,
    CLASS_LAYER,
    CLASS_DATASOURCE,
    CLASS_DATASOURCE_CACHE,
    CLASS_FEATURE_TYPE_STYLE,
    CLASS_PROJECTION,
    CLASS_QUERY,
    CLASS_FEATURE,
    CLASS_FEATURESET,
    CLASS_GEOMETRY,
    CLASS_IMAGE,

    CLASS_OBJECT,
    CLASS_OBJECT_ARRAY,
    CLASS_STRING,
    CLASS_DOUBLE,
    CLASS_INTEGER,
    CLASS_BOOLEAN,
    CLASS_LONG,
    CLASS_HASHSET,
    CLASS_MAP,
    CLASS_SET,
    CLASS_HASHMAP,
    CLASS_ITERATOR,
    CLASS_MAP_ENTRY,
    CLASS_PARAMETERS,
    CLASS_BOX2D,
    CLASS_COLOR,
    CLASS_COORD,
    CLASS_LAYERDESCRIPTOR,
    CLASS_ATTRIBUTEDESCRIPTOR,
    CLASS_VECTOR_TILE_INFO,
    CLASS_VECTOR_TILE_INFO_LAYER,
    CLASS_VECTOR_TILE_JSON,
    CLASS_VECTOR_TILE_JSON_LAYER,
    CLASS_VECTOR_TILE_JSON_LAYER_FEATURE,
    CLASS_VECTOR_TILE_NOT_SIMPLE_FEATURE,
    CLASS_VECTOR_TILE_NOT_VALID_FEATURE,
    CLASS_VECTOR_TILE_QUERY_RESULT,
    CLASS_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE,
    CLASS_VECTOR_TILE_QUERY_MANY_RESULT,
    CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE,
    CLASS_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT;

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
    METHOD_DOUBLE_DOUBLEVALUE,
    METHOD_INTEGER_VALUEOF,
    METHOD_INTEGER_INTVALUE,
    METHOD_BOOLEAN_VALUEOF,
    METHOD_BOOLEAN_BOOLEANVALUE,
    METHOD_LONG_VALUEOF,
    METHOD_LONG_LONGVALUE,
    CTOR_HASHSET,
    CTOR_HASHMAP,
    METHOD_HASHMAP_PUT,
    METHOD_HASHSET_ADD,
    METHOD_MAP_ENTRYSET,
    METHOD_SET_ITERATOR,
    METHOD_ITERATOR_HASNEXT,
    METHOD_ITERATOR_NEXT,
    METHOD_MAP_ENTRY_GETKEY,
    METHOD_MAP_ENTRY_GETVALUE,
    CTOR_LAYERDESCRIPTOR,
    METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR,
    CTOR_ATTRIBUTEDESCRIPTOR,
    CTOR_VECTOR_TILE_INFO,
    CTOR_VECTOR_TILE_INFO_LAYER,
    CTOR_VECTOR_TILE_JSON,
    CTOR_VECTOR_TILE_JSON_LAYER,
    CTOR_VECTOR_TILE_JSON_LAYER_FEATURE,
    CTOR_VECTOR_TILE_NOT_SIMPLE_FEATURE,
    CTOR_VECTOR_TILE_NOT_VALID_FEATURE;

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

    FIELD_VECTOR_TILE_INFO_LAYERS,
    FIELD_VECTOR_TILE_INFO_ERRORS,
    FIELD_VECTOR_TILE_INFO_TILE_ERRORS,

    FIELD_VECTOR_TILE_INFO_LAYER_NAME,
    FIELD_VECTOR_TILE_INFO_LAYER_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_POINT_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_LINESTRING_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_POLYGON_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_UNKNOWN_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_RASTER_FEATURES,
    FIELD_VECTOR_TILE_INFO_LAYER_VERSION,
    FIELD_VECTOR_TILE_INFO_LAYER_ERRORS,

    FIELD_VECTOR_TILE_JSON_LAYERS,
    FIELD_VECTOR_TILE_JSON_LAYER_NAME,
    FIELD_VECTOR_TILE_JSON_LAYER_EXTENT,
    FIELD_VECTOR_TILE_JSON_LAYER_VERSION,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURES,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_ID,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_TYPE,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_RASTER,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_PROPERTIES,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY,
    FIELD_VECTOR_TILE_JSON_LAYER_FEATURE_GEOMETRY_TYPE,

    FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_LAYER,
    FIELD_VECTOR_TILE_NOT_SIMPLE_FEATURE_FEATURE_ID,

    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_MESSAGE,
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_LAYER,
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_FEATURE_ID,
    FIELD_VECTOR_TILE_NOT_VALID_FEATURE_GEOJSON,

    FIELD_VECTOR_TILE_QUERY_RESULT_FEATURES,
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_LAYER,
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_DISTANCE,
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_XHIT,
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_YHIT,
    FIELD_VECTOR_TILE_QUERY_RESULT_QUERY_FEATURE_FEATURE,

    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_FEATURES,
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_HITS,
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_LAYER,
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_FEATURE_FEATURE,
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_DISTANCE,
    FIELD_VECTOR_TILE_QUERY_MANY_RESULT_QUERY_HIT_FEATURE_ID;
// clang-format on

void mapnik_jni_init_ids(JNIEnv* env);

#endif  // MAPNIK_JNI_IDS_HPP