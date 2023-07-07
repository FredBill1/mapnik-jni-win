package geowin.mapnik;

import java.util.Map;

public class Feature extends NativeObject {

    @Override
    native void dealloc(long ptr);

    private Feature() {
    }

    public static native Feature fromJSON(String geojson);

    public native Map<String, Object> attributes();

    public native Box2d extent();

    public native Geometry geometry();

    public native int id();

    public native String toJSON();
}
