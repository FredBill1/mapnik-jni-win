package mapnik;

import java.util.Collection;
import java.util.Set;

/**
 * Wrapper around the Mapnik map object and friends
 * @author stella
 *
 */
public class Map {
	private long ptr;
	
	private static native long alloc(int width, int height, String srs);
	private static native long alloc();
	private static native void dealloc(long ptr);
	
	public Map() {
		ptr=alloc();
	}
	
	public Map(int width, int height, String srs) {
		ptr=alloc(width, height, srs);
	}
	
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	public native void loadMap(String filename, boolean strict);
	public native void loadMapString(String str, boolean strict, String basePath);
	
	public native int getWidth();
	public native int getHeight();
	public native void setWidth(int width);
	public native void setHeight(int height);
	public native void resize(int widht, int height);
	public native String getSrs();
	public native String setSrs(String srs);
	
	public native void setBufferSize(int bufferSize);
	public native int getBufferSize();
	
	public native String getBasePath();
	public native void setBasePath(String basePath);
	
	// Layers
	public native int getLayerCount();
	public native Layer getLayer(int index);
	public native void setLayer(int index, Layer layer);
	public native void removeLayer(int index);
	public native void removeAllLayers();
	public native void addLayer(Layer layer);
	
	// Styles
	public native Collection<String> getStyleNames();
	public native FeatureTypeStyle getStyle(String name);
	public native void addStyle(String name, FeatureTypeStyle style);
	public native void removeStyle(String name);
	
	
}
