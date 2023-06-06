#include "mapnik_ProjTransform.h"

#include "globals.hpp"

/*
 * Class:     mapnik_ProjTransform
 * Method:    alloc
 * Signature: (Lmapnik/Projection;Lmapnik/Projection;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_ProjTransform_alloc(JNIEnv *env, jclass, jobject src_obj, jobject dst_obj) {
    PREAMBLE;
    auto src = LOAD_PROJECTION_POINTER(src_obj);
    auto dst = LOAD_PROJECTION_POINTER(dst_obj);
    auto tr = new mapnik::proj_transform(*src, *dst);
    return FROM_POINTER(tr);
    TRAILER(0);
}

/*
 * Class:     mapnik_ProjTransform
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_ProjTransform_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete reinterpret_cast<mapnik::proj_transform *>(ptr);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_ProjTransform
 * Method:    forward
 * Signature: (Lmapnik/Box2d;)Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_ProjTransform_forward__Lmapnik_Box2d_2(JNIEnv *env, jobject obj,
                                                                             jobject box_obj) {
    PREAMBLE;
    auto tr = LOAD_PROJ_TRANSFORM_POINTER(obj);
    auto box = box2dToNative(env, box_obj);
    if (!tr->forward(box)) {
        std::ostringstream s;
        s << "Failed to forward project " << box << " using project transfrom `" << tr->definition() << '`';
        throw_runtime_exception(env, s.str().c_str());
        return NULL;
    }
    return box2dFromNative(env, box);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_ProjTransform
 * Method:    forward
 * Signature: (DD)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_ProjTransform_forward__DD(JNIEnv *env, jobject obj, jdouble x, jdouble y) {
    PREAMBLE;
    auto tr = LOAD_PROJ_TRANSFORM_POINTER(obj);
    double xyz[3] = {x, y, 0};
    if (!tr->forward(xyz[0], xyz[1], xyz[2])) {
        std::ostringstream s;
        s << "Failed to forward project " << x << ", " << y << " using project transfrom `" << tr->definition() << '`';
        throw_runtime_exception(env, s.str().c_str());
        return NULL;
    }
    jdoubleArray arr = env->NewDoubleArray(2);
    env->SetDoubleArrayRegion(arr, 0, 2, xyz);
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_ProjTransform
 * Method:    backward
 * Signature: (Lmapnik/Box2d;)Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_ProjTransform_backward__Lmapnik_Box2d_2(JNIEnv *env, jobject obj,
                                                                              jobject box_obj) {
    PREAMBLE;
    auto tr = LOAD_PROJ_TRANSFORM_POINTER(obj);
    auto box = box2dToNative(env, box_obj);
    if (!tr->backward(box)) {
        std::ostringstream s;
        s << "Failed to backward project " << box << " using project transfrom `" << tr->definition() << '`';
        throw_runtime_exception(env, s.str().c_str());
        return NULL;
    }
    return box2dFromNative(env, box);
    TRAILER(NULL);
}

/*
 * Class:     mapnik_ProjTransform
 * Method:    backward
 * Signature: (DD)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_ProjTransform_backward__DD(JNIEnv *env, jobject obj, jdouble x, jdouble y) {
    PREAMBLE;
    auto tr = LOAD_PROJ_TRANSFORM_POINTER(obj);
    double xyz[3] = {x, y, 0};
    if (!tr->backward(xyz[0], xyz[1], xyz[2])) {
        std::ostringstream s;
        s << "Failed to backward project " << x << ", " << y << " using project transfrom `" << tr->definition() << '`';
        throw_runtime_exception(env, s.str().c_str());
        return NULL;
    }
    jdoubleArray arr = env->NewDoubleArray(2);
    env->SetDoubleArrayRegion(arr, 0, 2, xyz);
    return arr;
    TRAILER(NULL);
}