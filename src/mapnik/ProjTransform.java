package mapnik;

public class ProjTransform extends NativeObject {
    private static native long alloc(Projection source, Projection dest);

    @Override
    native void dealloc(long ptr);

    public ProjTransform(Projection source, Projection dest) {
        ptr = alloc(source, dest);
    }

    public ProjTransform(String source, String dest) {
        try (Projection sourceProj = new Projection(source); Projection destProj = new Projection(dest)) {
            ptr = alloc(sourceProj, destProj);
        }
    }

    public native Box2d forward(Box2d box);

    public native double[] forward(double x, double y); // TODO: use Coord instead

    public native Box2d backward(Box2d box);

    public native double[] backward(double x, double y); // TODO: use Coord instead
}
