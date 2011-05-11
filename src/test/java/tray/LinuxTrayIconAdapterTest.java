package tray;

import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.TrayIcon.MessageType;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.LinkedHashMap;

import org.junit.Assert;
import org.junit.Test;

import tray.mocks.NativeTrayMock;

public class LinuxTrayIconAdapterTest {

	@Test
	public void testAdapter() throws MalformedURLException
	{
		NativeTrayMock nativeTrayMock = new NativeTrayMock();
		LinuxTrayIconAdapter linuxTrayIconAdapter = makeSubject(nativeTrayMock);
		
		Assert.assertEquals("/tmp", nativeTrayMock.getFile());
		Assert.assertEquals("bogus", nativeTrayMock.getTooltip());
		Assert.assertEquals("foo,bar", nativeTrayMock.getDeclaredMenuItems());
		
		linuxTrayIconAdapter.setImageAutoSize(false);
		linuxTrayIconAdapter.displayMessage("foo", "bar", MessageType.INFO);
		
		Assert.assertEquals(
				"nativeSetAutosize(false)\n" + 
				"nativeDisplayMessage(foo,bar,INFO)",
				nativeTrayMock.getOperations());
	}

	final StringBuffer actionsPerformed = new StringBuffer();
	@Test
	public void testAdapterNativeListener() throws MalformedURLException {
		NativeTrayMock nativeTrayMock = new NativeTrayMock();
		LinuxTrayIconAdapter linuxTrayIconAdapter = makeSubject(nativeTrayMock);
		
		ActionListener actionListener = new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				actionsPerformed.append("fireActionActivated\n");
			}
		};
		linuxTrayIconAdapter.addActionListener(actionListener);
		linuxTrayIconAdapter.fireActionActivated();
		linuxTrayIconAdapter.fireMenuAction(produceMenuItem("bar"));
		
		String actionList = actionsPerformed.toString().trim();
		Assert.assertEquals(
				"fireActionActivated\n" + 
				"bar-0 java.awt.MenuItem[menuitem0,label=bar]", 
				actionList);
	}

	private LinuxTrayIconAdapter makeSubject(NativeTrayMock nativeTrayMock)
	throws MalformedURLException {
		URL imageURL = new URL("file:/tmp");
		String tooltip = "bogus";
		PopupMenu popup = new PopupMenu();
		popup.add(produceMenuItem("foo"));
		popup.add(produceMenuItem("bar"));
		
		LinuxTrayIconAdapter linuxTrayIconAdapter = new LinuxTrayIconAdapter(nativeTrayMock, imageURL, tooltip, popup);
		return linuxTrayIconAdapter;
	}
	
	LinkedHashMap<String,MenuItem> items = new LinkedHashMap<String, MenuItem>();
	private MenuItem produceMenuItem(String caption) {
		if (items.containsKey(caption)) {
			return items.get(caption);
		}
		MenuItem menuItem = new MenuItem(caption);
		ActionListener l = new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				actionsPerformed.append(
						String.format("%s-%s %s", e.getActionCommand(), e.getID(), e.getSource() )
						);
			}
		};
		menuItem.addActionListener(l);
		items.put("caption", menuItem);
		return menuItem;
	}
}
