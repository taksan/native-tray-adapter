package tray;

import java.awt.AWTException;
import java.awt.Image;
import java.awt.PopupMenu;
import java.awt.SystemTray;
import java.io.IOException;
import java.net.URL;

import svg.SvgImage;

public class JavaTrayAdapter implements SystemTrayAdapter {
	
	SystemTray systemTray = SystemTray.getSystemTray();

	@Override
	public void add(TrayIconAdapter trayIcon) {
		JavaIconAdapter adapter = (JavaIconAdapter)trayIcon;
		try {
			systemTray.add(adapter.getTrayIcon());
		} catch (AWTException e) {
			throw new RuntimeException(e);
		}
	}

	@Override
	public TrayIconAdapter newNativeTrayIcon(URL imageURL, String string,
			PopupMenu popup) {
		return new JavaIconAdapter(makeSvgImage(imageURL), string, popup);
	}
	
	private Image makeSvgImage(URL imageURL) {
		Image image;
		try {
			image = new SvgImage(imageURL).getImage(24, 24);
		} catch (IOException e1) {
			throw new RuntimeException(e1);
		}
		return image;
	}

}
