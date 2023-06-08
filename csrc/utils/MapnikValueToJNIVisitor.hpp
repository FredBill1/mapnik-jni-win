#ifndef MAPNIK_VALUE_TO_JNI_VISITOR_HPP
#define MAPNIK_VALUE_TO_JNI_VISITOR_HPP

#include <jni.h>

#include <mapnik/value/types.hpp>

class MapnikValueToJNIVisitor {
    JNIEnv *env;

 public:
    MapnikValueToJNIVisitor(JNIEnv *env) : env(env) {}
    jobject operator()(mapnik::value_null) const { return NULL; }
    jobject operator()(const char *val) const { return env->NewStringUTF(val); }
    jobject operator()(std::string const &val) const { return (*this)(val.c_str()); }
    jobject operator()(mapnik::value_unicode_string const &val) const {
        std::string result;
        mapnik::to_utf8(val, result);
        return (*this)(result);
    }
    jobject operator()(mapnik::value_bool const &val) const {
        return env->CallStaticObjectMethod(CLASS_BOOLEAN, METHOD_BOOLEAN_VALUEOF, val);
    }
    jobject operator()(mapnik::value_integer const &val) const {
        return env->CallStaticObjectMethod(CLASS_LONG, METHOD_LONG_VALUEOF, val);
    }
    jobject operator()(mapnik::value_double const &val) const {
        return env->CallStaticObjectMethod(CLASS_DOUBLE, METHOD_DOUBLE_VALUEOF, val);
    }
};

#endif  // MAPNIK_VALUE_TO_JNI_VISITOR_HPP