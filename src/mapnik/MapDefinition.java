package mapnik;

import java.util.Collection;
import java.util.Map;

import mapnik.options.ImageFormat;
import mapnik.options.ImageScaling;
import mapnik.options.Launch;
import mapnik.options.PolygonFillType;

/**
 * Wrapper around the mapnik::Map object.
 * <p>
 * Several methods that are found elsewhere in the C++ code but operate
 * exlusively
 * on maps are located directly on this class:
 * <ul>
 * <li>loadMap
 * <li>loadMapString
 * <li>saveMap
 * <li>saveMapToString
 * </ul>
 * <p>
 * Not all child objects are implemented.
 * <p>
 * Note that the following child objects are value types. When they are added to
 * or
 * read from a MapDefinition, the caller will be left with a copy.
 * <ul>
 * <li>Layer
 * <li>FeatureTypeStyle
 * </ul>
 * 
 * @author stella
 *
 */
public class MapDefinition extends NativeObject {
    private static native long alloc(int width, int height, String srs);

    private static native long alloc();

    private static native long copy(long ptr);

    @Override
    native void dealloc(long ptr);

    public MapDefinition() {
        ptr = alloc();
    }

    public MapDefinition(int width, int height, String srs) {
        ptr = alloc(width, height, srs);
    }

    public MapDefinition(MapDefinition other) {
        ptr = copy(other.ptr);
    }

    public MapDefinition clone() {
        return new MapDefinition(this);
    }

    public static class LoadOptions {
        boolean strict = false;
        String base = "";
    }

    private native void loadImpl(String filename, boolean strict, String basePath);

    public void load(String filename, LoadOptions options) {
        loadImpl(filename, options.strict, options.base);
    }

    public void load(String filename) {
        load(filename, new LoadOptions());
    }

    private native void fromStringImpl(String str, boolean strict, String basePath);

    public void fromString(String stylesheet, LoadOptions options) {
        fromStringImpl(stylesheet, options.strict, options.base);
    }

    public void fromString(String stylesheet) {
        fromString(stylesheet, new LoadOptions());
    }

    public native int getWidth();

    public native int getHeight();

    public native void setWidth(int width);

    public native void setHeight(int height);

    public native void resize(int widht, int height);

    public native String getSrs();

    public native void setSrs(String srs);

    public native void setBufferSize(int bufferSize);

    public native int getBufferSize();

    public native String getBasePath();

    public native void setBasePath(String basePath);

    // Layers
    public native int getLayerCount();

    public native Layer getLayer(int index);

    public Layer getLayer(String name) {
        for (int i = 0; i < getLayerCount(); i++) {
            Layer layer = getLayer(i);
            if (layer.getName().equals(name))
                return layer;
            layer.close();
        }
        return null;
    }

    public Layer get_layer(int index) {
        return getLayer(index);
    }

    public Layer get_layer(String name) {
        return getLayer(name);
    }

    public Layer[] layers() {
        Layer[] layers = new Layer[getLayerCount()];
        for (int i = 0; i < layers.length; i++)
            layers[i] = getLayer(i);
        return layers;
    }

    public native void setLayer(int index, Layer layer);

    public native void removeLayer(int index);

    public void removeLayer(String name) {
        int count = getLayerCount();
        for (int i = 0; i < count; i++) {
            try (Layer layer = getLayer(i)) {
                if (layer.getName().equals(name)) {
                    removeLayer(i);
                    return;
                }
            }
        }
    }

    public void remove_layer(int index) {
        removeLayer(index);
    }

    public void remove_layer(String name) {
        removeLayer(name);
    }

    public native void removeAllLayers();

    public void clear() {
        removeAllLayers();
    }

    public native void addLayer(Layer layer);

    public void add_layer(Layer layer) {
        addLayer(layer);
    }

    // Aspect fix mode
    public void setAspectFixMode(AspectFixMode m) {
        _setAspectFixMode(m.nativeCode);
    }

    public AspectFixMode getAspectFixMode() {
        return AspectFixMode.fromNativeCode(_getAspectFixMode());
    }

    private native void _setAspectFixMode(int v);

    private native int _getAspectFixMode();

    // Styles
    public native Collection<String> getStyleNames();

    public native FeatureTypeStyle getStyle(String name);

    public native void addStyle(String name, FeatureTypeStyle style);

    public native void removeStyle(String name);

    // Extents
    public native Box2d getMaximumExtent();

    public native void setMaximumExtent(Box2d extent);

    public native Box2d getExtent();

    public void setExtent(Box2d extent) {
        zoomToBox(extent);
    }

    public native Box2d getBufferedExtent();

    // Movement
    public native void zoom(double factor);

    public native void zoomToBox(Box2d box);

    public native void zoomAll();

    public native void pan(int x, int y);

    public native void panAndZoom(int x, int y, double zoom);

    public native double getScale();

    public double scale() {
        return getScale();
    }

    public native double getScaleDenominator();

    public double scaleDenominator() {
        return getScaleDenominator();
    }

    // Background
    public native Color getBackground();

    public native void setBackground(Color color);

    public native String getBackgroundImage();

    public native void setBackgroundImage(String filename);

    // Save map
    public native void saveMap(String fileName, boolean explicitDefaults);

    public native String saveMapToString(boolean explicitDefaults);

    public void saveMap(String fileName) {
        saveMap(fileName, false);
    }

    public void save(String fileName) {
        saveMap(fileName);
    }

    public String saveMapToString() {
        return saveMapToString(false);
    }

    public String toXML() {
        return saveMapToString();
    }

    public static class RenderVectorTileOptions {
        double scale = 1.0;
        double scale_denominator = 0.0;
        int offset_x = 0;
        int offset_y = 0;
        ImageScaling image_scaling = ImageScaling.bilinear;
        ImageFormat image_format = ImageFormat.webp;
        double area_threshold = 0.1;
        boolean strictly_simple = true;
        boolean multi_polygon_union = false;
        PolygonFillType fill_type = PolygonFillType.positive;
        Launch threading_mode = Launch.deferred;
        double simplify_distance = 0.0;
        Map<String, Object> variables = null;
        boolean process_all_rings = false;
    }

    private native void renderVectorTileImpl(VectorTile tile, double scale, double scale_denominator, int offset_x,
            int offset_y, int image_scaling, int image_format, double area_threshold, boolean strictly_simple,
            boolean multi_polygon_union, int fill_type, int threading_mode, double simplify_distance,
            Map<String, Object> variables, boolean process_all_rings);

    public void renderVectorTile(VectorTile tile, RenderVectorTileOptions options) {
        renderVectorTileImpl(tile, options.scale, options.scale_denominator, options.offset_x, options.offset_y,
                options.image_scaling.ordinal(), options.image_format.ordinal(), options.area_threshold,
                options.strictly_simple, options.multi_polygon_union, options.fill_type.ordinal(),
                options.threading_mode.ordinal(), options.simplify_distance, options.variables,
                options.process_all_rings);
    }

    public void renderVectorTile(VectorTile tile) {
        renderVectorTile(tile, new RenderVectorTileOptions());
    }

    public void render(VectorTile tile) {
        renderVectorTile(tile);
    }

    public void render(VectorTile tile, RenderVectorTileOptions options) {
        renderVectorTile(tile, options);
    }

    public static class RenderImageOptions {
        int buffer_size = 0;
        double scale = 1.0;
        double scale_denominator = 0.0;
        int offset_x = 0;
        int offset_y = 0;
        Map<String, Object> variables = null;
    }

    private native void renderImageImpl(Image image, int buffer_size, double scale, double scale_denominator,
            int offset_x, int offset_y, Map<String, Object> variables);

    public void renderImage(Image image, RenderImageOptions options) {
        renderImageImpl(image, options.buffer_size, options.scale, options.scale_denominator, options.offset_x,
                options.offset_y, options.variables);
    }

    public void renderImage(Image image) {
        renderImage(image, new RenderImageOptions());
    }

    public void render(Image image) {
        renderImage(image);
    }

    public void render(Image image, RenderImageOptions options) {
        renderImage(image, options);
    }

    public void renderFile(String fileName, String type, RenderImageOptions options) {
        try (Image image = new Image(getWidth(), getHeight())) {
            renderImage(image, options);
            image.saveToFile(fileName, type);
        }
    }

    public native String[] fonts();

    public native String fontDirectory();

    public native Map<String, String> fontFiles();

    public native String[] memoryFonts();

    public native boolean loadFonts();

    public native boolean registerFonts(String path, boolean recurse);

    public boolean registerFonts(String path) {
        return registerFonts(path, false);
    }

    public native FeatureSet[] queryMapPoint(double x, double y, String layer_name);

    public native FeatureSet[] queryMapPoint(double x, double y, int layer_index);

    public native FeatureSet[] queryMapPoint(double x, double y);

    public native FeatureSet[] queryPoint(double x, double y, String layer_name);

    public native FeatureSet[] queryPoint(double x, double y, int layer_index);

    public native FeatureSet[] queryPoint(double x, double y);
}
