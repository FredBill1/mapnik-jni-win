package geowin.mapnik;

/**
 * Wraps the mapnik::Layer value type. When a Layer is read from or
 * written to the map, it is copied.
 * 
 * @author stella
 *
 */
public class Layer extends NativeObject {
    private static native long alloc(String name, String srs);

    @Override
	native void dealloc(long ptr);

    public Layer(String name, String srs) {
        ptr = alloc(name, srs);
    }

    public Layer(String name) {
        this(name, Projection.LATLNG_PARAMS);
    }

    public native String getName();

    public native void setName(String name);

    public native String getSrs();

    public native void setSrs(String srs);

    public native String[] getStyles();

    public native void setStyles(String[] styles);

    public native double getMinZoom();

    public native void setMinZoom(double z);

    public native double getMaxZoom();

    public native void setMaxZoom(double z);

    public native boolean isActive();

    public native void setActive(boolean active);

    public native boolean isQueryable();

    public native void setQueryable(boolean b);

    public native boolean isVisible(double scale);

    public native boolean isClearLabelCache();

    public native void setClearLabelCache(boolean b);

    public native boolean isCacheFeatures();

    public native void setCacheFeatures(boolean b);

    public native Datasource getDatasource();

    public native void setDatasource(Datasource datasource);

    public native double getMinimumScaleDenominator();

    public native void setMinimumScaleDenominator(double minimum_scale_denom);

    public native double getMaximumScaleDenominator();

    public native void setMaximumScaleDenominator(double maximum_scale_denom);

    public static class Description implements AutoCloseable {
        public String name;
        public String srs;
        public boolean active;
        public boolean clear_label_cache;
        public double minimum_scale_denominator;
        public double maximum_scale_denominator;
        public boolean queryable;
        public String[] style_names;
        public Datasource datasource;

        @Override
        public void close() {
            if (datasource != null) {
                datasource.close();
                datasource = null;
            }
        }
    }

    public Description describe() {
        Description desc = new Description();
        desc.name = getName();
        desc.srs = getSrs();
        desc.active = isActive();
        desc.clear_label_cache = isClearLabelCache();
        desc.minimum_scale_denominator = getMinimumScaleDenominator();
        desc.maximum_scale_denominator = getMaximumScaleDenominator();
        desc.queryable = isQueryable();
        desc.style_names = getStyles();
        desc.datasource = getDatasource();
        return desc;
    }
}
