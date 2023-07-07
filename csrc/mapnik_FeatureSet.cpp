#include "geowin_mapnik_FeatureSet.h"
#include "globals.hpp"

/*
 * Class:     mapnik_FeatureSet
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_FeatureSet_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete reinterpret_cast<mapnik::featureset_ptr *>(ptr);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    next
 * Signature: ()Lmapnik/Feature;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_FeatureSet_next(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto featureset = LOAD_FEATURESET_POINTER(obj);
    auto feature = new mapnik::feature_ptr((*featureset)->next());
    if (*feature == nullptr) return NULL;
    return createFeatureObj(env, feature);
    TRAILER(NULL);
}
