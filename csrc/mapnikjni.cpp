#include <jni.h>
#include <mapnik/mapnik.hpp>
#include <mapnik/version.hpp>
#include <mapnik/map.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/attribute_collector.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/geometry/geometry_type.hpp>
#include <mapnik/projection.hpp>
#include <mapnik/vertex_processor.hpp>

#include <boost/variant/static_visitor.hpp>

#include "mapnikjni.h"
#include "globals.cpp"
#include "class_parameters.cpp"
#include "class_map.cpp"
#include "class_layer.cpp"
#include "class_datasource.cpp"
#include "class_datasourcecache.cpp"
#include "class_featuretypestyle.cpp"
#include "class_projection.cpp"
#include "class_query.cpp"
#include "class_featureset.cpp"
#include "class_geometry.cpp"
#include "class_image.cpp"
#include "class_renderer.cpp"
#include "class_freetypeengine.cpp"

/// -- Mapnik class
/*
 * Class:     mapnik_Mapnik
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_nativeInit
( JNIEnv* env, jclass c ) {
  PREAMBLE;

  if ( initialized ) {
    return;
  }

  if ( init_ids ( env ) ) {
    initialized = true;
  }

  TRAILER_VOID;
}

/*
* Class:     mapnik_Mapnik
* Method:    initLogger
* Signature: (Ljava/lang/String;I)V
*/
JNIEXPORT void JNICALL Java_mapnik_Mapnik_initLogger
( JNIEnv* env, jclass c, jstring sj, jint level ) {
  PREAMBLE;
  refjavastring path ( env, sj );
#if MAPNIK_VERSION >= 200200
  mapnik::logger::instance().use_file ( path.stringz );
  mapnik::logger::instance().set_severity ( static_cast<mapnik::logger::severity_type> ( level ) );
#else
  mapnik::logger::instance()->use_file ( path.stringz );
  mapnik::logger::instance()->set_severity ( static_cast<mapnik::logger::severity_type> ( level ) );
#endif
  TRAILER_VOID;
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledFontsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledFontsDir
( JNIEnv* env, jclass ) {
#ifndef MAPNIK_FONTS_DIR
  return 0;
#else
  return env->NewStringUTF ( MAPNIK_FONTS_DIR );
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    getInstalledInputPluginsDir
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Mapnik_getInstalledInputPluginsDir
( JNIEnv* env, jclass ) {
#ifndef MAPNIK_INPUT_PLUGINS_DIR
  return 0;
#else
  return env->NewStringUTF ( MAPNIK_INPUT_PLUGINS_DIR );
#endif
}

/*
 * Class:     mapnik_Mapnik
 * Method:    isThreadSafe
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Mapnik_isThreadSafe
( JNIEnv* env, jclass c ) {
#ifdef MAPNIK_THREADSAFE
  return 1;
#else
  return 0;
#endif
}
