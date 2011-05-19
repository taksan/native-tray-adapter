#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdarg.h>
#include "tray_linux_NativeLinuxTray.h"

typedef struct {
	GtkStatusIcon *trayIcon;
	GtkWidget *trayIconMenu;
} TrayIconInstance;

TrayIconInstance nativeInstance[10];

int nativeInstanceCount = 0;
int nextInstanceId = 0;

typedef struct  {
	int nativeId;
	int itemIndex;
} MenuActivationData;

typedef struct {
	int nativeId;
	char * iconFileName;
	char * tooltipText;
	pthread_mutex_t lockToWaitInit;
} TrayIconData;

JNIEnv * gtkMainThreadJniEnv = NULL;
JavaVM *javaVM;

GtkWidget * createTrayIconMenu(GtkStatusIcon *trayIcon);
void setTooltip(JNIEnv *env, GtkStatusIcon *trayIcon, jstring tooltip); 
void * gtkMainThread(void * nothingUseful);

gboolean createTrayIconWhenGtkMainIsIdle(TrayIconData * trayIconData);
static char * convertJStringToCString(JNIEnv * env, jstring aString); 
static jobject getLinuxTrayIconAdapter(JNIEnv* env, int nativeId);
void print_and_flush(const char *fmt, ...); 

static void trayIconPopupHandler(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
static void trayIconActivatedHandler(GObject *trayIcon, gpointer data);
static void menuItemActivationHandler(GtkMenuItem *item, gpointer menuItemIndex);

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeInit0
  (JNIEnv *env, jobject invokingObject)
{
	(*env)->GetJavaVM(env, &javaVM);

	int argc = 0;
	char ** argv = malloc(sizeof(char*));
	argv[0] = "";
    gtk_init (&argc, &argv);

	pthread_t gtkMainThreadId;
	pthread_create(&gtkMainThreadId, NULL, gtkMainThread , NULL);
}

JNIEXPORT int JNICALL Java_tray_linux_NativeLinuxTray_nativeCreateTrayIcon0
  (JNIEnv *env, jobject invokingObject, jstring file, jstring tooltip)
{
	int newInstanceId = getNextInstanceId();
	TrayIconData * trayIconData = (TrayIconData*)malloc(sizeof(TrayIconData));
	trayIconData->nativeId = newInstanceId;
	trayIconData->iconFileName = convertJStringToCString(env, file);
	trayIconData->tooltipText  = convertJStringToCString(env, tooltip);

	pthread_mutex_init(&trayIconData->lockToWaitInit, NULL);
	pthread_mutex_lock(&trayIconData->lockToWaitInit);

	g_idle_add((GSourceFunc)createTrayIconWhenGtkMainIsIdle, trayIconData);

	pthread_mutex_lock(&trayIconData->lockToWaitInit);
	pthread_mutex_unlock(&trayIconData->lockToWaitInit);

	free(trayIconData);
	return newInstanceId;
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeDisplayTrayIcon
  (JNIEnv *env, jobject invokingObject, jint nativeId)
{
	gtk_widget_show_all (nativeInstance[nativeId].trayIconMenu);	
}

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

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeRemoveMe0
  (JNIEnv *env, jobject invoker, jint nativeId)
{
	gtk_status_icon_set_visible(nativeInstance[nativeId].trayIcon, 0);
	nativeInstanceCount--;
	if (nativeInstanceCount == 0)
		gtk_main_quit();
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeAddMenuItem0
  (JNIEnv *env, jobject invokingObject, jint nativeId, jint itemIndex, jstring caption)
{
	jboolean isCopy;
	GtkWidget *menuItem;
	const char *menuItemName= (*env)->GetStringUTFChars(env, caption, &isCopy);

	menuItem = gtk_menu_item_new_with_label (menuItemName);
	MenuActivationData * activationData = (MenuActivationData*)malloc(sizeof(MenuActivationData)); 
	activationData->nativeId = nativeId;
	activationData->itemIndex = (int)itemIndex;

	g_signal_connect (G_OBJECT (menuItem), "activate", G_CALLBACK (menuItemActivationHandler), (gpointer)activationData);
	gtk_menu_shell_append (GTK_MENU_SHELL (nativeInstance[nativeId].trayIconMenu), menuItem);

	if (isCopy == JNI_TRUE) {
		(*env)->ReleaseStringUTFChars(env, caption, menuItemName);
	}   
}

JNIEXPORT jobject JNICALL Java_tray_linux_NativeLinuxTray_nativeGetIconLocation0
  (JNIEnv *env, jobject invokingObject, jint nativeId)
{
	GdkScreen                 *screen = NULL;
	GdkRectangle    rect;

	gtk_status_icon_get_geometry (nativeInstance[nativeId].trayIcon,
			&screen,
			&rect,
			NULL);

	jclass cls = (*env)->FindClass(env, "java/awt/Point");
	jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(II)V");

	jvalue args[2];
	args[0].i = rect.x;
	args[1].i = rect.y;
	return (*env)->NewObjectA(env, cls, constructor, args);
}

JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeSetAutosize0
  (JNIEnv * evn, jobject invokingObject, jint nativeId, jboolean autoSize)
{
}

gboolean createTrayIconWhenGtkMainIsIdle(TrayIconData * trayIconData)
{
	GtkStatusIcon *trayIcon = gtk_status_icon_new_from_file (trayIconData->iconFileName);
	TrayIconInstance * trayIconInstance = &nativeInstance[trayIconData->nativeId];
	trayIconInstance->trayIcon = trayIcon;

	gtk_status_icon_set_tooltip (trayIcon, trayIconData->tooltipText);

	trayIconInstance->trayIconMenu = createTrayIconMenu(trayIcon);

	int * nativeIdParam = (int*)malloc(sizeof(int));
	*nativeIdParam = trayIconData->nativeId;

	g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivatedHandler), nativeIdParam);

	gtk_status_icon_set_visible(trayIcon, TRUE);

	pthread_mutex_unlock(&trayIconData->lockToWaitInit);
	return 0;
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

static void menuItemActivationHandler(GtkMenuItem *item, gpointer activationDataPointer)
{
	MenuActivationData * activationData = (MenuActivationData*)activationDataPointer;
	int menuItemIndex = activationData->itemIndex;

	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv, activationData->nativeId);
	jclass classRef = (*gtkMainThreadJniEnv)->GetObjectClass(gtkMainThreadJniEnv, trayAdapter);

	jmethodID mid = (*gtkMainThreadJniEnv)->GetMethodID(gtkMainThreadJniEnv, classRef, "fireMenuAction", "(I)V");
	if (mid == 0) {
		return;
	}
	(*gtkMainThreadJniEnv)->CallVoidMethod(gtkMainThreadJniEnv, trayAdapter, mid, menuItemIndex);
}

static void trayIconActivatedHandler(GObject *trayIcon, gpointer nativeIdPointer)
{
	jobject trayAdapter = getLinuxTrayIconAdapter(gtkMainThreadJniEnv, *((int*)nativeIdPointer));
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

static jobject getLinuxTrayIconAdapter(JNIEnv* env, int nativeId) {
	
	jclass classRef = (*gtkMainThreadJniEnv)->FindClass(gtkMainThreadJniEnv, "tray/LinuxNativeTrayAdapter");
	jmethodID mid = (*env)->GetStaticMethodID(env, classRef, "getLinuxTrayIconAdapter", "(I)Ltray/LinuxTrayIconAdapter;");
	if (mid == 0) {
		return NULL;
	}
	return (*env)->CallStaticObjectMethod(env, classRef, mid, nativeId);
}

void * gtkMainThread(void * nothingUseful)
{
	(*javaVM)->AttachCurrentThread(javaVM, (void**) &gtkMainThreadJniEnv, NULL );
	gtk_main();
	(*javaVM)->DetachCurrentThread(javaVM);
}

static char * convertJStringToCString(JNIEnv * env, jstring aString) 
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

void print_and_flush(const char *fmt, ...) 
{
	va_list ap;
    va_start(ap, fmt);

    vprintf(fmt, ap);

    printf("\n");
    fflush(stdin);
    va_end(ap);
}
