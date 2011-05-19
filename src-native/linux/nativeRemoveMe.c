#include "commonFunctions.h"

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeRemoveMe0
  (JNIEnv *env, jobject invoker, jint nativeId)
{
	gtk_status_icon_set_visible(nativeInstance[nativeId].trayIcon, 0);
	nativeInstanceCount--;
	if (nativeInstanceCount == 0)
		gtk_main_quit();
}

