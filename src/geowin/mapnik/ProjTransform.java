package geowin.mapnik;

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

    public native Coord forward(Coord xy);

    public native Box2d backward(Box2d box);

    public native Coord backward(Coord xy);
}
