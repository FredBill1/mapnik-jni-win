#ifndef _UTILS_JNI_MAP_TO_MAPNIK_ATTRIBUTES_HPP
#define _UTILS_JNI_MAP_TO_MAPNIK_ATTRIBUTES_HPP

#include "../globals.hpp"

inline void jni_map_to_mapnik_attributes(JNIEnv* env, jobject mapj, mapnik::attributes& mapnik_attributes) {
    if (mapj == NULL) return;
    JNIObject setj(env, env->CallObjectMethod(mapj, METHOD_MAP_ENTRYSET));
    JNIObject iterj(env, env->CallObjectMethod(setj.get(), METHOD_SET_ITERATOR));
    while (env->CallBooleanMethod(iterj.get(), METHOD_ITERATOR_HASNEXT)) {
        JNIObject entryj(env, env->CallObjectMethod(iterj.get(), METHOD_ITERATOR_NEXT));
        JNIObject key(env, env->CallObjectMethod(entryj.get(), METHOD_MAP_ENTRY_GETKEY));
        JNIObject value(env, env->CallObjectMethod(entryj.get(), METHOD_MAP_ENTRY_GETVALUE));
        std::string keyc = JNIString(env, static_cast<jstring>(key.get())).get();
        if (value.get() == NULL) {
            mapnik_attributes[keyc] = mapnik::value_null();
        } else if (env->IsInstanceOf(value.get(), CLASS_STRING)) {
            mapnik_attributes[keyc] =
                mapnik::value_unicode_string(JNIString(env, static_cast<jstring>(value.get())).get());
        } else if (env->IsInstanceOf(value.get(), CLASS_INTEGER)) {
            jint valuei = env->CallIntMethod(value.get(), METHOD_INTEGER_INTVALUE);
            mapnik_attributes[keyc] = mapnik::value_integer(valuei);
        } else if (env->IsInstanceOf(value.get(), CLASS_BOOLEAN)) {
            jboolean valueb = env->CallBooleanMethod(value.get(), METHOD_BOOLEAN_BOOLEANVALUE);
            mapnik_attributes[keyc] = mapnik::value_bool(valueb);
        } else if (env->IsInstanceOf(value.get(), CLASS_LONG)) {
            jlong valuej = env->CallLongMethod(value.get(), METHOD_LONG_LONGVALUE);
            mapnik_attributes[keyc] = mapnik::value_integer(valuej);
        } else if (env->IsInstanceOf(value.get(), CLASS_DOUBLE)) {
            jdouble valued = env->CallDoubleMethod(value.get(), METHOD_DOUBLE_DOUBLEVALUE);
            mapnik_attributes[keyc] = mapnik::value_double(valued);
        } else {
            throw std::exception(
                "Unknown type of value in `mapnik_attributes`, the value must be a `String`, `Integer`, `Boolean`, "
                "`Long`, `Double` or `null`");
        }
    }
}

#endif  // _UTILS_JNI_MAP_TO_VARIABLES_HPP