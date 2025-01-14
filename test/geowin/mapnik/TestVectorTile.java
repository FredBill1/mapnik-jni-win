package geowin.mapnik;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import geowin.mapnik.options.Compression;

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
        tile.close();
        tile = new VectorTile(0, 0, 0, 4096, 128);
        tile.close();
    }

    @Test
    public void test1() {
        Box2d bounds = new Box2d(-180, -90, 180, 90);
        MapDefinition m = new MapDefinition();
        m.load("test/122.xml");
        m.setSrs(Projection.SRS900913_PARAMS);
        m.resize(512, 512);
        m.zoomToBox(bounds);
        Image image = new Image(512, 512);
        VectorTile vt = new VectorTile(3, 3, 2);
        m.renderVectorTile(vt);
        vt.render(m, image);

        VectorTile.GetDataOptions getDataOptions = new VectorTile.GetDataOptions();
        getDataOptions.compression = Compression.gzip;
        getDataOptions.level = 9;
        VectorTile.Info info = VectorTile.info(vt.getData(getDataOptions));

        image.saveToFile("test/test.png", "png");
        Image image2 = new Image(4096, 4096);
        m.renderImage(image2);
        image2.saveToFile("test/test2.png", "png");

        File file = new File("test/test.pbf");
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

        Box2d box = vt.extent();
        VectorTile.QueryResult queryResult = vt.query((box.maxx + box.minx) / 2, (box.maxy + box.miny) / 2);

        double[][] array = new double[1][2];
        array[0][0] = 0;
        array[0][1] = 0;
        VectorTile.QueryManyResult queryManyResult = vt.queryMany(array, "countries");

        m.close();
        vt.close();
        image.close();
        image2.close();
    }
}
