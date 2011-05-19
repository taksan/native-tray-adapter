#include <stdlib.h>
#include "commonFunctions.h"

void * gtkMainThread(void * nothingUseful);
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

void * gtkMainThread(void * nothingUseful)
{
	(*javaVM)->AttachCurrentThread(javaVM, (void**) &gtkMainThreadJniEnv, NULL );
	gtk_main();
	(*javaVM)->DetachCurrentThread(javaVM);
}

