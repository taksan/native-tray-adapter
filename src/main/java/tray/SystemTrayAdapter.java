package tray;

import java.awt.PopupMenu;
import java.net.URL;


public interface SystemTrayAdapter {

	void add(TrayIconAdapter trayIcon);

	TrayIconAdapter newNativeTrayIcon(URL imageUrl, String tooltip, PopupMenu popup);


}
