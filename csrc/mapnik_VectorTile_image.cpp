#include "mapnik_VectorTile.h"
//
#include "globals.hpp"
/*
 * Class:     mapnik_VectorTile
 * Method:    addImageImpl
 * Signature: (JLjava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_addImageImpl(JNIEnv *env, jobject obj, jlong image_ptr, jstring name,
                                                           jint image_scaling, jint image_format) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto image = reinterpret_cast<mapnik::image_rgba8 *>(image_ptr);
    JNIString layer_name(env, name);
    auto ds = std::make_shared<mapnik::memory_datasource>(mapnik::parameters());
    mapnik::raster_ptr ras = std::make_shared<mapnik::raster>(tile->extent(), *image, 1.0);
    mapnik::context_ptr ctx = std::make_shared<mapnik::context_type>();
    mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx, 1));
    feature->set_raster(ras);
    ds->push(feature);
    ds->envelope();  // can be removed later, currently doesn't work with out this.
    ds->set_envelope(tile->extent());
    // create map object
    mapnik::Map map(tile->tile_size(), tile->tile_size(), "+init=epsg:3857");
    mapnik::layer lyr(layer_name.get(), "+init=epsg:3857");
    lyr.set_datasource(ds);
    map.add_layer(lyr);

    mapnik::vector_tile_impl::processor ren(map);
    ren.set_scaling_method(static_cast<mapnik::scaling_method_e>(image_scaling));
    ren.set_image_format(image_format_names[image_format]);
    ren.update_tile(*tile);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    addImageBuffer
 * Signature: ([BLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_addImageBuffer(JNIEnv *env, jobject obj, jbyteArray buffer,
                                                             jstring name) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    JNIString layer_name(env, name);
    auto data = env->GetByteArrayElements(buffer, NULL);
    auto size = env->GetArrayLength(buffer);
    mapnik::vector_tile_impl::add_image_buffer_as_tile_layer(*tile, layer_name.get(),
                                                             reinterpret_cast<const char *>(data), size);
    env->ReleaseByteArrayElements(buffer, data, JNI_ABORT);
    TRAILER_VOID;
}
