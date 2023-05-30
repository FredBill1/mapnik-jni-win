package mapnik.outputformat;

import java.util.Map;

public class PngFormatStringBuilder extends FormatStringBuilderBase {

    private final boolean paletted;

    public PngFormatStringBuilder() {
        paletted = false;
    }

    private PngFormatStringBuilder(boolean paletted, Map<String, String> options) {
        super(options);
        this.paletted = paletted;
    }

    @Override
    protected String getFormatName() {
        return paletted ? "png8" : "png";
    }

    public PngFormatStringBuilder withPalette(boolean paletted) {
        return new PngFormatStringBuilder(paletted, options);
    }

    public PngFormatStringBuilder withHextree(boolean useHextree) {
        return new PngFormatStringBuilder(paletted, addOption("m", useHextree ? "h" : "o"));
    }

    public PngFormatStringBuilder withColors(int colors) {
        if (colors < 1 || colors > 256) {
            throw new Error("Invalid colors. Expected: 1..256, actual: " + colors);
        }
        return new PngFormatStringBuilder(paletted, addOption("c", String.valueOf(colors)));
    }

    public PngFormatStringBuilder withTransMode(int transMode) {
        if (transMode < 0 || transMode > 2) {
            throw new Error("Invalid transMode. Expected: 0..2, actual: " + transMode);
        }
        return new PngFormatStringBuilder(paletted, addOption("t", String.valueOf(transMode)));
    }

    public PngFormatStringBuilder withGamma(double gamma) {
        if (gamma < 0) {
            throw new Error("Invalid gamma. Expected: non-negative, actual: " + gamma);
        }
        return new PngFormatStringBuilder(paletted, addOption("g", String.valueOf(gamma)));
    }

    public PngFormatStringBuilder withCompression(int compression) {
        if (compression < -1 || compression > 9) {
            throw new Error("Invalid compression. Expected: -1..9, actual: " + compression);
        }
        return new PngFormatStringBuilder(paletted, addOption("z", String.valueOf(compression)));
    }

    public PngFormatStringBuilder withStrategy(Strategy strategy) {
        return new PngFormatStringBuilder(paletted, addOption("s", strategy.name().toLowerCase()));
    }

    public enum Strategy {
        DEFAULT,
        FILTERED,
        HUFF,
        RLE,
        FIXED
    }
}
