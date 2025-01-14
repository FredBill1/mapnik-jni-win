#include "geowin_mapnik_MapDefinition.h"

#include <algorithm>

#include "globals.hpp"

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    alloc
 * Signature: (IILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_geowin_mapnik_MapDefinition_alloc__IILjava_lang_String_2(JNIEnv* env, jclass c, jint width,
                                                                               jint height, jstring srsj) {
    PREAMBLE;
    JNIString srs(env, srsj);
    return FROM_POINTER(new mapnik::Map(width, height, srs.get()));
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    alloc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_geowin_mapnik_MapDefinition_alloc__(JNIEnv* env, jclass c) {
    PREAMBLE;
    return FROM_POINTER(new mapnik::Map());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    copy
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_geowin_mapnik_MapDefinition_copy(JNIEnv* env, jclass c, jlong otherPtr) {
    if (!otherPtr) return 0;
    PREAMBLE;

    mapnik::Map* other = static_cast<mapnik::Map*>(TO_POINTER(otherPtr));
    mapnik::Map* m = new mapnik::Map(*other);

    return FROM_POINTER(m);
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_dealloc(JNIEnv* env, jobject, jlong ptr) {
    PREAMBLE;
    if (ptr) { delete static_cast<mapnik::Map*>(TO_POINTER(ptr)); }
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getLayerCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_MapDefinition_getLayerCount(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return (jint)map->layer_count();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getLayer
 * Signature: (I)Lmapnik/Layer;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getLayer__I(JNIEnv* env, jobject mapobject, jint index) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    auto& layers = map->layers();
    auto layer_copy = new mapnik::layer(layers.at(index));
    return createLayerObj(env, layer_copy);
    TRAILER(NULL);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getLayer
 * Signature: (Ljava/lang/String;)Lmapnik/Layer;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getLayer__Ljava_lang_String_2(JNIEnv* env, jobject mapobject,
                                                                                  jstring namej) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    std::string name = JNIString(env, namej).get();
    auto& layers = map->layers();
    auto it = std::find_if(layers.begin(), layers.end(), [&](auto&& x) { return x.name() == name; });
    if (it == layers.end()) {
        std::ostringstream ss;
        ss << "layer with name `" << name << "` not found";
        throw_runtime_exception(env, ss.str().c_str());
        return NULL;
    }
    auto layer_copy = new mapnik::layer(*it);
    return createLayerObj(env, layer_copy);
    TRAILER(NULL);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setLayer
 * Signature: (ILmapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setLayer(JNIEnv* env, jobject mapobject, jint index,
                                                          jobject layerobject) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    auto layer = LOAD_LAYER_POINTER(layerobject);
    auto& layers = map->layers();
    layers.at(index) = *layer;
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    removeLayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_removeLayer(JNIEnv* env, jobject mapobject, jint index) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->remove_layer(index);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    addLayer
 * Signature: (Lgeowin/mapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_addLayer(JNIEnv* env, jobject mapobject, jobject layerobject) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    auto layer = LOAD_LAYER_POINTER(layerobject);
    map->add_layer(*layer);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    removeAllLayers
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_removeAllLayers(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    auto& layers = map->layers();
    layers.clear();
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_MapDefinition_getWidth(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return (jint)map->width();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_MapDefinition_getHeight(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return (jint)map->height();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setWidth
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setWidth(JNIEnv* env, jobject mapobject, jint width) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->set_width(width);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setHeight
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setHeight(JNIEnv* env, jobject mapobject, jint height) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->set_height(height);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    resize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_resize(JNIEnv* env, jobject mapobject, jint width, jint height) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->resize(width, height);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_MapDefinition_getSrs(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return env->NewStringUTF(map->srs().c_str());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setSrs(JNIEnv* env, jobject mapobject, jstring srsj) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString srs(env, srsj);

    map->set_srs(srs.get());
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setBufferSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setBufferSize(JNIEnv* env, jobject mapobject, jint s) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->set_buffer_size(s);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_MapDefinition_getBufferSize(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return (jint)map->buffer_size();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getBasePath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_MapDefinition_getBasePath(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return env->NewStringUTF(map->base_path().c_str());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setBasePath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setBasePath(JNIEnv* env, jobject mapobject, jstring basepathj) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString basepath(env, basepathj);
    map->set_base_path(basepath.get());
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getStyleNames
 * Signature: ()Ljava/util/Collection;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getStyleNames(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    jobject ret = env->NewObject(CLASS_HASHSET, CTOR_HASHSET);
    for (mapnik::Map::style_iterator iter = map->begin_styles(); iter != map->end_styles(); iter++) {
        JNIObject name(env, env->NewStringUTF(iter->first.c_str()));
        env->CallBooleanMethod(ret, METHOD_HASHSET_ADD, name.get());
    }

    return ret;
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getStyle
 * Signature: (Ljava/lang/String;)Lmapnik/FeatureTypeStyle;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getStyle(JNIEnv* env, jobject mapobject, jstring namej) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString name(env, namej);
    std::string namestring(name.get());

    boost::optional<mapnik::feature_type_style const&> style = map->find_style(namestring);
    if (!style) return 0;

    mapnik::feature_type_style* stylepinned = new mapnik::feature_type_style(style.get());

    jobject ret = env->NewObject(CLASS_FEATURE_TYPE_STYLE, CTOR_NATIVEOBJECT);
    env->SetLongField(ret, FIELD_PTR, FROM_POINTER(stylepinned));
    return ret;
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    addStyle
 * Signature: (Ljava/lang/String;Lmapnik/FeatureTypeStyle;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_addStyle(JNIEnv* env, jobject mapobject, jstring namej,
                                                          jobject styleobject) {
    PREAMBLE;
    if (!styleobject) return;

    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString name(env, namej);
    mapnik::feature_type_style* style =
        static_cast<mapnik::feature_type_style*>(TO_POINTER(env->GetLongField(styleobject, FIELD_PTR)));
    map->insert_style(name.get(), *style);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    removeStyle
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_removeStyle(JNIEnv* env, jobject mapobject, jstring namej) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    JNIString name(env, namej);
    map->remove_style(name.get());
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getMaximumExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getMaximumExtent(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    boost::optional<mapnik::box2d<double> > extent = map->maximum_extent();
    if (!extent) return 0;

    return box2dFromNative(env, extent.get());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setMaximumExtent
 * Signature: (Lgeowin/mapnik/Box2d;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setMaximumExtent(JNIEnv* env, jobject mapobject, jobject extentj) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(mapobject);
    map->set_maximum_extent(box2dToNative(env, extentj));
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getExtent(JNIEnv* env, jobject obj) {
    PREAMBLE;
    auto map = LOAD_MAP_POINTER(obj);
    auto& extent = map->get_current_extent();
    return box2dFromNative(env, extent);
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getBufferedExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getBufferedExtent(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    mapnik::box2d<double> extent = map->get_buffered_extent();
    return box2dFromNative(env, extent);
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    zoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_zoom(JNIEnv* env, jobject mapobject, jdouble z) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->zoom(z);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    zoomToBox
 * Signature: (Lgeowin/mapnik/Box2d;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_zoomToBox(JNIEnv* env, jobject mapobject, jobject box) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->zoom_to_box(box2dToNative(env, box));
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    zoomAll
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_zoomAll(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->zoom_all();
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    pan
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_pan(JNIEnv* env, jobject mapobject, jint x, jint y) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->pan(x, y);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    panAndZoom
 * Signature: (IID)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_panAndZoom(JNIEnv* env, jobject mapobject, jint x, jint y, jdouble z) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->pan_and_zoom(x, y, z);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getScale
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_geowin_mapnik_MapDefinition_getScale(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return map->scale();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getScaleDenominator
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_geowin_mapnik_MapDefinition_getScaleDenominator(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return map->scale_denominator();
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getBackground
 * Signature: ()Lmapnik/Color;
 */
JNIEXPORT jobject JNICALL Java_geowin_mapnik_MapDefinition_getBackground(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    boost::optional<mapnik::color> const& color(map->background());
    if (!color) return 0;

    return colorFromNative(env, color.get());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setBackground
 * Signature: (Lgeowin/mapnik/Color;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setBackground(JNIEnv* env, jobject mapobject, jobject c) {
    PREAMBLE;
    if (!c) return;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    map->set_background(colorToNative(env, c));
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    getBackgroundImage
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_MapDefinition_getBackgroundImage(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    boost::optional<std::string> const& image(map->background_image());
    if (!image) return 0;

    return env->NewStringUTF(image.get().c_str());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    setBackgroundImage
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_setBackgroundImage(JNIEnv* env, jobject mapobject, jstring filenamej) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    if (!filenamej) return;
    JNIString filename(env, filenamej);
    map->set_background_image(filename.get());
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    saveMap
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition_saveMap(JNIEnv* env, jobject mapobject, jstring filenamej,
                                                         jboolean explicitDefaults) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    if (!filenamej) {
        throw_runtime_exception(env, "Filename is required");
        return;
    }

    JNIString filename(env, filenamej);
    mapnik::save_map(*map, filename.get(), explicitDefaults);

    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    saveMapToString
 * Signature: (Z)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_geowin_mapnik_MapDefinition_saveMapToString(JNIEnv* env, jobject mapobject,
                                                                    jboolean explicit_defaults) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    std::string s = mapnik::save_map_to_string(*map, explicit_defaults);
    return env->NewStringUTF(s.c_str());
    TRAILER(0);
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    _setAspectFixMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_geowin_mapnik_MapDefinition__1setAspectFixMode(JNIEnv* env, jobject mapobject, jint v) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    if (v < 0 || v > mapnik::Map::aspect_fix_mode_MAX) {
        throw std::runtime_error("aspect_fix_mode value is out of range");
    }
    map->set_aspect_fix_mode((mapnik::Map::aspect_fix_mode)v);
    TRAILER_VOID;
}

/*
 * Class:     geowin_mapnik_MapDefinition
 * Method:    _getAspectFixMode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_geowin_mapnik_MapDefinition__1getAspectFixMode(JNIEnv* env, jobject mapobject) {
    PREAMBLE;
    mapnik::Map* map = LOAD_MAP_POINTER(mapobject);
    return (jint)map->get_aspect_fix_mode();
    TRAILER(0);
}
