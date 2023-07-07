package geowin.mapnik.outputformat;

import java.util.Map;

public class WebpFormatStringBuilder extends FormatStringBuilderBase {

    public WebpFormatStringBuilder() {
    }

    private WebpFormatStringBuilder(Map<String, String> options) {
        super(options);
    }

    @Override
    protected String getFormatName() {
        return "webp";
    }

    public WebpFormatStringBuilder withQuality(int quality) {
        if (quality < 0 || quality > 100) {
            throw new Error("Invalid quality. Expected: 0..100, actual: " + quality);
        }
        return new WebpFormatStringBuilder(addOption("quality", String.valueOf(quality)));
    }

    public WebpFormatStringBuilder withMethod(int method) {
        if (method < 0 || method > 6) {
            throw new Error("Invalid method. Expected: 0..6, actual: " + method);
        }
        return new WebpFormatStringBuilder(addOption("method", String.valueOf(method)));
    }

    public WebpFormatStringBuilder withLossless(boolean lossless) {
        return new WebpFormatStringBuilder(addOption("lossless", lossless ? "1" : "0"));
    }

    public WebpFormatStringBuilder withImageHint(int imageHint) {
        if (imageHint < 0 || imageHint > 3) {
            throw new Error("Invalid imageHint. Expected: 0..3, actual: " + imageHint);
        }
        return new WebpFormatStringBuilder(addOption("image_hint", String.valueOf(imageHint)));
    }

    public WebpFormatStringBuilder withAlpha(boolean alpha) {
        return new WebpFormatStringBuilder(addOption("alpha", alpha ? "1" : "0"));
    }

    public WebpFormatStringBuilder withTargetSize(int targetSize) {
        return new WebpFormatStringBuilder(addOption("target_size", String.valueOf(targetSize)));
    }

    public WebpFormatStringBuilder withTargetPsnr(double targetPsnr) {
        return new WebpFormatStringBuilder(addOption("target_psnr", String.valueOf(targetPsnr)));
    }

    public WebpFormatStringBuilder withSegments(int segments) {
        return new WebpFormatStringBuilder(addOption("segments", String.valueOf(segments)));
    }

    public WebpFormatStringBuilder withSnsStrength(int snsStrength) {
        return new WebpFormatStringBuilder(addOption("sns_strength", String.valueOf(snsStrength)));
    }

    public WebpFormatStringBuilder withFilterStrength(int filterStrength) {
        return new WebpFormatStringBuilder(addOption("filter_strength", String.valueOf(filterStrength)));
    }

    public WebpFormatStringBuilder withFilterSharpness(int filterSharpness) {
        return new WebpFormatStringBuilder(addOption("filter_sharpness", String.valueOf(filterSharpness)));
    }

    public WebpFormatStringBuilder withFilterType(int filterType) {
        return new WebpFormatStringBuilder(addOption("filter_type", String.valueOf(filterType)));
    }

    public WebpFormatStringBuilder withAutofilter(int autofilter) {
        return new WebpFormatStringBuilder(addOption("autofilter", String.valueOf(autofilter)));
    }

    public WebpFormatStringBuilder withAlphaCompression(int alphaCompression) {
        return new WebpFormatStringBuilder(addOption("alpha_compression", String.valueOf(alphaCompression)));
    }

    public WebpFormatStringBuilder withAlphaFiltering(int alphaFiltering) {
        return new WebpFormatStringBuilder(addOption("alpha_filtering", String.valueOf(alphaFiltering)));
    }

    public WebpFormatStringBuilder withAlphaQuality(int alphaQuality) {
        return new WebpFormatStringBuilder(addOption("alpha_quality", String.valueOf(alphaQuality)));
    }

    public WebpFormatStringBuilder withPass(int pass) {
        return new WebpFormatStringBuilder(addOption("pass", String.valueOf(pass)));
    }

    public WebpFormatStringBuilder withPreprocessing(int preprocessing) {
        return new WebpFormatStringBuilder(addOption("preprocessing", String.valueOf(preprocessing)));
    }

    public WebpFormatStringBuilder withPartitions(int partitions) {
        return new WebpFormatStringBuilder(addOption("partitions", String.valueOf(partitions)));
    }

    public WebpFormatStringBuilder withPartitionLimit(int partitionLimit) {
        return new WebpFormatStringBuilder(addOption("partition_limit", String.valueOf(partitionLimit)));
    }
}
