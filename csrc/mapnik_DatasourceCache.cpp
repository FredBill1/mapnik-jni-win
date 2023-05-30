#include "jni/mapnik_DatasourceCache.h"

#include "globals.hpp"
//// -- DatasourceCache
/*
 * Class:     mapnik_DatasourceCache
 * Method:    pluginNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_DatasourceCache_pluginNames(JNIEnv *env, jclass c) {
    PREAMBLE;
#if MAPNIK_VERSION >= 200200
    std::vector<std::string> names(mapnik::datasource_cache::instance().plugin_names());
#else
    std::vector<std::string> names(mapnik::datasource_cache::instance()->plugin_names());
#endif
    jobjectArray ary = env->NewObjectArray(names.size(), CLASS_STRING, (jobject)0);

    for (unsigned i = 0; i < names.size(); i++) {
        env->SetObjectArrayElement(ary, i, env->NewStringUTF(names[i].c_str()));
    }

    return ary;
    TRAILER(0);
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    pluginDirectories
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_DatasourceCache_pluginDirectories(JNIEnv *env, jclass c) {
    PREAMBLE;
#if MAPNIK_VERSION >= 200200
    std::string s(mapnik::datasource_cache::instance().plugin_directories());
#else
    std::string s(mapnik::datasource_cache::instance()->plugin_directories());
#endif
    return env->NewStringUTF(s.c_str());
    TRAILER(0);
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    registerDatasources
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_DatasourceCache_registerDatasources(JNIEnv *env, jclass c, jstring sj) {
    PREAMBLE;
    refjavastring path(env, sj);
#if MAPNIK_VERSION >= 200200
    mapnik::datasource_cache::instance().register_datasources(path.stringz);
#else
    mapnik::datasource_cache::instance()->register_datasources(path.stringz);
#endif
    TRAILER_VOID;
}

static void translate_to_mapnik_parameters(JNIEnv *env, jobject javaparams, mapnik::parameters &mapnikparams) {
    if (!javaparams) return;
    env->CallVoidMethod(javaparams, METHOD_PARAMETERS_COPY_TO_NATIVE, (jlong)(&mapnikparams));
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    create
 * Signature: (Lmapnik/Parameters;Z)Lmapnik/Datasource;
 */
JNIEXPORT jobject JNICALL Java_mapnik_DatasourceCache_create(JNIEnv *env, jclass c, jobject paramsmap) {
    PREAMBLE;
    mapnik::parameters params;
    translate_to_mapnik_parameters(env, paramsmap, params);
#if MAPNIK_VERSION >= 200200
    mapnik::datasource_ptr ds = mapnik::datasource_cache::instance().create(params);
#else
    mapnik::datasource_ptr ds = mapnik::datasource_cache::instance()->create(params);
#endif
    if (!ds) return 0;

    mapnik::datasource_ptr *dspinned = new mapnik::datasource_ptr(ds);
    jobject ret = env->NewObject(CLASS_DATASOURCE, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(dspinned));
    return ret;
    TRAILER(0);
}
