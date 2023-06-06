package mapnik;

public class FeatureSet extends NativeObject {
    // Pointer to mapnik::featureset_ptr

    private FeatureSet() {
    }

    @Override
    native void dealloc(long ptr);

    public native Feature next();
}
