#include "globals.hpp"
/// -- FeatureSet class
/*
 * Class:     mapnik_FeatureSet
 * Method:    dealloc
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_mapnik_FeatureSet_dealloc(JNIEnv* env, jobject, jlong fsptr, jlong fptr) {
    if (fsptr) delete static_cast<mapnik::featureset_ptr*>(TO_POINTER(fsptr));
    if (fptr) delete static_cast<mapnik::feature_ptr*>(TO_POINTER(fptr));
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    _next
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FeatureSet__1next(JNIEnv* env, jobject fsobj) {
    PREAMBLE;

    mapnik::featureset_ptr* fsp = LOAD_FEATURESET_POINTER(fsobj);
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (fp) delete fp;

    mapnik::feature_ptr next = (*fsp)->next();
    if (!next) {
        // End
        env->SetLongField(fsobj, FIELD_FEATURESET_FEATURE_PTR, 0);
        return false;
    } else {
        // Another
        fp = new mapnik::feature_ptr(next);
        env->SetLongField(fsobj, FIELD_FEATURESET_FEATURE_PTR, FROM_POINTER(fp));
        return true;
    }

    TRAILER(false);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    _loadGeometries
 * Signature: ()[Lmapnik/Geometry;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_FeatureSet__1loadGeometries(JNIEnv* env, jobject fsobj) {
    PREAMBLE;
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (!fp) {
        throw_runtime_exception(env, "No feature loaded");
        return 0;
    }

    // Mapnik 3.x only holds one geometry<> per feature_impl.
    // (In Mapnik 2.x this used to be a variable-length vector of geometries.)
    unsigned count = 1;
    jobjectArray ret = env->NewObjectArray(count, CLASS_GEOMETRY, (jobject)0);
    for (unsigned index = 0; index < count; index++) {
        mapnik::geometry::geometry<double> const& g((*fp)->get_geometry());
        jobject gobject = env->NewObject(CLASS_GEOMETRY, CTOR_NATIVEOBJECT);
        env->SetLongField(gobject, FIELD_PTR, FROM_POINTER(&g));
        env->SetObjectArrayElement(ret, index, gobject);
    }

    return ret;
    TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_FeatureSet_getId(JNIEnv* env, jobject fsobj) {
    PREAMBLE;
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (!fp) {
        throw_runtime_exception(env, "No feature loaded");
        return 0;
    }

    return (*fp)->id();
    TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getEnvelope
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getEnvelope(JNIEnv* env, jobject fsobj) {
    PREAMBLE;
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (!fp) {
        throw_runtime_exception(env, "No feature loaded");
        return 0;
    }

    return box2dFromNative(env, (*fp)->envelope());
    TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getPropertyNames
 * Signature: ()Ljava/util/Set;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getPropertyNames(JNIEnv* env, jobject fsobj) {
    PREAMBLE;
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (!fp) {
        throw_runtime_exception(env, "No feature loaded");
        return 0;
    }

    jobject ret = env->NewObject(CLASS_HASHSET, CTOR_HASHSET);
    mapnik::feature_impl::iterator itr = (*fp)->begin();
    mapnik::feature_impl::iterator end = (*fp)->end();
    for (; itr != end; ++itr) {
        std::string const& name(std::get<0>(*itr));
        env->CallBooleanMethod(ret, METHOD_HASHSET_ADD, env->NewStringUTF(name.c_str()));
    }

    return ret;
    TRAILER(0);
}

class value_to_java : public boost::static_visitor<jobject> {
    JNIEnv* env;

 public:
    value_to_java(JNIEnv* aenv) : env(aenv) {}

    jobject operator()(mapnik::value_integer value) const {
#ifdef BIGINT
        return env->CallStaticObjectMethod(CLASS_LONG, METHOD_LONG_VALUEOF, value);
#else
        return env->CallStaticObjectMethod(CLASS_INTEGER, METHOD_INTEGER_VALUEOF, value);
#endif
    }

    jobject operator()(mapnik::value_bool value) const {
        return env->CallStaticObjectMethod(CLASS_BOOLEAN, METHOD_BOOLEAN_VALUEOF, value);
    }

    jobject operator()(mapnik::value_double value) const {
        return env->CallStaticObjectMethod(CLASS_DOUBLE, METHOD_DOUBLE_VALUEOF, value);
    }

    jobject operator()(std::string const& value) const { return env->NewStringUTF(value.c_str()); }

    jobject operator()(mapnik::value_unicode_string const& value) const {
        return env->NewString(reinterpret_cast<const jchar*>(value.getBuffer()), value.length());
    }

    jobject operator()(mapnik::value_null const& value) const { return 0; }
};

/*
 * Class:     mapnik_FeatureSet
 * Method:    getProperty
 * Signature: (Ljava/lang/String;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getProperty(JNIEnv* env, jobject fsobj, jstring namej) {
    PREAMBLE;
    mapnik::feature_ptr* fp = LOAD_FEATURE_POINTER(fsobj);
    if (!fp) {
        throw_runtime_exception(env, "No feature loaded");
        return 0;
    }

    refjavastring name(env, namej);

    // Convert the value
    mapnik::value_type const& value = (*fp)->get(name.stringz);
    return mapnik::value_type::visit(value, value_to_java(env));
    TRAILER(0);
}
