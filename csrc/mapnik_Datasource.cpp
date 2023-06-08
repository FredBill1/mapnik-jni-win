#include "mapnik_Datasource.h"

#include "globals.hpp"
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

class translate_parameter_visitor : public boost::static_visitor<> {
    JNIEnv* env;
    jobject paramobject;
    jstring key;

 public:
    translate_parameter_visitor(JNIEnv* aenv, jobject aparamobject, jstring akey)
        : env(aenv), paramobject(aparamobject), key(akey) {}

    void operator()(mapnik::value_integer value) const {
#ifdef BIGINT
        env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_LONG, key, (jlong)value);
#else
        env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_INT, key, (jint)value);
#endif
    }

    void operator()(mapnik::value_bool value) const {
        env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_BOOLEAN, key, (jboolean)value);
    }

    void operator()(mapnik::value_double value) const {
        env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_DOUBLE, key, (jdouble)value);
    }

    void operator()(std::string const& value) const {
        JNIObject jstr(env, env->NewStringUTF(value.c_str()));
        env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_STRING, key, jstr.get());
    }

    void operator()(mapnik::value_null const& value) const {
        // FIXME
    }
};

/*
 * Class:     mapnik_Datasource
 * Method:    getParameters
 * Signature: ()Lmapnik/Parameters;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getParameters(JNIEnv* env, jobject dsobj) {
    PREAMBLE;
    mapnik::datasource_ptr* dsp = LOAD_DATASOURCE_POINTER(dsobj);
    const mapnik::parameters& params((*dsp)->params());

    jobject paramobject = env->NewObject(CLASS_PARAMETERS, CTOR_PARAMETERS);

    for (mapnik::param_map::const_iterator iter = params.begin(); iter != params.end(); iter++) {
        JNIObject key(env, env->NewStringUTF(iter->first.c_str()));
        translate_parameter_visitor visitor(env, paramobject, (jstring)key.get());
        // TODO - The call to visit() does not compile on MSVC 2015 (error C2783).
        // The compiler cannot deduce the __formal type(?) in:
        // const T &mapnik::util::variant<mapnik::value_null,
        //                                mapnik::value_integer,
        //                                mapnik::value_double,
        //                                std::string,
        //                                mapnik::value_bool>::get(void) const
        // (ditto for the non-const version)
        // mapnik::value_type::visit(iter->second, visitor);
        // So, the variant<> types are temporarily unrolled here...
        if (iter->second.is<mapnik::value_integer>()) {
            visitor(iter->second.get<mapnik::value_integer>());
        } else if (iter->second.is<mapnik::value_double>()) {
            visitor(iter->second.get<mapnik::value_double>());
        } else if (iter->second.is<std::string>()) {
            visitor(iter->second.get<std::string>());
        } else if (iter->second.is<mapnik::value_bool>()) {
            visitor(iter->second.get<mapnik::value_bool>());
        }
        // else: value_null or unexpected value - ignore
    }

    return paramobject;
    TRAILER(0);
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
