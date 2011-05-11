package tray.linux;

import java.awt.TrayIcon.MessageType;

import org.apache.commons.lang.NotImplementedException;

import tray.NativeTray;

public class NativeLinuxTray implements NativeTray {

	@Override
	public void nativeInit(String file, String tooltip) {
		nativeInit0(file,tooltip);
	}

	@Override
	public void nativeAddMenuItem(int i, String label) {
		throw new NotImplementedException();
	}

	private native void nativeInit0(String file, String tooltip);
	
	private native void nativeAddMenuItem0(int menuItemIndex, String caption);
	
	
	private native void nativeDisplayMessage0(String title, String text,
			MessageType info);
	
	private native void nativeSetAutosize0(boolean autosize);

	@Override
	public void nativeDisplayMessage(String title, String caption,
			MessageType info) {
		throw new NotImplementedException();
	}

	@Override
	public void nativeSetAutosize(boolean autosize) {
		throw new NotImplementedException();
	}
}
