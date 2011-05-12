package tray.linux;

import java.awt.TrayIcon.MessageType;

import tray.NativeTray;

public class NativeLinuxTray implements NativeTray {

	@Override
	public void nativeInit(String file, String tooltip) {
		nativeInit0(file,tooltip);
	}

	@Override
	public void nativeAddMenuItem(int menuItemIndex, String caption) {
		nativeAddMenuItem0(menuItemIndex, caption);
	}
	

	@Override
	public void nativeDisplayMessage(String title, String caption,
			MessageType info) {
		nativeDisplayMessage0(title, caption, info);
	}

	@Override
	public void nativeSetAutosize(boolean autosize) {
		nativeSetAutosize0(autosize);
	}

	@Override
	public void displayTrayIcon() {
		nativeDisplayTrayIcon();
	}	

	private native void nativeInit0(String file, String tooltip);
	
	private native void nativeAddMenuItem0(int menuItemIndex, String caption);
	
	private native void nativeDisplayTrayIcon();
	
	private native void nativeDisplayMessage0(String title, String caption, MessageType info);
	
	private native void nativeSetAutosize0(boolean autosize);

}
