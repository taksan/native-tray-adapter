package tray;

import java.awt.AWTException;
import java.awt.PopupMenu;
import java.awt.SystemTray;
import java.net.URL;


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
		return new JavaIconAdapter(imageURL, tooltip, popup);
	}
}
