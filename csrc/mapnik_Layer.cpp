#include "mapnik_Layer.h"

#include "globals.hpp"

/*
 * Class:     mapnik_Layer
 * Method:    alloc
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Layer_alloc(JNIEnv *env, jclass c, jstring namej, jstring srsj) {
    PREAMBLE;
    JNIString name(env, namej);
    JNIString srs(env, srsj);

    mapnik::layer *layer = new mapnik::layer(name.get(), srs.get());
    return FROM_POINTER(layer);
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    if (ptr) { delete static_cast<mapnik::layer *>(TO_POINTER(ptr)); }
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getName(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return env->NewStringUTF(layer->name().c_str());
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setName(JNIEnv *env, jobject layerobj, jstring sj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    JNIString s(env, sj);
    layer->set_name(s.get());
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getSrs(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return env->NewStringUTF(layer->srs().c_str());
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setSrs
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setSrs(JNIEnv *env, jobject layerobj, jstring sj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    JNIString s(env, sj);
    layer->set_srs(s.get());
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getStyles
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_Layer_getStyles(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    std::vector<std::string> &styles(layer->styles());

    jobjectArray ary = env->NewObjectArray(styles.size(), CLASS_STRING, (jobject)0);

    for (unsigned i = 0; i < styles.size(); i++) {
        JNIObject style(env, env->NewStringUTF(styles[i].c_str()));
        env->SetObjectArrayElement(ary, i, style.get());
    }

    return ary;
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setStyles
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setStyles(JNIEnv *env, jobject layerobj, jobjectArray ary) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    std::vector<std::string> &styles(layer->styles());
    styles.clear();

    if (ary) {
        jsize size = env->GetArrayLength(ary);
        for (jsize i = 0; i < size; i++) {
            JNIObject element(env, env->GetObjectArrayElement(ary, i));
            JNIString s(env, static_cast<jstring>(element.get()));
            styles.push_back(s.get());
        }
    }
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getMinZoom
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMinZoom(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return layer->minimum_scale_denominator();
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setMinZoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMinZoom(JNIEnv *env, jobject layerobj, jdouble z) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_minimum_scale_denominator(z);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getMaxZoom
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMaxZoom(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return layer->maximum_scale_denominator();
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setMaxZoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMaxZoom(JNIEnv *env, jobject layerobj, jdouble z) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_maximum_scale_denominator(z);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    isActive
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isActive(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return (jboolean)layer->active();
    TRAILER(false);
}

/*
 * Class:     mapnik_Layer
 * Method:    setActive
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setActive(JNIEnv *env, jobject layerobj, jboolean b) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_active((bool)b);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    isQueryable
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isQueryable(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return (jboolean)layer->queryable();
    TRAILER(false);
}

/*
 * Class:     mapnik_Layer
 * Method:    setQueryable
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setQueryable(JNIEnv *env, jobject layerobj, jboolean b) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_queryable((bool)b);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    isVisible
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isVisible(JNIEnv *env, jobject layerobj, jdouble scale) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return layer->visible(scale);
    TRAILER(false);
}

/*
 * Class:     mapnik_Layer
 * Method:    isClearLabelCache
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isClearLabelCache(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return (jboolean)layer->clear_label_cache();
    TRAILER(false);
}

/*
 * Class:     mapnik_Layer
 * Method:    setClearLabelCache
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setClearLabelCache(JNIEnv *env, jobject layerobj, jboolean b) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_clear_label_cache((bool)b);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    isCacheFeatures
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isCacheFeatures(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    return (jboolean)layer->cache_features();
    TRAILER(false);
}

/*
 * Class:     mapnik_Layer
 * Method:    setCacheFeatures
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setCacheFeatures(JNIEnv *env, jobject layerobj, jboolean b) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    layer->set_cache_features((bool)b);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getDatasource
 * Signature: ()Lmapnik/Datasource;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Layer_getDatasource(JNIEnv *env, jobject layerobj) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);
    mapnik::datasource_ptr ds = layer->datasource();
    if (!ds) return 0;

    mapnik::datasource_ptr *dspinned = new mapnik::datasource_ptr(ds);
    return createDatasourceObj(env, dspinned);
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setDatasource
 * Signature: (Lmapnik/Datasource;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setDatasource(JNIEnv *env, jobject layerobj, jobject dsobject) {
    PREAMBLE;
    mapnik::layer *layer = LOAD_LAYER_POINTER(layerobj);

    if (!dsobject) {
        layer->set_datasource(mapnik::datasource_ptr());
        return;
    }

    mapnik::datasource_ptr *dspinned =
        static_cast<mapnik::datasource_ptr *>(TO_POINTER(env->GetLongField(dsobject, FIELD_PTR)));
    layer->set_datasource(*dspinned);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getMinimumScaleDenominator
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMinimumScaleDenominator(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto layer = LOAD_LAYER_POINTER(obj);
    return layer->minimum_scale_denominator();
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setMinimumScaleDenominator
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMinimumScaleDenominator(JNIEnv *env, jobject obj,
                                                                    jdouble minimum_scale_denom) {
    PREAMBLE;
    auto layer = LOAD_LAYER_POINTER(obj);
    layer->set_minimum_scale_denominator(minimum_scale_denom);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Layer
 * Method:    getMaximumScaleDenominator
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMaximumScaleDenominator(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto layer = LOAD_LAYER_POINTER(obj);
    return layer->maximum_scale_denominator();
    TRAILER(0);
}

/*
 * Class:     mapnik_Layer
 * Method:    setMaximumScaleDenominator
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMaximumScaleDenominator(JNIEnv *env, jobject obj,
                                                                    jdouble maximum_scale_denom) {
    PREAMBLE;
    auto layer = LOAD_LAYER_POINTER(obj);
    layer->set_maximum_scale_denominator(maximum_scale_denom);
    TRAILER_VOID;
}
