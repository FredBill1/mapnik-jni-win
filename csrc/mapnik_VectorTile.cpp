#include "mapnik_VectorTile.h"

#include "globals.hpp"
#include "utils/jni_map_to_mapnik_attributes.hpp"

/*
 * Class:     mapnik_VectorTile
 * Method:    alloc
 * Signature: (JJJII)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_VectorTile_alloc(JNIEnv *env, jclass, jlong x, jlong y, jlong z, jint tileSize,
                                                     jint bufferSize) {
    PREAMBLE;
    auto t = new mapnik::vector_tile_impl::merc_tile(x, y, z, tileSize, bufferSize);
    return FROM_POINTER(t);
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_dealloc(JNIEnv *env, jobject, jlong ptr) {
    PREAMBLE;
    delete static_cast<mapnik::vector_tile_impl::merc_tile *>(TO_POINTER(ptr));
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileX
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileX(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->x();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileX
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileX(JNIEnv *env, jobject obj, jint x) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->x(x);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileY
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileY(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->y();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileY
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileY(JNIEnv *env, jobject obj, jint y) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->y(y);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileZ
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileZ(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->z();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileZ
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileZ(JNIEnv *env, jobject obj, jint z) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->z(z);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getTileSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getTileSize(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->tile_size();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setTileSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setTileSize(JNIEnv *env, jobject obj, jint tile_size) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->tile_size(tile_size);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_VectorTile_getBufferSize(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->buffer_size();
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    setBufferSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_setBufferSize(JNIEnv *env, jobject obj, jint buffer_size) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->buffer_size(buffer_size);
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
 * Method:    addGeoJSONImpl
 * Signature: (Ljava/lang/String;Ljava/lang/String;DDZZIZ)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_addGeoJSONImpl(JNIEnv *env, jobject obj, jstring geojsonj, jstring namej,
                                                             jdouble area_threshold, jdouble simplify_distance,
                                                             jboolean strictly_simple, jboolean multi_polygon_union,
                                                             jint fill_type, jboolean process_all_rings) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    if (geojsonj == NULL) throw std::exception("geojson is null");
    auto geojson_string = env->GetStringUTFChars(geojsonj, 0);
    if (namej == NULL) throw std::exception("layer name is null");
    auto geojson_name = env->GetStringUTFChars(namej, 0);
    mapnik::Map map(tile->tile_size(), tile->tile_size(), "+init=epsg:3857");
    mapnik::parameters p;
    p["type"] = "geojson";
    p["inline"] = geojson_string;
    mapnik::layer lyr(geojson_name, "+init=epsg:4326");
    env->ReleaseStringUTFChars(geojsonj, geojson_string);
    env->ReleaseStringUTFChars(namej, geojson_name);
    lyr.set_datasource(mapnik::datasource_cache::instance().create(p));
    map.add_layer(lyr);

    mapnik::vector_tile_impl::processor ren(map);
    ren.set_area_threshold(area_threshold);
    ren.set_simplify_distance(simplify_distance);
    ren.set_strictly_simple(strictly_simple);
    ren.set_multi_polygon_union(multi_polygon_union);
    ren.set_fill_type(static_cast<mapnik::vector_tile_impl::polygon_fill_type>(fill_type));
    ren.set_process_all_rings(process_all_rings);
    ren.update_tile(*tile);
    TRAILER_VOID;
}

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
    if (name == NULL) throw std::exception("layer name is null");
    auto layer_name = env->GetStringUTFChars(name, 0);
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
    mapnik::layer lyr(layer_name, "+init=epsg:3857");
    env->ReleaseStringUTFChars(name, layer_name);
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
    if (name == NULL) throw std::exception("layer name is null");
    auto layer_name = env->GetStringUTFChars(name, 0);
    auto data = env->GetByteArrayElements(buffer, NULL);
    auto size = env->GetArrayLength(buffer);
    mapnik::vector_tile_impl::add_image_buffer_as_tile_layer(*tile, layer_name, reinterpret_cast<const char *>(data),
                                                             size);
    env->ReleaseStringUTFChars(name, layer_name);
    env->ReleaseByteArrayElements(buffer, data, JNI_ABORT);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    bufferedExtent
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_VectorTile_bufferedExtent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ext = tile->get_buffered_extent();
    jdoubleArray arr = env->NewDoubleArray(4);
    jdouble tmp[4]{ext.minx(), ext.miny(), ext.maxx(), ext.maxy()};
    env->SetDoubleArrayRegion(arr, 0, 4, tmp);
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_clear(JNIEnv *env, jobject obj) {
    // TODO: noexcept?
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    tile->clear();
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    compositeImpl
 * Signature: ([Lmapnik/VectorTile;DIIDZZIDZ[DDZIII)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_compositeImpl(
    JNIEnv *env, jobject obj, jobjectArray array, jdouble scale_factor, jint offset_x, jint offset_y,
    jdouble area_threshold, jboolean strictly_simple, jboolean multi_polygon_union, jint fill_type,
    jdouble scale_denominator, jboolean reencode, jdoubleArray max_extent, jdouble simplify_distance,
    jboolean process_all_rings, jint image_format, jint scaling_method, jint threading_mode) {
    PREAMBLE;
    if (array == NULL) throw std::exception("input array is null");
    jsize array_len = env->GetArrayLength(array);
    if (array_len == 0) throw std::exception("input array is empty");

    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    mapnik::Map map(tile->tile_size(), tile->tile_size(), "+init=epsg:3857");
    if (max_extent != NULL) {
        if (env->GetArrayLength(max_extent) != 4)
            throw std::exception("max_extent value must be an array of [minx,miny,maxx,maxy]");
        jdouble *tmp = env->GetDoubleArrayElements(max_extent, NULL);
        mapnik::box2d<double> ext(tmp[0], tmp[1], tmp[2], tmp[3]);
        env->ReleaseDoubleArrayElements(max_extent, tmp, JNI_ABORT);
        map.set_maximum_extent(ext);
    }

    std::vector<mapnik::vector_tile_impl::merc_tile_ptr> merc_vtiles;
    for (jsize i = 0; i < array_len; ++i) {
        jobject obj = env->GetObjectArrayElement(array, i);
        if (obj == NULL) throw std::exception("some element of the input VectorTile array is null");
        auto tile = LOAD_VECTOR_TILE_POINTER(obj);
        // prevent shared_ptr from deleting the tile objects by creating a no-op deleter
        merc_vtiles.emplace_back(tile, [](auto &&) {});
        env->DeleteLocalRef(obj);
    }

    mapnik::vector_tile_impl::processor ren(map);
    ren.set_fill_type(static_cast<mapnik::vector_tile_impl::polygon_fill_type>(fill_type));
    ren.set_simplify_distance(simplify_distance);
    ren.set_process_all_rings(process_all_rings);
    ren.set_multi_polygon_union(multi_polygon_union);
    ren.set_strictly_simple(strictly_simple);
    ren.set_area_threshold(area_threshold);
    ren.set_scale_factor(scale_factor);
    ren.set_scaling_method(static_cast<mapnik::scaling_method_e>(scaling_method));
    ren.set_image_format(image_format_names[image_format]);

    ren.set_threading_mode(threading_modes[threading_mode]);
    mapnik::vector_tile_impl::composite(*tile, merc_vtiles, map, ren, scale_denominator, offset_x, offset_y, reencode);
    TRAILER_VOID;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    empty
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_VectorTile_empty(JNIEnv *env, jobject obj) {
    // TODO: noexcept?
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->is_empty();
    TRAILER(false);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    emptyLayers
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_emptyLayers(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_empty_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    extent
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_mapnik_VectorTile_extent(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto ext = tile->extent();
    jdoubleArray arr = env->NewDoubleArray(4);
    jdouble tmp[4]{ext.minx(), ext.miny(), ext.maxx(), ext.maxy()};
    env->SetDoubleArrayRegion(arr, 0, 4, tmp);
    return arr;
    TRAILER(NULL);
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

// TODO: info

/*
 * Class:     mapnik_VectorTile
 * Method:    layerImpl
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_VectorTile_layerImpl(JNIEnv *env, jobject obj, jstring layer_namej) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    if (layer_namej == NULL) throw std::exception("layer name is null");
    auto layer_name = env->GetStringUTFChars(layer_namej, NULL);
    if (!tile->has_layer(layer_name)) throw std::exception("layer does not exist in vector tile");

    auto v = new mapnik::vector_tile_impl::merc_tile(tile->x(), tile->y(), tile->z(), tile->tile_size(),
                                                     tile->buffer_size());
    protozero::pbf_reader tile_message(tile->get_reader());
    while (tile_message.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        auto data_pair = tile_message.get_data();
        protozero::pbf_reader layer_message(data_pair);
        if (!layer_message.next(mapnik::vector_tile_impl::Layer_Encoding::NAME)) continue;
        std::string name = layer_message.get_string();
        if (layer_name == name) {
            v->append_layer_buffer(data_pair.first, data_pair.second, layer_name);
            break;
        }
    }
    env->ReleaseStringUTFChars(layer_namej, layer_name);
    return FROM_POINTER(v);
    TRAILER(0);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    names
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_names(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    painted
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_VectorTile_painted(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    return tile->is_painted();
    TRAILER(false);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    paintedLayers
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_VectorTile_paintedLayers(JNIEnv *env, jobject obj) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    auto &names = tile->get_painted_layers();
    auto arr = env->NewObjectArray(names.size(), CLASS_STRING, NULL);
    int i = 0;
    for (auto &name : names) env->SetObjectArrayElement(arr, i++, env->NewStringUTF(name.c_str()));
    return arr;
    TRAILER(NULL);
}

// TODO: query

// TODO: queryMany

template <typename Renderer>
static void process_layers(Renderer &ren, mapnik::request const &m_req, mapnik::projection const &map_proj,
                           std::vector<mapnik::layer> const &layers, double scale_denom, std::string const &map_srs,
                           mapnik::vector_tile_impl::merc_tile *tile) {
    for (auto const &lyr : layers) {
        if (lyr.visible(scale_denom)) {
            protozero::pbf_reader layer_msg;
            if (tile->layer_reader(lyr.name(), layer_msg)) {
                mapnik::layer lyr_copy(lyr);
                lyr_copy.set_srs(map_srs);
                std::shared_ptr<mapnik::vector_tile_impl::tile_datasource_pbf> ds =
                    std::make_shared<mapnik::vector_tile_impl::tile_datasource_pbf>(layer_msg, tile->x(), tile->y(),
                                                                                    tile->z());
                ds->set_envelope(m_req.get_buffered_extent());
                lyr_copy.set_datasource(ds);
                std::set<std::string> names;
                ren.apply_to_layer(lyr_copy, ren, map_proj, m_req.scale(), scale_denom, m_req.width(), m_req.height(),
                                   m_req.extent(), m_req.buffer_size(), names);
            }
        }
    }
}

/*
 * Class:     mapnik_VectorTile
 * Method:    renderImpl
 * Signature: (Lmapnik/MapDefinition;Lmapnik/Image;[JIDDLjava/util/Map;)V
 */
JNIEXPORT void JNICALL Java_mapnik_VectorTile_renderImpl(JNIEnv *env, jobject obj, jobject mapj, jobject surfacej,
                                                         jlongArray zxy, jint buffer_size, jdouble scale,
                                                         jdouble scale_denominator, jobject variablesj) {
    // TODO: dont know what `Grid` and `CairoSurface` is, only using `Image` type
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);

    if (mapj == 0) throw std::exception("map is null");
    auto &map_in = *LOAD_MAP_POINTER(mapj);

    if (surfacej == 0) throw std::exception("surface is null");
    auto &im = *LOAD_IMAGE_POINTER(surfacej);

    mapnik::box2d<double> map_extent;
    if (zxy != NULL) {
        if (env->GetArrayLength(zxy) != 3) throw std::exception("zxy must be an array of length 3");
        jlong *tmp = env->GetLongArrayElements(zxy, NULL);
        map_extent = mapnik::vector_tile_impl::tile_mercator_bbox(tmp[1], tmp[2], tmp[0]);
        env->ReleaseLongArrayElements(zxy, tmp, JNI_ABORT);
    } else {
        map_extent = mapnik::vector_tile_impl::tile_mercator_bbox(tile->x(), tile->y(), tile->z());
    }

    mapnik::request m_req(im.width(), im.height(), map_extent);
    m_req.set_buffer_size(buffer_size);
    mapnik::projection map_proj(map_in.srs(), true);
    if (scale_denominator <= 0.0)
        scale_denominator = mapnik::scale_denominator(m_req.scale(), map_proj.is_geographic());
    scale_denominator *= scale;
    const auto &layers = map_in.layers();

    mapnik::attributes variables;
    jni_map_to_mapnik_attributes(env, variablesj, variables);
    mapnik::agg_renderer<mapnik::image_rgba8> ren(map_in, m_req, variables, im, scale);
    ren.start_map_processing(map_in);
    process_layers(ren, m_req, map_proj, layers, scale_denominator, map_in.srs(), tile);
    ren.end_map_processing(map_in);

    TRAILER_VOID;
}

// TODO: reportGeometrySimplicity

// TODO: reportGeometryValidity

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

static bool layer_to_geojson(protozero::pbf_reader const &layer, std::string &result, unsigned x, unsigned y,
                             unsigned z) {
    mapnik::vector_tile_impl::tile_datasource_pbf ds(layer, x, y, z);
    mapnik::projection wgs84("epsg:4326", true);
    mapnik::projection merc("epsg:3857", true);
    mapnik::proj_transform prj_trans(merc, wgs84);
    // This mega box ensures we capture all features, including those
    // outside the tile extent. Geometries outside the tile extent are
    // likely when the vtile was created by clipping to a buffered extent
    mapnik::query q(mapnik::box2d<double>(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                                          std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
    mapnik::layer_descriptor ld = ds.get_descriptor();
    for (auto const &item : ld.get_descriptors()) { q.add_property_name(item.get_name()); }
    mapnik::featureset_ptr fs = ds.features(q);
    bool first = true;
    if (fs && mapnik::is_valid(fs)) {
        mapnik::feature_ptr feature;
        while ((feature = fs->next())) {
            if (first) {
                first = false;
            } else {
                result += "\n,";
            }
            std::string feature_str;
            mapnik::feature_impl feature_new(feature->context(), feature->id());
            feature_new.set_data(feature->get_data());
            unsigned int n_err = 0;
            feature_new.set_geometry(mapnik::geometry::reproject_copy(feature->get_geometry(), prj_trans, n_err));
            if (!mapnik::util::to_geojson(feature_str, feature_new)) {
                // LCOV_EXCL_START
                throw std::runtime_error("Failed to generate GeoJSON geometry");
                // LCOV_EXCL_STOP
            }
            result += feature_str;
        }
    }
    return !first;
}

static void write_geojson_array(std::string &result, mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader tile_msg = v->get_reader();
    result += "[";
    bool first = true;
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        if (first) {
            first = false;
        } else {
            result += ",";
        }
        auto pair_data = tile_msg.get_data();
        protozero::pbf_reader layer_msg(pair_data);
        protozero::pbf_reader name_msg(pair_data);
        std::string layer_name;
        if (name_msg.next(mapnik::vector_tile_impl::Layer_Encoding::NAME)) { layer_name = name_msg.get_string(); }
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + layer_name + "\",\"features\":[";
        std::string features;
        bool hit = layer_to_geojson(layer_msg, features, v->x(), v->y(), v->z());
        if (hit) { result += features; }
        result += "]}";
    }
    result += "]";
}

static void write_geojson_all(std::string &result, mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader tile_msg = v->get_reader();
    result += "{\"type\":\"FeatureCollection\",\"features\":[";
    bool first = true;
    while (tile_msg.next(mapnik::vector_tile_impl::Tile_Encoding::LAYERS)) {
        protozero::pbf_reader layer_msg(tile_msg.get_message());
        std::string features;
        bool hit = layer_to_geojson(layer_msg, features, v->x(), v->y(), v->z());
        if (hit) {
            if (first) {
                first = false;
            } else {
                result += ",";
            }
            result += features;
        }
    }
    result += "]}";
}

static bool write_geojson_layer_index(std::string &result, std::size_t layer_idx,
                                      mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader layer_msg;
    if (v->layer_reader(layer_idx, layer_msg) && v->get_layers().size() > layer_idx) {
        std::string layer_name = v->get_layers()[layer_idx];
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + layer_name + "\",\"features\":[";
        layer_to_geojson(layer_msg, result, v->x(), v->y(), v->z());
        result += "]}";
        return true;
    }
    // LCOV_EXCL_START
    return false;
    // LCOV_EXCL_STOP
}

static bool write_geojson_layer_name(std::string &result, std::string const &name,
                                     mapnik::vector_tile_impl::merc_tile *v) {
    protozero::pbf_reader layer_msg;
    if (v->layer_reader(name, layer_msg)) {
        result += "{\"type\":\"FeatureCollection\",";
        result += "\"name\":\"" + name + "\",\"features\":[";
        layer_to_geojson(layer_msg, result, v->x(), v->y(), v->z());
        result += "]}";
        return true;
    }
    return false;
}

/*
 * Class:     mapnik_VectorTile
 * Method:    toGeoJSON
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_VectorTile_toGeoJSON__Ljava_lang_String_2(JNIEnv *env, jobject obj,
                                                                                jstring layer_namej) {
    PREAMBLE;
    if (layer_namej == NULL) throw std::exception("layer name is null");
    auto layer_name_c = env->GetStringUTFChars(layer_namej, 0);
    std::string layer_name = layer_name_c;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::string result;
    if (layer_name == "__array__") {
        write_geojson_array(result, tile);
    } else if (layer_name == "__all__") {
        write_geojson_all(result, tile);
    } else {
        if (!write_geojson_layer_name(result, layer_name, tile))
            throw std::exception("layer does not exist in vector tile");
    }
    env->ReleaseStringUTFChars(layer_namej, layer_name_c);
    return env->NewStringUTF(result.c_str());
    TRAILER(NULL);
}

/*
 * Class:     mapnik_VectorTile
 * Method:    toGeoJSON
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_VectorTile_toGeoJSON__J(JNIEnv *env, jobject obj, jlong layer_idx) {
    PREAMBLE;
    auto tile = LOAD_VECTOR_TILE_POINTER(obj);
    std::string result;
    if (!write_geojson_layer_index(result, layer_idx, tile))
        throw std::exception("layer does not exist in vector tile");
    return env->NewStringUTF(result.c_str());
    TRAILER(NULL);
}

// TODO: toJSON