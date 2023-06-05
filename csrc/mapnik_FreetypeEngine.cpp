#include "mapnik_FreetypeEngine.h"

#include "globals.hpp"
/*
 * Class:     mapnik_FreetypeEngine
 * Method:    registerFont
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_registerFont(JNIEnv* env, jclass c, jstring filenamej) {
    PREAMBLE;

    if (!filenamej) { return false; }

    JNIString filename(env, filenamej);
    return mapnik::freetype_engine::register_font(filename.get());
    TRAILER(false);
}

/*
 * Class:     mapnik_FreetypeEngine
 * Method:    registerFonts
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_registerFonts(JNIEnv* env, jclass c, jstring dirj,
                                                                    jboolean recurse) {
    PREAMBLE;

    if (!dirj) { return false; }

    JNIString dir(env, dirj);
    return mapnik::freetype_engine::register_fonts(dir.get(), (bool)recurse);
    TRAILER(false);
}

/*
 * Class:     mapnik_FreetypeEngine
 * Method:    isFontFile
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_isFontFile(JNIEnv* env, jclass c, jstring filenamej) {
    PREAMBLE;

    if (!filenamej) { return false; }

    JNIString filename(env, filenamej);
    return mapnik::freetype_engine::is_font_file(filename.get());
    TRAILER(false);
}

/*
 * Class:     mapnik_FreetypeEngine
 * Method:    faceNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_FreetypeEngine_faceNames(JNIEnv* env, jclass c) {
    PREAMBLE;
    std::vector<std::string> names(mapnik::freetype_engine::face_names());
    jobjectArray ary = env->NewObjectArray(names.size(), CLASS_STRING, (jobject)0);

    for (unsigned i = 0; i < names.size(); i++) {
        env->SetObjectArrayElement(ary, i, env->NewStringUTF(names[i].c_str()));
    }

    return ary;
    TRAILER(0);
}
