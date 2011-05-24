#include "commonFunctions.h"

#define JAVA_AWT_POINT_CLASS_REFERENCE "java/awt/Point"

JNIEXPORT jobject JNICALL Java_tray_linux_NativeLinuxTray_nativeGetIconLocation0
  (JNIEnv *env, jobject invokingObject, jint nativeId)
{
	GdkScreen                 *screen = NULL;
	GdkRectangle    rect;
	gtk_status_icon_get_geometry (nativeInstance[nativeId].trayIcon, &screen, &rect, NULL);

	jclass cls = (*env)->FindClass(env, JAVA_AWT_POINT_CLASS_REFERENCE);
	jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(II)V");

	jvalue args[2];
	args[0].i = rect.x;
	args[1].i = rect.y;
	return (*env)->NewObjectA(env, cls, constructor, args);
}


