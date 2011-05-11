package tray;

import java.awt.MenuItem;
import java.awt.TrayIcon.MessageType;

public interface NativeTray {

	void nativeInit(String file, String tooltip);

	void nativeAddMenuItem(MenuItem item, String label);

	void nativeDisplayMessage(String title, String caption, MessageType info);

	void nativeSetAutosize(boolean autosize);

	void displayTrayIcon();


}
