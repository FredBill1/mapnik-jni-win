package geowin.mapnik;

public class Demo {
    public static void main(String[] args) {
        Mapnik.initialize();
        // France métropolitaine
        // -9.86 41.15 : -1097610.18 5034491.87
        // 10.38 51.56 : 1155496.31 6720955.49
        Box2d bounds = new Box2d(-1097610.18, 5034491.87, 1155496.31, 6720955.49);
        try (MapDefinition m = new MapDefinition()) {
            m.load("test/data/france.xml");
            // Programatically get Aquitaine and set it to red Style
            try (Layer aquitaine = m.getLayer(1)) {
                aquitaine.setStyles(new String[] { "red" });
                m.removeLayer(1);
                m.addLayer(aquitaine);
            }
            m.setSrs(Projection.SRS900913_PARAMS);
            m.resize(4096, 4096);
            m.zoomToBox(bounds);

            try (Image image = new Image(4096, 4096)) {
                m.render(image);
                image.saveToFile("test.png", "png");
            }
        }
        System.out.println("Done.");
    }
}
