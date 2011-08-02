#include <stdarg.h>
#include "commonFunctions.h"
#define LINUX_NATIVE_TRAY_ADAPTER_CLASS "tray/linux/LinuxNativeTrayAdapter"
#define JAVA_TRAY_ICON_ADAPTER_CLASS "tray/linux/LinuxTrayIconAdapter"

JNIEnv * gtkMainThreadJniEnv = NULL;
TrayIconInstance nativeInstance[10];
int nativeInstanceCount = 0;
int nextInstanceId = 0;
JavaVM *javaVM = NULL;

jobject getLinuxTrayIconAdapter(JNIEnv* env, int nativeId) {
	if (gtkMainThreadJniEnv == NULL) {
		return NULL;
	}
	jclass classRef = (*gtkMainThreadJniEnv)->FindClass(gtkMainThreadJniEnv, LINUX_NATIVE_TRAY_ADAPTER_CLASS);
	jmethodID mid = (*env)->GetStaticMethodID(env, classRef, "getLinuxTrayIconAdapter", "(I)L" JAVA_TRAY_ICON_ADAPTER_CLASS ";");
	if (mid == 0) {
		return NULL;
	}
	return (*env)->CallStaticObjectMethod(env, classRef, mid, nativeId);
}

void print_debugger(const char *fmt, ...) 
{
	va_list ap;
    va_start(ap, fmt);

    vprintf(fmt, ap);

    printf("\n");
    fflush(stdin);
    va_end(ap);
}
