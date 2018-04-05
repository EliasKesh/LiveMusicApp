
CC=gcc


CFLAGS = -I./ 
CFLAGS += -w
CFLAGS += -Wno-deprecated-declarations
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-implicit-function-declaration 
CFLAGS += -Wno-pointer-to-int-cast
CFLAGS += -Wno-format
CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
CFLAGS += $(shell pkg-config --cflags gladeui-2.0)
CFLAGS += $(shell pkg-config --cflags jack)
# CFLAGS += -g -gdb

LDFLAGS =-lusb-1.0 -lrt -lX11 -lXmu -lxmltok
LDFLAGS += $(shell pkg-config --cflags --libs gtk+-3.0)
LDFLAGS += $(shell pkg-config --cflags --libs alsa)
LDFLAGS += $(shell pkg-config --cflags --libs gladeui-2.0)
LDFLAGS += $(shell pkg-config --cflags --libs webkitgtk-3.0)
LDFLAGS += $(shell pkg-config --cflags --libs jack)
LDFLAGS += $(shell pkg-config --cflags --libs liblo)
   

SOURCES = AlsaUtils.c  Connections.c HTML.c Player.c PrefsGui.c 
SOURCES += Timers.c Chorder.c GTKMidiUI.c MyWidgets.c PrefsFile.c RightMenuClick.c wmctrl.c
SOURCES += transport.c SooperOSC.c


# SUBDIRS := GTK DecTrack
# include $(SUBDIRS)

OBJECTS=$(SOURCES:.cxx=.o)
# OBJECTS+=$(SOURCES:.c=.o)

EXECUTABLE=LiveMusicApp

all: $(SOURCES) $(EXECUTABLE) $(OBJECTS) $(OBJECTSPP)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm *.o $(EXECUTABLE)