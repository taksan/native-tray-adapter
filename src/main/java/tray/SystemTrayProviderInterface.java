package tray;

public interface SystemTrayProviderInterface {

	public abstract SystemTrayAdapter getSystemTray();

	public abstract boolean isSupported();

}