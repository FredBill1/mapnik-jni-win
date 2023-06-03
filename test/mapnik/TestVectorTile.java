package mapnik;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import mapnik.options.Compression;

import static org.junit.Assert.*;

import java.io.File;
import java.io.FileOutputStream;

public class TestVectorTile {
    @BeforeClass
    public static void initMapnik() {
        Setup.initialize();
    }

    @AfterClass
    public static void tearDownMapnik() {
        Setup.tearDown();
    }

    @Test
    public void testCreate() {
        VectorTile tile = new VectorTile(0, 0, 0);
        tile = new VectorTile(0, 0, 0, 4096, 128);
    }

    @Test
    public void test1() {
        Box2d bounds = new Box2d(-180, -90, 180, 90);
        MapDefinition m = new MapDefinition();
        m.loadMap("C:/Users/17819/GEO/mapnik-jni-win/122.xml", false);
        m.setSrs(Projection.SRS900913_PARAMS);
        m.resize(512, 512);
        m.zoomToBox(bounds);
        Image image = new Image(512, 512);
        VectorTile vt = new mapnik.VectorTile(3, 3, 2);
        m.renderVectorTile(vt);
        vt.render(m, image);

        VectorTile.GetDataOptions getDataOptions = new VectorTile.GetDataOptions();
        getDataOptions.compression = Compression.gzip;
        getDataOptions.level = 9;
        VectorTile.Info info = VectorTile.info(vt.getData(getDataOptions));

        image.saveToFile("C:/Users/17819/GEO/mapnik-jni-win/test.png", "png");
        Image image2 = new Image(4096, 4096);
        m.renderImage(image2);
        image2.saveToFile("C:/Users/17819/GEO/mapnik-jni-win/test2.png", "png");

        File file = new File("C:/Users/17819/GEO/mapnik-jni-win/test.pbf");
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(vt.getData(getDataOptions));
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        VectorTile.JSON json = vt.toJSON(true);

        VectorTile.NotSimpleFeature[] notSimpleFeatures = vt.reportGeometrySimplicity();
        VectorTile.NotValidFeature[] notValidFeatures = vt.reportGeometryValidity();

        m.close();
        vt.close();
        image.close();
        image2.close();
    }
}
