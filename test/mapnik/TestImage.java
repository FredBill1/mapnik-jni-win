package mapnik;

import java.util.Arrays;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

import mapnik.outputformat.*;

public class TestImage {
    private MapDefinition map;
    private Image image;

    @BeforeClass
    public static void initMapnik() {
        Setup.initialize();
    }

    @AfterClass
    public static void tearDownMapnik() {
        Setup.tearDown();
    }

    @Before
    public void setUp() {
        map = new MapDefinition();
        image = new Image(128, 128);
        Renderer.renderAgg(map, image);
    }

    @After
    public void tearDown() {
        image.close();
        map.close();
    }

    @Test
    public void testSavetoMemoryWithJpegQuality() {
        JpegFormatStringBuilder formatStringBuilder = new JpegFormatStringBuilder();
        String formatString1 = formatStringBuilder.toString();
        String formatString2 = formatStringBuilder.withQuality(5).toString();
        byte[] contents1 = image.saveToMemory(formatString1);
        byte[] contents2 = image.saveToMemory(formatString2);

        assertEquals(formatString1, "jpeg");
        assertEquals(formatString2, "jpeg:quality=5");
        assert (!Arrays.equals(contents1, contents2));
    }

    @Test
    public void testSavetoMemoryWithTiffZlevel() {
        TiffFormatStringBuilder formatStringBuilder = new TiffFormatStringBuilder();
        String formatString1 = formatStringBuilder.toString();
        String formatString2 = formatStringBuilder.withZLevel(3).toString();
        byte[] contents1 = image.saveToMemory(formatString1);
        byte[] contents2 = image.saveToMemory(formatString2);

        assertEquals(formatString1, "tiff");
        assertEquals(formatString2, "tiff:zlevel=3");
        assert (!Arrays.equals(contents1, contents2));
    }

    @Test
    public void testSavetoMemoryWithTiffMethod() {
        TiffFormatStringBuilder formatStringBuilder = new TiffFormatStringBuilder()
                .withMethod(TiffFormatStringBuilder.Method.STRIPPED);
        String formatString1 = formatStringBuilder.toString();
        String formatString2 = formatStringBuilder.withMethod(TiffFormatStringBuilder.Method.SCANLINE).toString();
        byte[] contents1 = image.saveToMemory(formatString1);
        byte[] contents2 = image.saveToMemory(formatString2);

        assertEquals(formatString1, "tiff:method=stripped");
        assertEquals(formatString2, "tiff:method=scanline");
        assert (!Arrays.equals(contents1, contents2));
    }

    @Test
    public void testSavetoMemoryWithPalettedPng() {
        PngFormatStringBuilder formatStringBuilder = new PngFormatStringBuilder();
        String formatString1 = formatStringBuilder.toString();
        String formatString2 = formatStringBuilder.withPalette(true).toString();
        byte[] contents1 = image.saveToMemory(formatString1);
        byte[] contents2 = image.saveToMemory(formatString2);

        assertEquals(formatString1, "png");
        assertEquals(formatString2, "png8");
        assert (!Arrays.equals(contents1, contents2));
    }

    @Test
    public void testSaveToMemoryWebpWithAlpha() {
        WebpFormatStringBuilder formatStringBuilder = new WebpFormatStringBuilder();
        String formatString1 = formatStringBuilder.toString();
        String formatString2 = formatStringBuilder.withAlpha(true).toString();
        byte[] contents1 = image.saveToMemory(formatString1);
        byte[] contents2 = image.saveToMemory(formatString2);

        assertEquals(formatString1, "webp");
        assertEquals(formatString2, "webp:alpha=1");
        assert (!Arrays.equals(contents1, contents2));
    }
}
