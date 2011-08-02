#*******************************************************************************
# * Copyright (c) 2011 Gabriel Takeuchi <g.takeuchi@gmail.com>
# * 
# * Skype2Gmail is licensed under either the Apache License, Version 2.0 or
# * the Eclipse Public License v1.0.
# * You may use it freely in commercial and non-commercial products.
# * You may obtain a copy of the licenses at
# *
# *   the Apache License - http://www.apache.org/licenses/LICENSE-2.0
# *   the Eclipse Public License - http://www.eclipse.org/legal/epl-v10.html
# *
# * If it is possible to cooperate with the publicity of Skype2Gmail, please add
# * links to the Skype2Java web site <https://skype2gmail.org> 
# * in your web site or documents.
# * 
# * Contributors:
# * Gabriel Takeuchi 
# ******************************************************************************/
JDK_DIR = $(JAVA_HOME)

CC = gcc
CFLAGS = -pipe -g -fPIC `pkg-config --cflags-only-I --libs gtk+-2.0 dbus-1` -Wall -W -O2
#-Wall -W 
INCPATH = -I. -I$(JDK_DIR)/include -I$(JDK_DIR)/include/linux 
SOURCES = commonFunctions.c nativeInit.c nativeCreateTrayIcon.c nativeSetImage.c nativeAddMenuItem.c \
	nativeRemoveMe.c nativeGetIconLocation.c
OBJECTS = $(SOURCES:.c=.o)
OBJECTS_X64 = $(SOURCES:.c=.x64)
LIBS=`pkg-config --cflags --libs gtk+-2.0` 
TRAY_PATH=../../src/main/resources/

.SECONDEXPANSION:

LINK = g++
LFLAGS = -shared 

TARGET=../../src/main/resources/

all: liblinuxtray_x86.so liblinuxtray_x64.so

liblinuxtray_x86.so: $(OBJECTS)
	$(LINK) $(LFLAGS) -m32 -o $(TARGET)/liblinuxtray_x86.so $(OBJECTS) $(LIBS)

liblinuxtray_x64.so: $(OBJECTS_X64)
	$(LINK) $(LFLAGS) -m64 -o $(TARGET)/liblinuxtray_x64.so $(OBJECTS_X64) $(LIBS)

tray_linux_NativeLinuxTray.h: ../../src/main/java/tray/linux/NativeLinuxTray.java
	javah -classpath ../../target/classes tray.linux.NativeLinuxTray

#NativeLinuxTray: tray_linux_NativeLinuxTray.h NativeLinuxTray.c
#	$(CC) -c $(CFLAGS) $(INCPATH) -m32 -o $@.o $@.c
#	$(CC) -c $(CFLAGS) $(INCPATH) -m64 -o $@_x64.o $@.c

.SUFFIXES: .x64 .o .c

.c.x64:	
	$(CC) -c $(CFLAGS) $(INCPATH) -m64 -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -m32 -o $@ $<

nativeTrayTest: nativeTrayMainTest.c
	gcc -o nativeTrayTest nativeTrayMainTest.c -g  -lpthread -L$(TRAY_PATH)  -llinuxtray_x64	

clean:
	rm -f  *.o *.so *.x64
