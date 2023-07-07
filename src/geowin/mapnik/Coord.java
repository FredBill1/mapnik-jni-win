package geowin.mapnik;

import java.text.DecimalFormat;

public class Coord {
    public double x;
    public double y;

    public Coord() {
    }

    public Coord(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public Coord(Coord other) {
        this.x = other.x;
        this.y = other.y;
    }

    @Override
    public String toString() {
        DecimalFormat fmt = new DecimalFormat("0.0");
        return "Coord(" + fmt.format(x) + "," + fmt.format(y) + ")";
    }
}
