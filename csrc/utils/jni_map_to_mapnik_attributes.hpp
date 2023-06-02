#ifndef _UTILS_JNI_MAP_TO_MAPNIK_ATTRIBUTES_HPP
#define _UTILS_JNI_MAP_TO_MAPNIK_ATTRIBUTES_HPP

#include "../globals.hpp"

inline void jni_map_to_mapnik_attributes(JNIEnv* env, jobject mapj, mapnik::attributes& mapnik_attributes) {
    if (mapj == NULL) return;
    jobject setj = env->CallObjectMethod(mapj, METHOD_MAP_ENTRYSET);
    jobject iterj = env->CallObjectMethod(setj, METHOD_SET_ITERATOR);
    while (env->CallBooleanMethod(iterj, METHOD_ITERATOR_HASNEXT)) {
        jobject entryj = env->CallObjectMethod(iterj, METHOD_ITERATOR_NEXT);
        jstring key = (jstring)env->CallObjectMethod(entryj, METHOD_MAP_ENTRY_GETKEY);
        jobject value = env->CallObjectMethod(entryj, METHOD_MAP_ENTRY_GETVALUE);
        const char* keyc = env->GetStringUTFChars(key, NULL);
        if (value == NULL) {
            mapnik_attributes[keyc] = mapnik::value_null();
        } else if (env->IsInstanceOf(value, CLASS_STRING)) {
            const char* valuec = env->GetStringUTFChars((jstring)value, 0);
            mapnik_attributes[keyc] = mapnik::value_unicode_string(valuec);
            env->ReleaseStringUTFChars((jstring)value, valuec);
        } else if (env->IsInstanceOf(value, CLASS_INTEGER)) {
            jint valuei = env->CallIntMethod(value, METHOD_INTEGER_INTVALUE);
            mapnik_attributes[keyc] = mapnik::value_integer(valuei);
        } else if (env->IsInstanceOf(value, CLASS_BOOLEAN)) {
            jboolean valueb = env->CallBooleanMethod(value, METHOD_BOOLEAN_BOOLEANVALUE);
            mapnik_attributes[keyc] = mapnik::value_bool(valueb);
        } else if (env->IsInstanceOf(value, CLASS_LONG)) {
            jlong valuej = env->CallLongMethod(value, METHOD_LONG_LONGVALUE);
            mapnik_attributes[keyc] = mapnik::value_integer(valuej);
        } else if (env->IsInstanceOf(value, CLASS_DOUBLE)) {
            jdouble valued = env->CallDoubleMethod(value, METHOD_DOUBLE_DOUBLEVALUE);
            mapnik_attributes[keyc] = mapnik::value_double(valued);
        }
        env->ReleaseStringUTFChars(key, keyc);
        env->DeleteLocalRef(entryj);
        env->DeleteLocalRef(key);
        if (value != NULL) env->DeleteLocalRef(value);
    }
    env->DeleteLocalRef(setj);
    env->DeleteLocalRef(iterj);
}

#endif  // _UTILS_JNI_MAP_TO_VARIABLES_HPP