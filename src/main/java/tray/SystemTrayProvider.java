package tray;

import java.awt.SystemTray;

import tray.java.JavaTrayAdapter;
import tray.linux.LinuxNativeTrayAdapter;


public class SystemTrayProvider implements SystemTrayProviderInterface {

	/* (non-Javadoc)
	 * @see tray.SystemTrayProviderInterface#getSystemTray()
	 */
	@Override
	public SystemTrayAdapter getSystemTray() {
		String osName = System.getProperty("os.name").toLowerCase();
		if (osName.startsWith("linux")) {
			try {
				return new LinuxNativeTrayAdapter();
			}catch(Exception e) {
				
			}
		}
		return new JavaTrayAdapter();
	}

	/* (non-Javadoc)
	 * @see tray.SystemTrayProviderInterface#isSupported()
	 */
	@Override
	public boolean isSupported() {
		return SystemTray.isSupported();
	}
}
