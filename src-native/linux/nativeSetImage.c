#include "commonFunctions.h"

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeSetImage0
  (JNIEnv *env, jobject invoker, jint nativeId, jstring file)
{
	jboolean isCopy;
	const char *fileChar= (*env)->GetStringUTFChars(env, file, &isCopy);
	gtk_status_icon_set_from_file(nativeInstance[nativeId].trayIcon, fileChar);
	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, file, fileChar);
	}
}


