package tray;

import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.TrayIcon.MessageType;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.URL;

class LinuxTrayIconAdapter implements TrayIconAdapter, NativeLinuxTrayListener {

	private ActionListener actionListener;
	private final NativeTray nativeTray;

	public LinuxTrayIconAdapter(NativeTray nativeTray, URL imageURL,
			String tooltip, PopupMenu popup) {
		this.nativeTray = nativeTray;
		nativeTray.nativeInit(imageURL.getFile(), tooltip);
		populateNativeMenuListeners(popup);
	}

	private void populateNativeMenuListeners(PopupMenu popup) {
		int i;
		for (i = 0; i < popup.getItemCount(); i++) {
			MenuItem item = popup.getItem(i);
			nativeTray.nativeAddMenuItem(item, item.getLabel());
		}
		nativeTray.displayTrayIcon();
	}

	@Override
	public void displayMessage(String title, String text, MessageType info) {
		nativeTray.nativeDisplayMessage(title, text, info);
	}

	@Override
	public void setImageAutoSize(boolean autosize) {
		nativeTray.nativeSetAutosize(autosize);
	}

	@Override
	public void addActionListener(ActionListener actionListener) {
		this.actionListener = actionListener;
	}

	@Override
	public void fireActionActivated() {
		ActionEvent e = new ActionEvent(this, 0, "activate");
		this.actionListener.actionPerformed(e);
	}

	@Override
	public void fireMenuAction(MenuItem item) {
		ActionListener[] actionListeners = item.getActionListeners();
		ActionEvent e = new ActionEvent(item, 0,
				item.getActionCommand());
		for (ActionListener actionListener : actionListeners) {
			actionListener.actionPerformed(e);
		}
	}
}