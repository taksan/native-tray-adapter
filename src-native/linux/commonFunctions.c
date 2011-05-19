#include <stdarg.h>
#include "commonFunctions.h"

JNIEnv * gtkMainThreadJniEnv = NULL;
TrayIconInstance nativeInstance[10];
int nativeInstanceCount = 0;
int nextInstanceId = 0;
JavaVM *javaVM = NULL;

jobject getLinuxTrayIconAdapter(JNIEnv* env, int nativeId) {
	
	jclass classRef = (*gtkMainThreadJniEnv)->FindClass(gtkMainThreadJniEnv, "tray/LinuxNativeTrayAdapter");
	jmethodID mid = (*env)->GetStaticMethodID(env, classRef, "getLinuxTrayIconAdapter", "(I)Ltray/LinuxTrayIconAdapter;");
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
