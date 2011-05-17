package tray;

import java.awt.PopupMenu;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import jni.utils.JniUtils;
import jni.utils.StreamUtils;
import tray.linux.NativeLinuxTray;

public class LinuxNativeTrayAdapter implements SystemTrayAdapter {
	
	private static boolean isLoaded = false;
	private static LinuxTrayIconAdapter linuxTrayIconAdapter;

	public LinuxNativeTrayAdapter() {
		if (!isLoaded ) {
			JniUtils.loadLibrary("linuxtray_x64");
			isLoaded = true;
		}
	}

	@Override
	public void add(TrayIconAdapter trayIcon) {
		// nothing to do, really.
	}

	@Override
	public TrayIconAdapter newNativeTrayIcon(URL imageURL, String tooltip,
			PopupMenu popup) {
		try {
			URL extractedImageFileUrl = extractImageAndGetTempUrl(imageURL);
			return new LinuxTrayIconAdapter(new NativeLinuxTray(), extractedImageFileUrl,tooltip,popup);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	private URL extractImageAndGetTempUrl(URL imageURL) throws IOException,
			MalformedURLException {
		File imageFile;
		imageFile = StreamUtils.getStreamAsTempFileOrCry(imageURL.openStream(), "trayIconRehydratedImage");
		URL extractedImageFileUrl = imageFile.toURI().toURL();
		return extractedImageFileUrl;
	}
	
	public static LinuxTrayIconAdapter getLinuxTrayIconAdapter() {
		return linuxTrayIconAdapter;
	}
}