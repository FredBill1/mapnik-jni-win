#include "geowin_mapnik_Query.h"

#include "globals.hpp"
/// -- Query class
/*
 * Class:     mapnik_Query
 * Method:    alloc
 * Signature: (Lgeowin/mapnik/Box2d;DDD)J
 */
JNIEXPORT jlong JNICALL Java_geowin_mapnik_Query_alloc(JNIEnv *env, jclass c, jobject bboxj, jdouble resx, jdouble resy,
                                                jdouble scale_denominator) {
    PREAMBLE;
    mapnik::box2d<double> bbox(box2dToNative(env, bboxj));
    mapnik::query *q = new mapnik::query(bbox, mapnik::query::resolution_type(resx, resy), scale_denominator);

    return FROM_POINTER(q);
    TRAILER(0);
}

/*
 * Class:     mapnik_Query
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_Query_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete static_cast<mapnik::query *>(TO_POINTER(ptr));
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Query
 * Method:    addPropertyName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_Query_addPropertyName(JNIEnv *env, jobject qobj, jstring namej) {
    PREAMBLE;
    mapnik::query *q = LOAD_QUERY_POINTER(qobj);
    JNIString name(env, namej);
    q->add_property_name(name.get());
    TRAILER_VOID;
}
