#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdarg.h>
#include "tray_linux_NativeLinuxTray.h"

void createTrayIconMenu(GtkStatusIcon *trayIcon);
static void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);

void setTooltip(JNIEnv *env, GtkStatusIcon *trayIcon, jstring tooltip); 
GtkStatusIcon * createTrayIcon(JNIEnv *env, jstring file);
static jobject getLinuxTrayIconAdapter(JNIEnv* env); 
void print_and_flush(const char *fmt, ...); 

GtkStatusIcon *trayIcon;
GtkWidget *trayIconMenu;

JNIEnv * gtkMainThreadJniEnv = NULL;
JavaVM *javaVM;

static void trayIconActivatedHandler(GObject *trayIcon, gpointer data);
static void menuItemActivationHandler(GtkMenuItem *item, gpointer menuItemIndex);

void gtkMainThread(void * nothingUseful)
{
	(*javaVM)->AttachCurrentThread(javaVM, (void**) &gtkMainThreadJniEnv, NULL );
	gtk_main();
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeInit0
  (JNIEnv *env, jobject invokingObject, jstring file, jstring tooltip)
{
	(*env)->GetJavaVM(env, &javaVM);

	int argc = 0;
	char ** argv = malloc(sizeof(char*));
	argv[0] = "";
    gtk_init (&argc, &argv);

	trayIcon  = createTrayIcon(env, file);

	setTooltip(env, trayIcon, tooltip);
	createTrayIconMenu(trayIcon);

	g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivatedHandler), NULL);

	gtk_status_icon_set_visible(trayIcon, TRUE);
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeDisplayTrayIcon
  (JNIEnv *env, jobject invokingObject)
{
	gtk_widget_show_all (trayIconMenu);	
	pthread_t gtkMainThreadId;
	pthread_create(&gtkMainThreadId, NULL, gtkMainThread , NULL);
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeSetImage0
  (JNIEnv *env, jobject invoker, jstring file)
{
	jboolean isCopy;
	const char *fileChar= (*env)->GetStringUTFChars(env, file, &isCopy);

	gtk_status_icon_set_from_file(trayIcon, fileChar);

	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, file, fileChar);
	}
}


JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeAddMenuItem0
  (JNIEnv *env, jobject invokingObject, jint itemIndex, jstring caption)
{
	jboolean isCopy;
	GtkWidget *menuItem;
	const char *menuItemName= (*env)->GetStringUTFChars(env, caption, &isCopy);

	menuItem = gtk_menu_item_new_with_label (menuItemName);

	g_signal_connect (G_OBJECT (menuItem), "activate", G_CALLBACK (menuItemActivationHandler), (gpointer)itemIndex);
	gtk_menu_shell_append (GTK_MENU_SHELL (trayIconMenu), menuItem);

	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, caption, menuItemName);
	}   
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeDisplayMessage0
  (JNIEnv * env, jobject invokingObject, jstring title, jstring caption, jobject messageType)
{

}

JNIEXPORT jobject JNICALL Java_tray_linux_NativeLinuxTray_nativeGetIconLocation0
  (JNIEnv *env, jobject invokingObject)
{
	print_and_flush("Will get icon location");
	GdkScreen                 *screen = NULL;
	GdkRectangle    rect;

	gtk_status_icon_get_geometry (trayIcon,
			&screen,
			&rect,
			NULL);

	print_and_flush("location acquired");
	jclass cls = (*env)->FindClass(env, "java/awt/Point");
	jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(II)V");

	print_and_flush("Point constructor acquired");
	jvalue args[2];
	args[0].i = rect.x;
	args[1].i = rect.y;
	return (*env)->NewObjectA(env, cls, constructor, args);
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeSetAutosize0
  (JNIEnv * evn, jobject invokingObject, jboolean autoSize)
{
}

////////////////////////////////////////////////////////////////////////////////////////////

GtkStatusIcon * createTrayIcon(JNIEnv *env, jstring file) {
	jboolean isCopy;
	const char *fileChar= (*env)->GetStringUTFChars(env, file, &isCopy);

	GtkStatusIcon *newTrayIcon= gtk_status_icon_new_from_file (fileChar);

	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, file, fileChar);
	}
	return newTrayIcon;
}

void setTooltip(JNIEnv *env, GtkStatusIcon *trayIcon, jstring tooltip) {
	jboolean isCopy;
	const char *tooltipCharArray= (*env)->GetStringUTFChars(env, tooltip, &isCopy);

	gtk_status_icon_set_tooltip (trayIcon, tooltipCharArray);
	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, tooltip, tooltipCharArray);
	}   
}

void createTrayIconMenu(GtkStatusIcon *trayIcon)
{
	trayIconMenu = gtk_menu_new();

    g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopupHandler), trayIconMenu);
}

static void menuItemActivationHandler(GtkMenuItem *item, gpointer indexP)
{
	int menuItemIndex = (int) indexP;

	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv);
	jclass classRef = (*gtkMainThreadJniEnv)->GetObjectClass(gtkMainThreadJniEnv, trayAdapter);

	jmethodID mid = (*gtkMainThreadJniEnv)->GetMethodID(gtkMainThreadJniEnv, classRef, "fireMenuAction", "(I)V");
	if (mid == 0) {
		return;
	}
	(*gtkMainThreadJniEnv)->CallVoidMethod(gtkMainThreadJniEnv, trayAdapter, mid, menuItemIndex);
}

static void trayIconActivatedHandler(GObject *trayIcon, gpointer data)
{
	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv);
	jclass classRef = (*gtkMainThreadJniEnv)->GetObjectClass(gtkMainThreadJniEnv, trayAdapter);

	jmethodID mid = (*gtkMainThreadJniEnv)->GetMethodID(gtkMainThreadJniEnv, classRef, "fireActionActivated", "()V");

	if (mid == 0) {
		return;
	}

	(*gtkMainThreadJniEnv)->CallVoidMethod(gtkMainThreadJniEnv, trayAdapter, mid, NULL);
}

static void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}

static jobject getLinuxTrayIconAdapter(JNIEnv* env) {
	
	jclass classRef = (*gtkMainThreadJniEnv)->FindClass(gtkMainThreadJniEnv, "tray/LinuxNativeTrayAdapter");
	jmethodID mid = (*env)->GetStaticMethodID(env, classRef, "getLinuxTrayIconAdapter", "()Ltray/LinuxTrayIconAdapter;");
	if (mid == 0) {
		return NULL;
	}
	return (*env)->CallStaticObjectMethod(env, classRef, mid, NULL);
}

void print_and_flush(const char *fmt, ...) 
{
	va_list ap;
    va_start(ap, fmt);

    vprintf(fmt, ap);

    printf("\n");
    fflush(stdin);
    va_end(ap);
}
