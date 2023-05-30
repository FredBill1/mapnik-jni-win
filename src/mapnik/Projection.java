package mapnik;

/***
 * A geographical projection: this class makes it possible to translate between
 * locations in different projections
 */
public class Projection extends NativeObject {
    /***
     * An WGS84 (EPSG:4326) proj4 definition
     */
    public static final String LATLNG_PARAMS = "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";

    /**
     * A Spherical web mercator (EPSG:900913) proj4 definition
     */
    public static final String SRS900913_PARAMS = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs +over";

    private static native long alloc(String params);

    @Override
    native void dealloc(long ptr);

    /***
     * Creates a projection for a specified proj4 definition
     * 
     * @param params a proj4 definition
     */
    public Projection(String params) {
        if (params == null) {
            throw new IllegalArgumentException("Projection params cannot be null");
        }
        ptr = alloc(params);
    }

    /***
     * Creates a new WGS84 projection
     */
    public Projection() {
        ptr = alloc(LATLNG_PARAMS);
    }

    @Override
    public boolean equals(Object other) {
        return (other instanceof Projection) && ((Projection) other).getParams().equals(getParams());
    }

    /***
     * @return a proj4 definition passed to the Projection constructor
     */
    public native String getParams();

    /***
     * @return a proj4 definition for the Projection with expanded arguments,
     *         e.g. '+init=epsg:4326' turns into '+init=epsg:4326 +proj=longlat
     *         +datum=WGS84 +no_defs +ellps=WGS84 [...]'
     */
    public native String getExpanded();

    /***
     * Projects from a position in WGS84 space to a position in this projection.
     * Mutates the parameter.
     * 
     * @param coord a Coord in WGS84 space
     */
    public native void forward(Coord coord);

    /***
     * Unprojects from a position in this projection to the same position in WGS84
     * space.
     * Mutates the parameter.
     * 
     * @param coord a Coord in this projection
     */
    public native void inverse(Coord coord);

    /***
     * Projects a bounding box from WGS84 space to this projection.
     * Mutates the parameter.
     * 
     * @param box a Box2d in WGS84 space
     */
    public void forward(Box2d box) {
        Coord c = new Coord(box.minx, box.miny);
        forward(c);
        box.minx = c.x;
        box.miny = c.y;

        c.x = box.maxx;
        c.y = box.maxy;
        forward(c);
        box.maxx = c.x;
        box.maxy = c.y;
    }

    /***
     * Projects a bounding box from this projection to WGS84.
     * Mutates the parameter.
     * 
     * @param box a Box2d in this projection.
     */
    public void inverse(Box2d box) {
        Coord c = new Coord(box.minx, box.miny);
        inverse(c);
        box.minx = c.x;
        box.miny = c.y;

        c.x = box.maxx;
        c.y = box.maxy;
        inverse(c);
        box.maxx = c.x;
        box.maxy = c.y;
    }

}
