package tray;

import java.awt.TrayIcon.MessageType;
import java.awt.event.ActionListener;

public interface TrayIconAdapter {

	public void displayMessage(String caption, String text, MessageType messageType);

	public void setImageAutoSize(boolean autoSize);

	public void addActionListener(ActionListener actionListener);

}
