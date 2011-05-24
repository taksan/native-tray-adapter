package tray;

import tray.java.JavaTrayAdapter;
import tray.linux.LinuxNativeTrayAdapter;


public class SystemTrayProvider {

	public SystemTrayAdapter getSystemTray() {
		String osName = System.getProperty("os.name").toLowerCase();
		if (osName.startsWith("linux")) {
			return new LinuxNativeTrayAdapter();
		}
		return new JavaTrayAdapter();
	}
}
