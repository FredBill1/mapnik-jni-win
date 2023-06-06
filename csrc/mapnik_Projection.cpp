
#include "mapnik_Projection.h"

#include "globals.hpp"

/// -- Projection class
/*
 * Class:     mapnik_Projection
 * Method:    alloc
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Projection_alloc(JNIEnv *env, jclass c, jstring paramsj) {
    PREAMBLE;
    JNIString params(env, paramsj);
    mapnik::projection *prj = new mapnik::projection(params.get());
    return FROM_POINTER(prj);
    TRAILER(0);
}

/*
 * Class:     mapnik_Projection
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete static_cast<mapnik::projection *>(TO_POINTER(ptr));
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Projection
 * Method:    getParams
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Projection_getParams(JNIEnv *env, jobject prjobject) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(prjobject);
    return env->NewStringUTF(prj->params().c_str());
    TRAILER(0);
}

/*
 * Class:     mapnik_Projection
 * Method:    getExpanded
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Projection_getExpanded(JNIEnv *env, jobject prjobject) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(prjobject);
    return env->NewStringUTF(prj->expanded().c_str());
    TRAILER(0);
}

/*
 * Class:     mapnik_Projection
 * Method:    forward
 * Signature: (Lmapnik/Coord;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_forward(JNIEnv *env, jobject prjobject, jobject coord) {
    PREAMBLE;
    if (!coord) return;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(prjobject);
    double x = env->GetDoubleField(coord, FIELD_COORD_X), y = env->GetDoubleField(coord, FIELD_COORD_Y);
    prj->forward(x, y);
    env->SetDoubleField(coord, FIELD_COORD_X, x);
    env->SetDoubleField(coord, FIELD_COORD_Y, y);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Projection
 * Method:    inverse
 * Signature: (Lmapnik/Coord;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_inverse(JNIEnv *env, jobject prjobject, jobject coord) {
    PREAMBLE;
    if (!coord) return;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(prjobject);
    double x = env->GetDoubleField(coord, FIELD_COORD_X), y = env->GetDoubleField(coord, FIELD_COORD_Y);
    prj->inverse(x, y);
    env->SetDoubleField(coord, FIELD_COORD_X, x);
    env->SetDoubleField(coord, FIELD_COORD_Y, y);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Projection
 * Method:    forward
 * Signature: (Lmapnik/Coord;)Lmapnik/Coord;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Projection_forward__Lmapnik_Coord_2(JNIEnv *env, jobject obj, jobject xy_obj) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(obj);
    auto xy = coordToNative(env, xy_obj);
    prj->forward(xy.x, xy.y);
    return coordFromNative(env, xy);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Projection
 * Method:    inverse
 * Signature: (Lmapnik/Coord;)Lmapnik/Coord;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Projection_inverse__Lmapnik_Coord_2(JNIEnv *env, jobject obj, jobject xy_obj) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(obj);
    auto xy = coordToNative(env, xy_obj);
    prj->inverse(xy.x, xy.y);
    return coordFromNative(env, xy);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Projection
 * Method:    forward
 * Signature: (Lmapnik/Box2d;)Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Projection_forward__Lmapnik_Box2d_2(JNIEnv *env, jobject obj, jobject box_obj) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(obj);
    auto box = box2dToNative(env, box_obj);
    prj->forward(box.minx_, box.miny_);
    prj->forward(box.maxx_, box.maxy_);
    return box2dFromNative(env, box);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Projection
 * Method:    inverse
 * Signature: (Lmapnik/Box2d;)Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Projection_inverse__Lmapnik_Box2d_2(JNIEnv *env, jobject obj, jobject box_obj) {
    PREAMBLE;
    mapnik::projection *prj = LOAD_PROJECTION_POINTER(obj);
    auto box = box2dToNative(env, box_obj);
    prj->inverse(box.minx_, box.miny_);
    prj->inverse(box.maxx_, box.maxy_);
    return box2dFromNative(env, box);
    TRAILER(NULL);
}