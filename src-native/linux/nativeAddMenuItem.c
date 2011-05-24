#include <stdlib.h>
#include "commonFunctions.h"

#define MENU_ACTION_CALLBACK_METHOD_NAME "fireMenuAction"

typedef struct  {
	int nativeId;
	int itemIndex;
} MenuActivationData;

MenuActivationData * createActivationData(int nativeId, int itemIndex);
void menuItemActivationHandler(GtkMenuItem *item, gpointer menuItemIndex);
void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
GtkWidget * createMenuItem(JNIEnv *env, jstring caption);

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeAddMenuItem0
  (JNIEnv *env, jobject invokingObject, jint nativeId, jint itemIndex, jstring caption)
{
	GtkWidget *menuItem = createMenuItem(env, caption);
	TrayIconInstance * instance = &nativeInstance[nativeId];
	gtk_menu_shell_append (GTK_MENU_SHELL (instance->trayIconMenu), menuItem);

	MenuActivationData * activationData = createActivationData(nativeId, (int)itemIndex);
	g_signal_connect (G_OBJECT (menuItem), "activate", G_CALLBACK (menuItemActivationHandler), (gpointer)activationData);

	gtk_widget_show_all (instance->trayIconMenu);	
}

void menuItemActivationHandler(GtkMenuItem *item, gpointer activationDataPointer)
{
	MenuActivationData * activationData = (MenuActivationData*)activationDataPointer;
	int menuItemIndex = activationData->itemIndex;

	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv, activationData->nativeId);
	jclass classRef = (*gtkMainThreadJniEnv)->GetObjectClass(gtkMainThreadJniEnv, trayAdapter);

	jmethodID mid = (*gtkMainThreadJniEnv)->GetMethodID(gtkMainThreadJniEnv, classRef, MENU_ACTION_CALLBACK_METHOD_NAME, "(I)V");
	if (mid == 0) {
		return;
	}
	(*gtkMainThreadJniEnv)->CallVoidMethod(gtkMainThreadJniEnv, trayAdapter, mid, menuItemIndex);
}

GtkWidget * createMenuItem(JNIEnv *env, jstring caption)
{
	jboolean isCopy;
	const char *menuItemName= (*env)->GetStringUTFChars(env, caption, &isCopy);
	GtkWidget *menuItem = gtk_menu_item_new_with_label (menuItemName);
	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, caption, menuItemName);
	}   
	return menuItem;
}

MenuActivationData * createActivationData(int nativeId, int itemIndex)
{
	MenuActivationData * activationData = (MenuActivationData*)malloc(sizeof(MenuActivationData)); 
	activationData->nativeId = nativeId;
	activationData->itemIndex = (int)itemIndex;
	return activationData;
}


