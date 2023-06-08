package mapnik;

import java.util.Map;
import java.util.HashMap;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

public class TestDatasource {
    public static final String SHAPEFILE_NAME = "data/TM_WORLD_BORDERS_SIMPL_0.3_MOD";

    @BeforeClass
    public static void initMapnik() {
        Setup.initialize();
    }

    @AfterClass
    public static void tearDownMapnik() {
        Setup.tearDown();
    }

    @Test
    public void testBindAndTableDescriptors() {
        Map<String, Object> params = new HashMap<String, Object>();
        params.put("type", "shape");
        params.put("file", SHAPEFILE_NAME);

        Datasource ds = new Datasource(params);
        System.out.println("Bound datasource");

        LayerDescriptor ld = ds.getDescriptor();
        System.out.println("planet_osm_line descriptor:");
        System.out.println(ld.toString());
    }

    @Test
    public void testFeatures() {
        Map<String, Object> params = new HashMap<String, Object>();
        params.put("type", "shape");
        params.put("file", SHAPEFILE_NAME);

        Datasource ds = new Datasource(params);
        LayerDescriptor ld = ds.getDescriptor();

        Box2d bbox = new Box2d(-180, -85, 180, 85);

        Query query = new Query(bbox);
        for (AttributeDescriptor attr : ld.getDescriptors()) {
            query.addPropertyName(attr.getName());
        }

        FeatureSet fs = ds.features(query);
        System.out.println("FeatureSet queried");

        while (true) {
            try (Feature f = fs.next()) {
                if (f == null)
                    break;
                StringBuilder sb = new StringBuilder();
                sb.append("Feature:");
                Map<String, Object> attributes = f.attributes();
                for (String name : attributes.keySet()) {
                    Object value = attributes.get(name);
                    if (value == null)
                        continue;
                    sb.append(" ").append(name).append("=").append(value);
                }
                try (Geometry g = f.geometry()) {
                    sb.append(g.toJSON());
                }
                System.out.println(sb.toString());
            }
        }

        params = ds.parameters();
    }
}
