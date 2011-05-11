package tray;

import java.awt.MenuItem;

public interface NativeLinuxTrayListener {

	void fireActionActivated();

	void fireMenuAction(MenuItem item);

}
