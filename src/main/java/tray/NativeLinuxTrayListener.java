package tray;


public interface NativeLinuxTrayListener {

	void fireActionActivated();

	void fireMenuAction(int menuItemIndex);
}
