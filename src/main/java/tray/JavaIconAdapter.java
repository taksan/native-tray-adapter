package tray;

import java.awt.Image;
import java.awt.PopupMenu;
import java.awt.TrayIcon;
import java.awt.TrayIcon.MessageType;
import java.awt.event.ActionListener;

public class JavaIconAdapter implements TrayIconAdapter {

	private final TrayIcon trayIcon;

	public JavaIconAdapter(Image image, String tooltip, PopupMenu popup) {
		trayIcon = new TrayIcon(image, tooltip, popup);
	}

	@Override
	public void displayMessage(String caption, String text, MessageType messageType) {
		trayIcon.displayMessage(caption, text, messageType);
	}

	@Override
	public void setImageAutoSize(boolean autosize) {
		trayIcon.setImageAutoSize(autosize);
	}

	@Override
	public void addActionListener(ActionListener doubleClicklistener) {
		trayIcon.addActionListener(doubleClicklistener);
	}

	public TrayIcon getTrayIcon() {
		return trayIcon;
	}

}
