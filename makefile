# Makefile for Live Music App
CC=gcc

CFLAGS = -I./ 
CFLAGS += -w
CFLAGS += -Wno-deprecated-declarations
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-implicit-function-declaration 
CFLAGS += -Wno-pointer-to-int-cast
CFLAGS += -Wno-format
CFLAGS += -I./ChartFixer/
CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
CFLAGS += $(shell pkg-config --cflags gladeui-2.0)
CFLAGS += $(shell pkg-config --cflags jack)
# CFLAGS += -g -ggdb

LDFLAGS = -lusb-1.0 -lrt -lm
LDFLAGS += -lX11 -lXmu -lxmltok
LDFLAGS += $(shell pkg-config --cflags --libs gtk+-3.0)
LDFLAGS += $(shell pkg-config --cflags --libs alsa)
LDFLAGS += $(shell pkg-config --cflags --libs gladeui-2.0)
LDFLAGS += $(shell pkg-config --cflags --libs webkit2gtk-4.0)
LDFLAGS += $(shell pkg-config --cflags --libs jack)
LDFLAGS += $(shell pkg-config --cflags --libs liblo)
   
SOURCES = ./src/AlsaUtils.c  ./src/Connections.c ./src/HTML.c 
SOURCES += ./src/Player.c ./src/PrefsGui.c ./src/Timers.c 
SOURCES += ./src/Chorder.c ./src/LiveMusicApp.c ./src/MyWidgets.c 
SOURCES += ./src/PrefsFile.c ./src/RightMenuClick.c ./src/wmctrl.c
SOURCES += ./src/transport.c ./src/SooperOSC.c ./src/HIDGrab.c
SOURCES += ./ChartFixer/ChartFixer.c

INCLUDES = ./src/aconfig.h ./src/DefPrefs.h ./src/HTML.h 
INCLUDES += ./src/PrefsFile.h ./src/Timers.h
INCLUDES += ./src/AlsaUtils.h ./src/GenPrefs.h 
INCLUDES += ./src/MyWidgets.h ./src/PrefsGui.h
INCLUDES += ./src/Connections.h ./src/LiveMusicApp.h 
INCLUDES += ./src/Player.h ./src/SooperOSC.h
INCLUDES += ./src/transport.c
INCLUDES += ./ChartFixer/ChartFixer.h

OBJECTS=$(SOURCES:.cxx=.o)
# OBJECTS+=$(SOURCES:.c=.o)

EXECUTABLE=LiveMusicApp

all: $(SOURCES) $(EXECUTABLE) $(OBJECTS) $(OBJECTSPP)
$(EXECUTABLE): $(OBJECTS) $(INCLUDES)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm *.o $(EXECUTABLE)
