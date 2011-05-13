package tray.linux;

import java.awt.Point;
import java.awt.TrayIcon.MessageType;

import tray.NativeTray;
import tray.balloon.Balloon;

public class NativeLinuxTray implements NativeTray {
	@Override
	public void nativeInit(String file, String tooltip) {
		nativeInit0(file, tooltip);
	}

	@Override
	public void nativeAddMenuItem(int menuItemIndex, String caption) {
		nativeAddMenuItem0(menuItemIndex, caption);
	}

	@Override
	public void nativeSetAutosize(boolean autosize) {
		nativeSetAutosize0(autosize);
	}

	@Override
	public void displayTrayIcon() {
		nativeDisplayTrayIcon();
	}

	@Override
	public void nativeDisplayMessage(final String caption, final String text,
			final MessageType messageType) {
		Point loc = nativeGetIconLocation0();
		final Balloon balloon = new Balloon();
		balloon.setBounds(loc.x+12, loc.y+12, 1,1);
		balloon.display(caption, text, messageType);
	}

	private native void nativeInit0(String file, String tooltip);

	private native void nativeAddMenuItem0(int menuItemIndex, String caption);

	private native void nativeDisplayTrayIcon();

	private native void nativeDisplayMessage0(String title, String caption,
			MessageType info);

	private native void nativeSetAutosize0(boolean autosize);

	private native Point nativeGetIconLocation0();
}
