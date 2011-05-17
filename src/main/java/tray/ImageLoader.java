package tray;

import java.awt.Image;
import java.awt.Toolkit;
import java.io.IOException;
import java.net.URL;

import svg.SvgImage;

public class ImageLoader {
	public static Image load(URL imageURL) {
		try {
			if (imageURL.getFile().endsWith(".svg")) {
				int width = getSizeForCurrentOS();
				int height = width;
				return new SvgImage(imageURL).getImage(width, height);
			}
			return Toolkit.getDefaultToolkit().getImage(imageURL);
		} catch (IOException e1) {
			throw new RuntimeException(e1);
		}
	}

	private static int getSizeForCurrentOS() {
		String osName = System.getProperty("os.name").toLowerCase();
		if (osName.contains("windows")) {
			return 16;
		}
		return 24;
	}
}
