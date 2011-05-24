package tray;

import java.awt.Image;
import java.net.URL;

import org.junit.Assert;
import org.junit.Test;

public class ImageLoaderTest {
	@Test
	public void load() {
		URL resource = getClass().getResource("sample.png");
		Image image = ImageLoader.load(resource);
		int actualWidth = image.getWidth(null);
		int samplePngKnownSize = 417;
		Assert.assertEquals(samplePngKnownSize, actualWidth);
	}
	
	@Test
	public void loadSvg() {
		URL resource = getClass().getResource("sample.svg");
		Image image = ImageLoader.load(resource);
		int actualWidth = image.getWidth(null);
		Assert.assertEquals(ImageLoader.getSizeForCurrentOS(), actualWidth);
	}
}
