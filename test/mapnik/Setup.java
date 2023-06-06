package mapnik;

public class Setup {
    public static void initialize() {
        Mapnik.initialize();
    }

    public static void tearDown() {
        System.err.println("Mapnik native allocation report:");
        System.err.println("--------------------------------");
        System.err.println(Mapnik.reportNativeAllocations());
    }
}
