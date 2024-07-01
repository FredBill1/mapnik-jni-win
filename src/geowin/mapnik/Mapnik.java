package geowin.mapnik;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Global management of the Mapnik installation
 * 
 * @author stella
 *
 */
public class Mapnik {
    private static boolean initialized;
    private static boolean registered;
    private static boolean initializationFailure;

    private static final Map<Class<? extends NativeObject>, AtomicInteger> nativeAllocCounts;
    static {
        nativeAllocCounts = new HashMap<>();
        nativeAllocCounts.put(Datasource.class, new AtomicInteger());
        nativeAllocCounts.put(Feature.class, new AtomicInteger());
        nativeAllocCounts.put(FeatureSet.class, new AtomicInteger());
        nativeAllocCounts.put(FeatureTypeStyle.class, new AtomicInteger());
        nativeAllocCounts.put(Geometry.class, new AtomicInteger());
        nativeAllocCounts.put(Image.class, new AtomicInteger());
        nativeAllocCounts.put(Layer.class, new AtomicInteger());
        nativeAllocCounts.put(MapDefinition.class, new AtomicInteger());
        nativeAllocCounts.put(Projection.class, new AtomicInteger());
        nativeAllocCounts.put(ProjTransform.class, new AtomicInteger());
        nativeAllocCounts.put(Query.class, new AtomicInteger());
        nativeAllocCounts.put(VectorTile.class, new AtomicInteger());
    }

    /**
     * Return a Map of Object type to allocation count of current active native
     * allocations.
     * Native allocations are cleared either through an explicit call to close()
     * or
     * finalization.
     * 
     * @return Map of object type to count for all counts greater than zero
     */
    public static Map<String, Integer> getNativeAllocations() {
        Map<String, Integer> ret = new HashMap<>();
        for (Map.Entry<Class<? extends NativeObject>, AtomicInteger> entry : nativeAllocCounts.entrySet()) {
            int count = entry.getValue().get();
            if (count == 0)
                continue;

            String name = entry.getKey().getName();
            int dotPos = name.lastIndexOf('.');
            if (dotPos >= 0)
                name = name.substring(dotPos + 1);

            ret.put(name, count);
        }
        return ret;
    }

    /**
     * Generate a brief textual report of outstanding native allocations
     */
    public static CharSequence reportNativeAllocations() {
        Map<String, Integer> counts = getNativeAllocations();
        StringBuilder buffer = new StringBuilder();
        buffer.append("MapnikAllocations(");
        boolean first = true;
        for (Map.Entry<String, Integer> entry : counts.entrySet()) {
            if (first)
                first = false;
            else
                buffer.append(", ");
            buffer.append(entry.getKey()).append("=").append(entry.getValue());
        }
        buffer.append(")");
        return buffer;
    }

    static void incrementAlloc(Class<? extends NativeObject> clazz, int delta) {
        AtomicInteger i = nativeAllocCounts.get(clazz);
        if (i == null) {
            throw new IllegalStateException("Not allocation counter defined for " + clazz.getName());
        }
        i.addAndGet(delta);
    }

    /**
     * Initialize the mapnik library.
     * 
     * @param register If true, then also register input plugins and fonts
     */
    public synchronized static void initialize(boolean register) {
        if (!initialized) {
            if (initializationFailure) {
                throw new IllegalStateException("Previous call to Mapnik.initialize() failed");
            }
            File jni_file;
            try {
                jni_file = File.createTempFile("mapnik-jni", ".out");
            } catch (IOException e) {
                throw new IllegalStateException("Failed to create temporary file for jni library", e);
            }
            jni_file.deleteOnExit();

            try (InputStream link = Mapnik.class.getResourceAsStream("/geowin/mapnik/jnilib/mapnik-jni.out")) {
                if (link == null) {
                    throw new IllegalStateException("Failed to load jni library from resources");
                }
                Files.copy(link, jni_file.toPath(), java.nio.file.StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException e) {
                throw new IllegalStateException("Failed to write jni library to temporary file", e);
            }

            System.load(jni_file.getAbsolutePath());

            try {
                nativeInit();
                initialized = true;
            } finally {
                if (!initialized)
                    initializationFailure = true;
            }
        }

        if (!isThreadSafe()) {
            System.err.println("WARNING! Mapnik JNI bindings were compiled against a non-threadsafe Mapnik library.");
            System.err.println("The JVM is a threaded environment, running against a non-threadsafe Mapnik is highly");
            System.err.println("not recommended!");
        }

        if (register && !registered) {
            registered = true;
            String path;

            path = getInstalledFontsDir();
            if (path != null) {
                registerFonts(path);
            }

            path = getInstalledInputPluginsDir();
            if (path != null) {
                registerDatasources(path);
            }
        }
    }

    /**
     * Equivalent to initialize(true). Fully links and initializes the mapnik
     * library.
     */
    public static void initialize() {
        initialize(true);
    }

    private static native void nativeInit();

    /**
     * @return the Mapnik fonts dir as reported by mapnik at compile time or null
     */
    public static native String getInstalledFontsDir();

    /**
     * @return the Mapnik input plugins dir as reported by mapnik at compile time or
     *         null
     */
    public static native String getInstalledInputPluginsDir();

    /**
     * If mapnik is not compiled threadsafe, a warning will be issued on
     * initialization.
     * You can also query it here.
     * 
     * @return true if mapnik was compiled threadsafe
     */
    public static native boolean isThreadSafe();

    /**
     * init the mapnik logger
     */
    public static native void initLogger(String loggerPath, int level);

    public static native boolean registerDatasource(String path);

    public static native boolean registerDatasources(String path, boolean recurse);

    public static boolean registerDatasources(String path) {
        return registerDatasources(path, false);
    }

    public static native boolean datasourceRegistered(String name);

    public static boolean pluginRegistered(String name) {
        return datasourceRegistered(name);
    }

    public static native String datasourceDirectories();

    public static String pluginDirectories() {
        return datasourceDirectories();
    }

    public static native String[] datasources();

    public static String[] pluginNames() {
        return datasources();
    }

    public static native boolean registerFont(String path);

    public static native boolean registerFonts(String path, boolean recurse);

    public static boolean registerFonts(String path) {
        return registerFonts(path, false);
    }

    public static native boolean isFontFile(String path);

    public static native String[] fonts();

    public static native Map<String, String> fontFiles();

    public static native String[] memoryFonts();

    public static native void clearCache();

}
