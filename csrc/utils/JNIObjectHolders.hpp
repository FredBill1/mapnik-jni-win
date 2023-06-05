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

#endif  // JNI_OBJECT_HOLDERS_HPP_