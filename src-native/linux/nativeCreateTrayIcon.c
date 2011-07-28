#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdarg.h>
#include "tray_linux_NativeLinuxTray.h"

#include "commonFunctions.h"

#define ACTION_CALLBACK_METHOD_NAME "fireActionActivated"

void trayIconActivatedHandler(GObject *trayIcon, gpointer data);
void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);

gboolean createTrayIconWhenGtkMainIsIdle(TrayIconData * trayIconData);
char * convertJStringToCString(JNIEnv * env, jstring aString); 
void initializeMutexToWaitUntilActualTrayIconCreation(TrayIconData * trayIconData);
void waitUntilTheTrayIconIsCreatedWhenGtkMainIsIdle(TrayIconData * trayIconData);

GtkWidget * createTrayIconMenu(GtkStatusIcon *trayIcon);
TrayIconData * createNewTrayIconData(char * file, char * tooltip);

int getNextInstanceId();

JNIEXPORT int JNICALL Java_tray_linux_NativeLinuxTray_nativeCreateTrayIcon0
  (JNIEnv *env, jobject invokingObject, jstring file, jstring tooltip)
{
	(void) invokingObject;// avoid warning
	char * iconFileName = convertJStringToCString(env, file);
	char * tooltipText  = convertJStringToCString(env, tooltip);

	return createTrayIconForIconAndWithTooltip(iconFileName, tooltipText);
}

int createTrayIconForIconAndWithTooltip(char * iconFileName, char * tooltipText)
{
	TrayIconData * trayIconData = createNewTrayIconData(iconFileName, tooltipText);

	initializeMutexToWaitUntilActualTrayIconCreation(trayIconData);

	g_idle_add((GSourceFunc)createTrayIconWhenGtkMainIsIdle, trayIconData);

	waitUntilTheTrayIconIsCreatedWhenGtkMainIsIdle(trayIconData);

	int newInstanceId = trayIconData->nativeId;
	free(trayIconData);
	return newInstanceId;
}


gboolean createTrayIconWhenGtkMainIsIdle(TrayIconData * trayIconData)
{
	GtkStatusIcon *trayIcon = gtk_status_icon_new_from_file (trayIconData->iconFileName);
	TrayIconInstance * trayIconInstance = &nativeInstance[trayIconData->nativeId];
	trayIconInstance->trayIcon = trayIcon;

	gtk_status_icon_set_tooltip (trayIcon, trayIconData->tooltipText);

	trayIconInstance->trayIconMenu = createTrayIconMenu(trayIcon);
	gtk_widget_show_all (trayIconInstance->trayIconMenu);

	int * nativeIdParam = (int*)malloc(sizeof(int));
	*nativeIdParam = trayIconData->nativeId;
	g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivatedHandler), nativeIdParam);

	gtk_status_icon_set_visible(trayIcon, TRUE);


	printf("Will lock before sending signal\n");
	pthread_mutex_lock(&trayIconData->lockToWaitInit);


	printf("Will send signal\n");
	pthread_cond_signal(&trayIconData->condToWaitInit);

	printf("Signal sent, releasing lock\n");
	pthread_mutex_unlock(&trayIconData->lockToWaitInit);
	printf("Lock released\n");

	return 0;
}

TrayIconData * createNewTrayIconData(char * file, char * tooltip)
{
	int newInstanceId = getNextInstanceId();
	TrayIconData * trayIconData;
	trayIconData = (TrayIconData*)malloc(sizeof(TrayIconData));
	trayIconData->nativeId = newInstanceId;
	trayIconData->iconFileName = strdup(file);
	trayIconData->tooltipText  = strdup(tooltip);
	return trayIconData;
}

void initializeMutexToWaitUntilActualTrayIconCreation(TrayIconData * trayIconData)
{
	pthread_mutex_init(&trayIconData->lockToWaitInit, NULL);
	pthread_cond_init(&trayIconData->condToWaitInit, NULL);
	pthread_mutex_lock(&trayIconData->lockToWaitInit);
}

void waitUntilTheTrayIconIsCreatedWhenGtkMainIsIdle(TrayIconData * trayIconData)
{
	printf(" -- will wait condition -- \n");
	// wait until the tray icon is created
	pthread_cond_wait(&trayIconData->condToWaitInit, &trayIconData->lockToWaitInit);
	pthread_mutex_unlock(&trayIconData->lockToWaitInit);

	printf(" -- condition met -- \n");
}

void trayIconActivatedHandler(GObject *trayIcon, gpointer nativeIdPointer)
{
	(void) trayIcon; // avoid warning
	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv, *((int*)nativeIdPointer));
	jclass classRef = (*gtkMainThreadJniEnv)->GetObjectClass(gtkMainThreadJniEnv, trayAdapter);

	jmethodID mid = (*gtkMainThreadJniEnv)->GetMethodID(gtkMainThreadJniEnv, classRef, ACTION_CALLBACK_METHOD_NAME, "()V");

	if (mid == 0) {
		return;
	}

	(*gtkMainThreadJniEnv)->CallVoidMethod(gtkMainThreadJniEnv, trayAdapter, mid, NULL);
}

char * convertJStringToCString(JNIEnv * env, jstring aString) 
{
	jboolean isCopy;
	const char *aStringChar= (*env)->GetStringUTFChars(env, aString, &isCopy);

	char * copy = (char*)malloc(strlen(aStringChar));
	strcpy(copy, aStringChar);

	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, aString, aStringChar);
	}
	return copy;
}

int getNextInstanceId() {
	int newInstanceId = nextInstanceId;
	nativeInstanceCount++;
	nextInstanceId++;
	return newInstanceId;
}

GtkWidget *createTrayIconMenu(GtkStatusIcon *trayIcon)
{
	GtkWidget *trayIconMenu = gtk_menu_new();

    g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopupHandler), trayIconMenu);

	return trayIconMenu;
}

void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

