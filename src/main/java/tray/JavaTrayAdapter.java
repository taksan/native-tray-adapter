package tray;

import java.awt.AWTException;
import java.awt.Image;
import java.awt.PopupMenu;
import java.awt.SystemTray;
import java.awt.Toolkit;
import java.io.IOException;
import java.net.URL;

import svg.SvgImage;

public class JavaTrayAdapter implements SystemTrayAdapter {

	SystemTray systemTray = SystemTray.getSystemTray();

	@Override
	public void add(TrayIconAdapter trayIcon) {
		JavaIconAdapter adapter = (JavaIconAdapter) trayIcon;
		try {
			systemTray.add(adapter.getTrayIcon());
		} catch (AWTException e) {
			throw new RuntimeException(e);
		}
	}

	@Override
	public TrayIconAdapter newNativeTrayIcon(URL imageURL, String tooltip,
			PopupMenu popup) {
		return new JavaIconAdapter(makeImage(imageURL), tooltip, popup);
	}

	private Image makeImage(URL imageURL) {
		try {
			if (imageURL.getFile().endsWith(".svg")) {
				return new SvgImage(imageURL).getImage(24, 24);
			}
			return Toolkit.getDefaultToolkit().getImage(imageURL);
		} catch (IOException e1) {
			throw new RuntimeException(e1);
		}
	}
}
