package tray.mocks;

import java.awt.TrayIcon.MessageType;
import java.util.Collection;
import java.util.LinkedHashMap;

import org.apache.commons.lang.NotImplementedException;
import org.apache.commons.lang.StringUtils;

import tray.linux.NativeTray;

public class NativeTrayMock implements NativeTray {

	private String file;
	private String tooltip;
	private LinkedHashMap<Integer, String> linkedHashMap = new LinkedHashMap<Integer, String>();
	private StringBuffer operations = new StringBuffer();

	@Override
	public int nativeCreateTrayIcon(String file, String tooltip) {
		this.file = file;
		this.tooltip = tooltip;
		return 0;
		
	}

	@Override
	public void nativeAddMenuItem(int nativeId, int menuItemIndex, String label) {
		linkedHashMap.put(menuItemIndex, label);
	}

	@Override
	public void nativeDisplayMessage(int nativeId, String title, String caption,
			MessageType info) {
		operations.append(String.format("nativeDisplayMessage(%s,%s,%s)", title, caption, info));
	}

	@Override
	public void nativeSetAutosize(int nativeId, boolean autosize) {
		operations.append("nativeSetAutosize("+autosize+")\n");
	}

	public String getFile() {
		return this.file;
	}

	public String getTooltip() {
		return this.tooltip;
	}

	public String getDeclaredMenuItems() {
		Collection<String> values = linkedHashMap.values();
		String[] array = values.toArray(new String[0]);
		
		return StringUtils.join(array,",");
	}

	public String getOperations() {
		return operations.toString().trim();
	}

	@Override
	public void nativeSetImage(int nativeId, String file) {
		throw new NotImplementedException();
	}

	@Override
	public void nativeRemoveMe(int nativeId) {
		throw new NotImplementedException();
	}

}
