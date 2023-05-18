import java.io.File;
import java.io.FileOutputStream;

import mapnik.Box2d;
import mapnik.Image;
import mapnik.MapDefinition;
import mapnik.Mapnik;
import mapnik.Projection;
import mapnik.Renderer;
import mapnik.Layer;

public class App {
    public static void main(String[] args) throws Exception {
        Mapnik.initialize();

        // France métropolitaine
        // -9.86 41.15 : -1097610.18 5034491.87
        // 10.38 51.56 : 1155496.31 6720955.49
        Box2d bounds = new Box2d(-1097610.18, 5034491.87, 1155496.31, 6720955.49);
        MapDefinition m = new MapDefinition();

        m.loadMap(new File("data/france.xml").getAbsolutePath(), false);
        // Programatically get Aquitaine and set it to red Style
        Layer aquitaine = m.getLayer(1);
        aquitaine.setStyles(new String[] { "red" });
        m.removeLayer(1);
        m.addLayer(aquitaine);

        m.setSrs(Projection.SRS900913_PARAMS);
        m.resize(4096, 4096);
        m.zoomToBox(bounds);

        Image image = new Image(4096, 4096);
        Renderer.renderAgg(m, image);

        byte[] contents = image.saveToMemory("png");
        image.close();
        m.close();

        FileOutputStream fos = new FileOutputStream("result.png");
        fos.write(contents);
        fos.flush();
        fos.close();
    }
}
