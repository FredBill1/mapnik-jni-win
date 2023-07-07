package geowin.mapnik;

import java.util.Arrays;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class TestMap {
    public static final String SRS_MERCATOR = "+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs +over";

    @BeforeClass
    public static void initMapnik() {
        Setup.initialize();
    }

    @AfterClass
    public static void tearDownMapnik() {
        Setup.tearDown();
    }

    @Test
    public void testAlloc() {
        try (MapDefinition map = new MapDefinition()) {
        }
    }

    @Test
    public void testConstructorSettings() {
        // try (MapDefinition map = new MapDefinition()) {
        // assertEquals(400, map.getWidth());
        // assertEquals(400, map.getHeight());
        // assertEquals(Projection.LATLNG_PARAMS, map.getSrs());
        // }

        try (MapDefinition map = new MapDefinition(500, 500, SRS_MERCATOR)) {
            assertEquals(500, map.getWidth());
            assertEquals(500, map.getHeight());
            assertEquals(SRS_MERCATOR, map.getSrs());
        }
    }

    @Test
    public void testSize() {
        try (MapDefinition map = new MapDefinition()) {
            map.setWidth(500);
            map.setHeight(600);
            assertEquals(500, map.getWidth());
            assertEquals(600, map.getHeight());

            map.resize(300, 200);
            assertEquals(300, map.getWidth());
            assertEquals(200, map.getHeight());
        }
    }

    @Test
    public void testSrs() {
        try (MapDefinition map = new MapDefinition()) {
            map.setSrs(SRS_MERCATOR);
            assertEquals(SRS_MERCATOR, map.getSrs());
        }
    }

    @Test
    public void testBufferSize() {
        try (MapDefinition map = new MapDefinition()) {
            assertEquals(0, map.getBufferSize());
            map.setBufferSize(256);
            assertEquals(256, map.getBufferSize());
        }
    }

    @Test
    public void testBasePath() {
        try (MapDefinition map = new MapDefinition()) {
            assertEquals("", map.getBasePath());
            map.setBasePath("/tmp");
            assertEquals("/tmp", map.getBasePath());
        }
    }

    @Test
    public void testMapLoad() {
        String mapXml = "<Map buffer-size='256'></Map>";
        try (MapDefinition map = new MapDefinition()) {
            map.fromString(mapXml);
            assertEquals(256, map.getBufferSize());
        }
    }

    @Test
    public void testStyle() {
        try (MapDefinition map = new MapDefinition()) {

            assertNull(map.getStyle("notexist"));
            assertEquals(0, map.getStyleNames().size());
            map.removeStyle("notexist");

            try (FeatureTypeStyle style = new FeatureTypeStyle()) {
                map.addStyle("test", style);
            }
            assertEquals(1, map.getStyleNames().size());
            assertTrue(map.getStyleNames().contains("test"));
            try (FeatureTypeStyle style = map.getStyle("test")) {
                assertNotNull(style);
            }
        }
    }

    @Test
    public void testCollectAttributes() {
        MapDefinition map = new MapDefinition();
        map.load("test/testmap.xml");
        FeatureTypeStyle style = map.getStyle("landmark-poly-text");
        map.close();

        String[] attrs = style.collectAttributes().toArray(new String[0]);
        style.close();
        Arrays.sort(attrs);

        System.out.println("Attrs: " + Arrays.toString(attrs));
        assertTrue(Arrays.equals(attrs, new String[] { "landuse", "leisure", "name", "way_area" }));
    }
}
