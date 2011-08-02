#include <pthread.h>
#include "tray_linux_NativeLinuxTray.h"

void * mainThread(void * nothingUseful)
{
	gtk_main();
}

int main()
{
	gtk_init_with_empty_args();
	pthread_t mainThreadId;
	pthread_create(&mainThreadId, NULL, mainThread , NULL);

	createTrayIconForIconAndWithTooltip("/home/takeuchi/projects/trayicon-tests/skype2gmail.svg", "some tip");
	createTrayIconForIconAndWithTooltip("/home/takeuchi/projects/trayicon-tests/skype2gmail.svg", "some tip");

	pthread_join(mainThreadId, NULL);
}
