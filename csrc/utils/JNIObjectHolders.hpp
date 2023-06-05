#ifndef JNI_OBJECT_HOLDERS_HPP_
#define JNI_OBJECT_HOLDERS_HPP_

#include <jni.h>

#include <stdexcept>

class JNIString {
    JNIEnv* env;
    jstring js;
    const char* stringz;
    JNIString(const JNIString&) = delete;
    JNIString& operator=(const JNIString&) = delete;

 public:
    JNIString(JNIEnv* env, jstring js) : env(env), js(js) {
        if (js == NULL) throw std::invalid_argument("String is null");
        stringz = env->GetStringUTFChars(js, NULL);
    }
    ~JNIString() { env->ReleaseStringUTFChars(js, stringz); }
    const char* get() const { return stringz; }
};

class JNIObject {
    JNIEnv* env;
    jobject obj;
    JNIObject(const JNIObject&) = delete;
    JNIObject& operator=(const JNIObject&) = delete;

 public:
    JNIObject(JNIEnv* env, jobject obj) : env(env), obj(obj) {
        if (obj == NULL) throw std::invalid_argument("Object is null");
    }
    ~JNIObject() { env->DeleteLocalRef(obj); }
    jobject get() const { return obj; }
};

class JNIObjectAllowNull {
    JNIEnv* env;
    jobject obj;
    JNIObjectAllowNull(const JNIObjectAllowNull&) = delete;
    JNIObjectAllowNull& operator=(const JNIObjectAllowNull&) = delete;

 public:
    JNIObjectAllowNull(JNIEnv* env, jobject obj) : env(env), obj(obj) {}
    ~JNIObjectAllowNull() {
        if (obj) env->DeleteLocalRef(obj);
    }
    jobject get() const { return obj; }
};

class JNIByteArrayElements {
    JNIEnv* env;
    jbyteArray arr;
    jbyte* ptr;
    JNIByteArrayElements(const JNIByteArrayElements&) = delete;
    JNIByteArrayElements& operator=(const JNIByteArrayElements&) = delete;

 public:
    JNIByteArrayElements(JNIEnv* env, jbyteArray arr) : env(env), arr(arr) {
        if (arr == NULL) throw std::invalid_argument("byte Array is null");
        ptr = env->GetByteArrayElements(arr, NULL);
    }
    ~JNIByteArrayElements() { env->ReleaseByteArrayElements(arr, ptr, JNI_ABORT); }
    const jbyte* data() const { return ptr; }
    jbyte operator[](size_t i) const { return ptr[i]; }
    jsize size() const { return env->GetArrayLength(arr); }
};

class JNILongArrayElements {
    JNIEnv* env;
    jlongArray arr;
    jlong* ptr;
    JNILongArrayElements(const JNILongArrayElements&) = delete;
    JNILongArrayElements& operator=(const JNILongArrayElements&) = delete;

 public:
    JNILongArrayElements(JNIEnv* env, jlongArray arr) : env(env), arr(arr) {
        if (arr == NULL) throw std::invalid_argument("long Array is null");
        ptr = env->GetLongArrayElements(arr, NULL);
    }
    ~JNILongArrayElements() { env->ReleaseLongArrayElements(arr, ptr, JNI_ABORT); }
    const jlong* data() const { return ptr; }
    jlong operator[](size_t i) const { return ptr[i]; }
    jsize size() const { return env->GetArrayLength(arr); }
};

class JNIDoubleArrayElements {
    JNIEnv* env;
    jdoubleArray arr;
    jdouble* ptr;
    JNIDoubleArrayElements(const JNIDoubleArrayElements&) = delete;
    JNIDoubleArrayElements& operator=(const JNIDoubleArrayElements&) = delete;

 public:
    JNIDoubleArrayElements(JNIEnv* env, jdoubleArray arr) : env(env), arr(arr) {
        if (arr == NULL) throw std::invalid_argument("double Array is null");
        ptr = env->GetDoubleArrayElements(arr, NULL);
    }
    ~JNIDoubleArrayElements() { env->ReleaseDoubleArrayElements(arr, ptr, JNI_ABORT); }
    const jdouble* data() const { return ptr; }
    jdouble operator[](size_t i) const { return ptr[i]; }
    jsize size() const { return env->GetArrayLength(arr); }
};

#endif  // JNI_OBJECT_HOLDERS_HPP_