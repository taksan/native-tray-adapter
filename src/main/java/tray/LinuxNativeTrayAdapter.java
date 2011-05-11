package tray;

import java.awt.PopupMenu;
import java.net.URL;

import com.skype.connector.ConnectorUtils;

import tray.linux.NativeLinuxTray;

public class LinuxNativeTrayAdapter implements SystemTrayAdapter {
	
	private static boolean isLoaded = false;
	private static LinuxTrayIconAdapter linuxTrayIconAdapter;

	public LinuxNativeTrayAdapter() {
		if (!isLoaded ) {
			ConnectorUtils.loadLibrary("linuxtray_x64");
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
		linuxTrayIconAdapter = new LinuxTrayIconAdapter(new NativeLinuxTray(), imageURL,tooltip,popup);
		return linuxTrayIconAdapter;
	}
	
	public static LinuxTrayIconAdapter getLinuxTrayIconAdapter() {
		return linuxTrayIconAdapter;
	}
}