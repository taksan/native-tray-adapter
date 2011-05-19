#ifndef PRIVATE_DECLARATIONS__H__
#define PRIVATE_DECLARATIONS__H__
#include <gtk/gtk.h>
#include "tray_linux_NativeLinuxTray.h"

typedef struct {
	GtkStatusIcon *trayIcon;
	GtkWidget *trayIconMenu;
} TrayIconInstance;

typedef struct {
	int nativeId;
	char * iconFileName;
	char * tooltipText;
	pthread_mutex_t lockToWaitInit;
} TrayIconData;

jobject getLinuxTrayIconAdapter(JNIEnv* env, int nativeId);
void setTooltip(JNIEnv *env, GtkStatusIcon *trayIcon, jstring tooltip); 

void print_debugger(const char *fmt, ...); 

extern TrayIconInstance nativeInstance[10];
extern int nativeInstanceCount;
extern int nextInstanceId; 
extern JNIEnv * gtkMainThreadJniEnv;
extern JavaVM * javaVM;

#endif
