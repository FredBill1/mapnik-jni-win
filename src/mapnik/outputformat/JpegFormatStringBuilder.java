package mapnik.outputformat;

import java.util.Map;

public class JpegFormatStringBuilder extends FormatStringBuilderBase {

    public JpegFormatStringBuilder() {
    }

    private JpegFormatStringBuilder(Map<String, String> options) {
        super(options);
    }

    @Override
    protected String getFormatName() {
        return "jpeg";
    }

    public JpegFormatStringBuilder withQuality(int quality) {
        if (quality < 0 || quality > 100) {
            throw new Error("Invalid quality. Expected: 0..100, actual: " + quality);
        }
        return new JpegFormatStringBuilder(addOption("quality", String.valueOf(quality)));
    }
}
