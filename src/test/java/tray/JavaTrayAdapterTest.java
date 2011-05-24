package tray;

import java.awt.Image;
import java.awt.PopupMenu;
import java.net.URL;

import junit.framework.Assert;

import org.junit.Test;

import tray.java.JavaIconAdapter;
import tray.java.JavaTrayAdapter;

public class JavaTrayAdapterTest {
	@Test
	public void testCreation() {
		JavaTrayAdapter javaTrayAdapter = new JavaTrayAdapter();
		PopupMenu popup = new PopupMenu();
		URL imageURL = this.getClass().getResource("sample.svg");
		JavaIconAdapter trayIcon = (JavaIconAdapter) javaTrayAdapter.createAndAddTrayIcon(imageURL, "a tool tip", popup);
		Image loadedImage = trayIcon.getTrayIcon().getImage();
		Assert.assertNotNull(loadedImage);
	}
}
