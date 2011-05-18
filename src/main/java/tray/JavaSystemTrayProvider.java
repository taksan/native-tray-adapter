package tray;


public class JavaSystemTrayProvider {

	public SystemTrayAdapter getSystemTray() {
		String osName = System.getProperty("os.name").toLowerCase();
		if (osName.startsWith("linux")) {
			return new LinuxNativeTrayAdapter();
		}
		return new JavaTrayAdapter();
	}
}
