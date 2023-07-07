package geowin.mapnik;

import geowin.mapnik.options.GeometryTypes;

/**
 * A mapnik::geometry_type instance. These instances are always owned
 * by the corresponding feature.
 * 
 * @author stella
 *
 */
public class Geometry extends NativeObject {
    // holds a `mapnik::feature_ptr*`, same as the class `Feature`

    private Geometry() {
    }

    @Override
    native void dealloc(long ptr);

    public native Box2d extent();

    public native String toJSON(ProjTransform transform);

    public String toJSON() {
        return toJSON(null);
    }

    public native byte[] toWKB();

    public native String toWKT();

    private native int typeImpl();

    public GeometryTypes type() {
        return GeometryTypes.values()[typeImpl()];
    }

    public String typeName() {
        return type().name();
    }
}
