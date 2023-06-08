#include "mapnik_Datasource.h"

#include "globals.hpp"
#include "utils/MapnikValueToJNIVisitor.hpp"
#include "utils/jni_map_to_mapnik_attributes.hpp"
/*
 * Class:     mapnik_Datasource
 * Method:    alloc
 * Signature: (Ljava/util/Map;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Datasource_alloc(JNIEnv* env, jobject, jobject paramsj) {
    PREAMBLE;
    mapnik::parameters params;
    jni_map_to_mapnik_attributes(env, paramsj, params);
    auto ds = new mapnik::datasource_ptr(mapnik::datasource_cache::instance().create(params));
    return FROM_POINTER(ds);
    TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Datasource_dealloc(JNIEnv* env, jobject, jlong ptr) {
    PREAMBLE;
    mapnik::datasource_ptr* dspinned = static_cast<mapnik::datasource_ptr*>(TO_POINTER(ptr));
    delete dspinned;
    TRAILER_VOID;
}

/*
 * Class:     mapnik_Datasource
 * Method:    parameters
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_parameters(JNIEnv* env, jobject obj) {
    PREAMBLE;
    auto ds = LOAD_DATASOURCE_POINTER(obj);
    auto& params = (*ds)->params();
    MapnikValueToJNIVisitor visitor(env);
    jobject out = env->NewObject(CLASS_HASHMAP, CTOR_HASHMAP);
    for (auto&& [k, v] : params) {
        JNIObject key(env, env->NewStringUTF(k.c_str()));
        JNIObject value(env, mapnik::util::apply_visitor(visitor, v));
        JNIObjectAllowNull(env, env->CallObjectMethod(out, METHOD_HASHMAP_PUT, key.get(), value.get()));
    }
    return out;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_Datasource
 * Method:    getType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Datasource_getType(JNIEnv* env, jobject dsobj) {
    PREAMBLE;
    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    return (*dsp)->type();
    TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    getEnvelope
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getEnvelope(JNIEnv* env, jobject dsobj) {
    PREAMBLE;
    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    return box2dFromNative(env, (*dsp)->envelope());
    TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    features
 * Signature: (Lmapnik/Query;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_features(JNIEnv* env, jobject dsobj, jobject queryobj) {
    PREAMBLE;
    if (!queryobj) return 0;

    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    mapnik::query* query = LOAD_QUERY_POINTER(queryobj);

    mapnik::featureset_ptr* fspinned;

    mapnik::featureset_ptr fs = (*dsp)->features(*query);
    fspinned = new mapnik::featureset_ptr();
    fs.swap(*fspinned);

    return creatrFeatureSetObj(env, fspinned);

    TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    featuresAtPoint
 * Signature: (Lmapnik/Coord;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_featuresAtPoint(JNIEnv* env, jobject dsobj, jobject ptobj) {
    PREAMBLE;
    if (!ptobj) return 0;

    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    mapnik::coord2d pt(env->GetDoubleField(ptobj, FIELD_COORD_X), env->GetDoubleField(ptobj, FIELD_COORD_Y));

    mapnik::featureset_ptr fs = (*dsp)->features_at_point(pt);
    mapnik::featureset_ptr* fspinned = new mapnik::featureset_ptr(fs);

    return creatrFeatureSetObj(env, fspinned);

    TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    getDescriptor
 * Signature: ()Lmapnik/LayerDescriptor;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getDescriptor(JNIEnv* env, jobject dsobj) {
    PREAMBLE;
    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    mapnik::layer_descriptor desc = (*dsp)->get_descriptor();

    jobject ret = env->NewObject(CLASS_LAYERDESCRIPTOR, CTOR_LAYERDESCRIPTOR);
    {
        JNIObject name(env, env->NewStringUTF(desc.get_name().c_str()));
        JNIObject encoding(env, env->NewStringUTF(desc.get_encoding().c_str()));
        env->SetObjectField(ret, FIELD_LAYERDESCRIPTOR_NAME, name.get());
        env->SetObjectField(ret, FIELD_LAYERDESCRIPTOR_ENCODING, encoding.get());
    }

    std::vector<mapnik::attribute_descriptor>& descriptors(desc.get_descriptors());
    for (std::vector<mapnik::attribute_descriptor>::iterator iter = descriptors.begin(); iter != descriptors.end();
         iter++) {
        JNIObject attr(env, env->NewObject(CLASS_ATTRIBUTEDESCRIPTOR, CTOR_ATTRIBUTEDESCRIPTOR));
        JNIObject name(env, env->NewStringUTF(iter->get_name().c_str()));
        env->SetObjectField(attr.get(), FIELD_ATTRIBUTEDESCRIPTOR_NAME, name.get());
        env->SetIntField(attr.get(), FIELD_ATTRIBUTEDESCRIPTOR_TYPE, iter->get_type());
        env->SetBooleanField(attr.get(), FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY, (jboolean)iter->is_primary_key());
        env->SetIntField(attr.get(), FIELD_ATTRIBUTEDESCRIPTOR_SIZE, iter->get_size());
        env->SetIntField(attr.get(), FIELD_ATTRIBUTEDESCRIPTOR_PRECISION, iter->get_precision());

        env->CallVoidMethod(ret, METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR, attr.get());
    }

    return ret;
    TRAILER(0);
}
