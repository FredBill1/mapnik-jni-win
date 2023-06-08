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

    public native Map<String, Object> parameters();

    public native int getType();

    public native Box2d getEnvelope();

    public Box2d extent() {
        return getEnvelope();
    }

    public native FeatureSet features(Query q);

    public FeatureSet featureset(Box2d extent) {
        try (Query q = new Query(extent)) {
            return features(q);
        }
    }

    public native FeatureSet featuresAtPoint(Coord pt);

    public native LayerDescriptor getDescriptor();

}
