package mapnik;

import java.util.Map;

public class Datasource extends NativeObject {
    // Native pointer: boost::shared_ptr<mapnik::datasource>

    @Override
    native void dealloc(long ptr);

    private native long alloc(Map<String, Object> params);

    public Datasource(Map<String, Object> params) {
        ptr = alloc(params);
    }

    public native Parameters getParameters();

    public native int getType();

    public native Box2d getEnvelope();

    public native FeatureSet features(Query q);

    public native FeatureSet featuresAtPoint(Coord pt);

    public native LayerDescriptor getDescriptor();
}
