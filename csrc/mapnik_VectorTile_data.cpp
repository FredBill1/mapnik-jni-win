#include "mapnik_VectorTile.h"
//
#include "globals.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    setDataImpl
 * Signature: ([BZZ)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setDataImpl(JNIEnv *env, jobject obj, jbyteArray bufferj,
                                                          jboolean validate, jboolean upgrade) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    if (bufferj == NULL) throw std::exception("buffer is null");
    jsize size = env->GetArrayLength(bufferj);
    if (size == 0) throw std::exception("buffer is empty");
    jbyte *buffer = env->GetByteArrayElements(bufferj, NULL);
    tile->clear();
    mapnik::vector_tile_impl::merge_from_compressed_buffer(*tile, reinterpret_cast<const char *>(buffer), size,
                                                           validate, upgrade);
    env->ReleaseByteArrayElements(bufferj, buffer, JNI_ABORT);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    addData
 * Signature: ([BZZ)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_addDataImpl(JNIEnv *env, jobject obj, jbyteArray bufferj,
                                                          jboolean validate, jboolean upgrade) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    if (bufferj == NULL) throw std::exception("buffer is null");
    jsize size = env->GetArrayLength(bufferj);
    if (size == 0) throw std::exception("buffer is empty");
    jbyte *buffer = env->GetByteArrayElements(bufferj, NULL);
    mapnik::vector_tile_impl::merge_from_compressed_buffer(*tile, reinterpret_cast<const char *>(buffer), size,
                                                           validate, upgrade);
    env->ReleaseByteArrayElements(bufferj, buffer, JNI_ABORT);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getDataImpl
 * Signature: (ZII)[B
 */
JNIEXPORT jbyteArray JNICALL Java_mapnik_VectorTile_getDataImpl(JNIEnv *env, jobject obj, jboolean compress, jint level,
                                                                jint strategy) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    if (!compress) {
        jbyteArray arr = env->NewByteArray(tile->size());
        env->SetByteArrayRegion(arr, 0, tile->size(), reinterpret_cast<const jbyte *>(tile->data()));
        return arr;
    } else {
        std::string compressed;
        mapnik::vector_tile_impl::zlib_compress(tile->data(), tile->size(), compressed, true, level, strategy);
        jbyteArray arr = env->NewByteArray(compressed.size());
        env->SetByteArrayRegion(arr, 0, compressed.size(), reinterpret_cast<const jbyte *>(compressed.data()));
        return arr;
    }
    TRAILER(NULL);
}