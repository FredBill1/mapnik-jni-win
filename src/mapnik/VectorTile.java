package mapnik;

/**
 * A tile generator built according to the Mapbox Vector Tile specification for
 * compressed and simplified tiled vector data.
 * 
 * @author chencai
 */

public class VectorTile extends NativeObject {
    private static native long alloc(long x, long y, long z, int tileSize, int bufferSize);

    native void dealloc(long ptr);

    VectorTile(long ptr) {
        this.ptr = ptr;
    }

    public VectorTile(long x, long y, long z) {
        this(x, y, z, 4096, 128);
    }

    public VectorTile(long x, long y, long z, int tileSize, int bufferSize) {
        this(alloc(x, y, z, tileSize, bufferSize));
    }

    public native int getTileX();

    public native void setTileX(int x);

    public native int getTileY();

    public native void setTileY(int y);

    public native int getTileZ();

    public native void setTileZ(int z);

    public native int getTileSize();

    public native void setTileSize(int size);

    public native int getBufferSize();

    public native void setBufferSize(int size);

    static public class AddOrSetDataOptions {
        public boolean validate = false;
        public boolean upgrade = false;
    }

    public void addData(byte[] buffer, AddOrSetDataOptions options) {
        addDataImpl(buffer, options.validate, options.upgrade);
    }

    public void addData(byte[] buffer) {
        addData(buffer, new AddOrSetDataOptions());
    }

    public native void addDataImpl(byte[] buffer, boolean validate, boolean upgrade);

    public enum PolygonFillType {
        evenOdd, nonZero, positive, negative
    }

    static public class AddGeoJsonOptions {
        public double area_threshold = 0.1;
        public double simplify_distance = 0.0;
        public boolean strictly_simple = true;
        public boolean multi_polygon_union = false;
        public PolygonFillType fill_type = PolygonFillType.positive;
        public boolean process_all_rings = false;
    }

    public void addGeoJSON(String geojson, String name, AddGeoJsonOptions options) {
        addGeoJSONImpl(geojson, name, options.area_threshold, options.simplify_distance, options.strictly_simple,
                options.multi_polygon_union, options.fill_type.ordinal(), options.process_all_rings);
    }

    public void addGeoJSON(String geojson, String name) {
        addGeoJSON(geojson, name, new AddGeoJsonOptions());
    }

    private native void addGeoJSONImpl(String geojson, String name, double area_threshold, double simplify_distance,
            boolean strictly_simple, boolean multi_polygon_union, int fill_type, boolean process_all_rings);

    public enum ImageScaling {
        near, bilinear, bicubic, spline16, spline36, hanning, hamming, hermite, kaiser, quadric, catrom, gaussian,
        bessel, mitchell, sinc, lanczos, blackman
    }

    public enum ImageFormat {
        webp, jpeg, png, tiff
    }

    static public class AddImageOptions {
        public ImageScaling image_scaling = ImageScaling.bilinear;
        public ImageFormat image_format = ImageFormat.webp;
    }

    public void addImage(Image image, String name, AddImageOptions options) {
        addImageImpl(image.ptr, name, options.image_scaling.ordinal(), options.image_format.ordinal());
        image.ptr = 0;
    }

    public void addImage(Image image, String name) {
        addImage(image, name, new AddImageOptions());
    }

    private native void addImageImpl(long image, String name, int image_scaling, int image_format);

    public native void addImageBuffer(byte[] buffer, String name);

    public native double[] bufferedExtent();

    public native void clear();

    public enum Launch {
        async, deferred
    }

    static public class CompositeOptions {
        public double scale_factor = 1.0;
        public int offset_x = 0;
        public int offset_y = 0;
        public double area_threshold = 0.1;
        public boolean strictly_simple = true;
        public boolean multi_polygon_union = false;
        public PolygonFillType fill_type = PolygonFillType.positive;
        public double scale_denominator = 0.0;
        public boolean reencode = false;
        public double[] max_extent = null;
        public double simplify_distance = 0.0;
        public boolean process_all_rings = false;
        public ImageFormat image_format = ImageFormat.webp;
        public ImageScaling scaling_method = ImageScaling.bilinear;
        public Launch threading_mode = Launch.deferred;
    }

    public void composite(VectorTile[] array, CompositeOptions options) {
        compositeImpl(array, options.scale_factor, options.offset_x, options.offset_y, options.area_threshold,
                options.strictly_simple, options.multi_polygon_union, options.fill_type.ordinal(),
                options.scale_denominator, options.reencode, options.max_extent, options.simplify_distance,
                options.process_all_rings, options.image_format.ordinal(), options.scaling_method.ordinal(),
                options.threading_mode.ordinal());
    }

    public void composite(VectorTile[] array) {
        composite(array, new CompositeOptions());
    }

    private native void compositeImpl(VectorTile[] array, double scale_factor, int offset_x, int offset_y,
            double area_threshold, boolean strictly_simple, boolean multi_polygon_union, int fill_type,
            double scale_denominator, boolean reencode, double[] max_extent, double simplify_distance,
            boolean process_all_rings, int image_format, int scaling_method, int threading_mode);

    public native boolean empty();

    public native String[] emptyLayers();

    public native double[] extent();

    public enum Compression {
        none, gzip
    }

    public enum CompressionStrategy {
        DEFAULT, FILTERED, HUFFMAN_ONLY, RLE, FIXED
    }

    static public class GetDataOptions {
        public Compression compression = Compression.none;
        public int level = 0;
        public CompressionStrategy strategy = CompressionStrategy.DEFAULT;
    }

    public byte[] getData(GetDataOptions options) {
        return getDataImpl(options.compression == Compression.gzip, options.level, options.strategy.ordinal());
    }

    public byte[] getData() {
        return getData(new GetDataOptions());
    }

    public native byte[] getDataImpl(boolean compress, int level, int strategy);

    public native void info(byte[] buffer); // TODO: json

    public VectorTile layer(String layerName) {
        return new VectorTile(layerImpl(layerName));
    }

    private native long layerImpl(String layerName);

    public native String[] names();

    public native boolean painted();

    public native String[] paintedLayers();

    static public class QueryOptions {
        public double tolerance = 0.0;
        public String layer = null;
    }

    public FeatureSet query(double longitude, double latitude, QueryOptions options) {
        return queryImpl(longitude, latitude, options.tolerance, options.layer);
    }

    public native FeatureSet queryImpl(double longitude, double latitude, double tolerance, String layer);

    static public class QueryManyOptions {
        public double tolerance = 0.0;
        public String layer = null;
        public String[] fields = null;
    }

    public Object queryMany(float[][] array, QueryManyOptions options) {
        return queryManyImpl(array, options.tolerance, options.layer, options.fields);
    }

    public native Object queryManyImpl(float[][] array, double tolerance, String layer, String[] fields);

    enum Renderer {
        cairo, svg
    }

    static public class RenderOptions {
        public long[] zxy = null;
        public int buffer_size = 256;
        public double scale = 1.0;
        public double scale_denominator = 1.0;
    }

    public void render(MapDefinition map, Image surface, RenderOptions options) {
        renderImpl(map, surface, options.zxy, options.buffer_size, options.scale,
                options.scale_denominator);
    }

    public void render(MapDefinition map, Image surface) {
        render(map, surface, new RenderOptions());
    }

    public native void renderImpl(MapDefinition map, Image surface, long[] zxy, int buffer_size, double scale,
            double scale_denominator);

    static public class NotSimpleFeature {
        public String layer;
        public long feature_id;
    }

    public native NotSimpleFeature[] reportGeometrySimplicity();

    static public class NotValidFeature {
        public String message;
        public String layer;
        public long feature_id;
        public String geojson;
    }

    public native NotValidFeature[] reportGeometryValidity(Object options);

    public void setData(byte[] buffer, AddOrSetDataOptions options) {
        setDataImpl(buffer, options.validate, options.upgrade);
    }

    public void setData(byte[] buffer) {
        setData(buffer, new AddOrSetDataOptions());
    }

    public native void setDataImpl(byte[] buffer, boolean validate, boolean upgrade);

    public native String toGeoJSON(String layer_name);

    public native String toGeoJSON(long layer_idx);

    public native Object toJSON();
}
