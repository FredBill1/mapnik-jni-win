package geowin.mapnik;

/**
 * Base class for native objects
 * 
 * @author stella
 *
 */
public abstract class NativeObject implements AutoCloseable {
    /**
     * Native pointer. Depending on the type, this will either be a pointer
     * directly to the object or a smart pointer.
     */
    long ptr;

    NativeObject() {
        Mapnik.incrementAlloc(getClass(), 1);
    }

    /**
     * Manually closes of the native resources associated with this
     * object (versus waiting for finalization).
     */
    @Override
	public final void close() {
        if (ptr != 0) {
            long ptrLocal = ptr;
            ptr = 0;
            dealloc(ptrLocal);
            Mapnik.incrementAlloc(getClass(), -1);
        }
    }

    /**
     * @return true if the native resources associated with this object have been
     *         destroyed
     */
    public final boolean isClosed() {
        return ptr == 0;
    }

    // @Override
    // protected final void finalize() throws Throwable {
    // super.finalize();
    // close();
    // }

    /**
     * Called on finalize and close to dealloc the pointer if != 0
     * 
     * @param ptr
     */
    abstract void dealloc(long ptr);
}
