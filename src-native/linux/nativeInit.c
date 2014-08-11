#include <stdlib.h>
#include <pthread.h>
#include "commonFunctions.h"

void * gtkMainThread(void * nothingUseful);
JNIEXPORT void JNICALL Java_tray_linux_NativeLinuxTray_nativeInit0
  (JNIEnv *env, jobject invokingObject)
{
	(void)invokingObject;//avoid warning
	(*env)->GetJavaVM(env, &javaVM);

	gtk_init_with_empty_args();

	pthread_t gtkMainThreadId;
	pthread_create(&gtkMainThreadId, NULL, gtkMainThread , NULL);
}

void * gtkMainThread(void * nothingUseful)
{
	(void)nothingUseful;
	(*javaVM)->AttachCurrentThread(javaVM, (void**) &gtkMainThreadJniEnv, NULL );
	gtk_main();
	(*javaVM)->DetachCurrentThread(javaVM);
	return NULL;
}

void gtk_init_with_empty_args()
{
	int argc = 0;
	char ** argv = malloc(sizeof(char*));
	argv[0] = "";
	XInitThreads();
    if (gtk_init_check (&argc, &argv) == FALSE) {
		fprintf(stderr, "Failed to start GTK\n");
	}
}
