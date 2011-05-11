package tray;

import java.awt.PopupMenu;
import java.net.URL;

import tray.linux.NativeLinuxTray;

public class LinuxNativeTrayAdapter implements SystemTrayAdapter {
	
	private static boolean isLoaded = false;

	public LinuxNativeTrayAdapter() {
		if (!isLoaded ) {
			System.loadLibrary("liblinuxtray_x64");
			isLoaded = true;
		}
	}

	@Override
	public void add(TrayIconAdapter trayIcon) {
		// nothing to do, really.
	}

	@Override
	public TrayIconAdapter newNativeTrayIcon(URL imageURL, String tooltip,
			PopupMenu popup) {
		return new LinuxTrayIconAdapter(new NativeLinuxTray(), imageURL,tooltip,popup);
	}
}