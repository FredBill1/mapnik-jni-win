package mapnik.outputformat;

import java.util.Map;

public class TiffFormatStringBuilder extends FormatStringBuilderBase {

    public TiffFormatStringBuilder() {
    }

    private TiffFormatStringBuilder(Map<String, String> options) {
        super(options);
    }

    @Override
    protected String getFormatName() {
        return "tiff";
    }

    public TiffFormatStringBuilder withCompression(Compression compression) {
        return new TiffFormatStringBuilder(addOption("compression", compression.name().toLowerCase()));
    }

    public TiffFormatStringBuilder withMethod(Method method) {
        return new TiffFormatStringBuilder(addOption("method", method.name().toLowerCase()));
    }

    public TiffFormatStringBuilder withZLevel(int zLevel) {
        if (zLevel < 0 || zLevel > 9) {
            throw new Error("Invalid zLevel. Expected: 0..9, actual: " + zLevel);
        }
        return new TiffFormatStringBuilder(addOption("zlevel", String.valueOf(zLevel)));
    }

    public TiffFormatStringBuilder withTileHeight(int tileHeight) {
        if (tileHeight < 0) {
            throw new Error("Invalid tileHeight. Expected: non-negative, actual: " + tileHeight);
        }
        return new TiffFormatStringBuilder(addOption("tile_height", String.valueOf(tileHeight)));
    }

    public TiffFormatStringBuilder withTileWidth(int tileWidth) {
        if (tileWidth < 0) {
            throw new Error("Invalid tileWidth. Expected: non-negative, actual: " + tileWidth);
        }
        return new TiffFormatStringBuilder(addOption("tile_width", String.valueOf(tileWidth)));
    }

    public TiffFormatStringBuilder withRowsPerStrip(int rowsPerStrip) {
        if (rowsPerStrip < 0) {
            throw new Error("Invalid rowsPerStrip. Expected: non-negative, actual: " + rowsPerStrip);
        }
        return new TiffFormatStringBuilder(addOption("rows_per_strip", String.valueOf(rowsPerStrip)));
    }

    public enum Compression {
        DEFLATE,
        NONE,
        LZW,
        ADOBEDEFLATE
    }

    public enum Method {
        SCANLINE,
        STRIPPED,
        TILED
    }
}
