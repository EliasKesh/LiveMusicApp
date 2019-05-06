
CC=gcc


CFLAGS = -I./ 
#CFLAGS += -w
#CFLAGS += -Wno-deprecated-declarations
#CFLAGS += -Wno-int-conversion
#CFLAGS += -Wno-implicit-function-declaration 
#CFLAGS += -Wno-pointer-to-int-cast
CFLAGS += -Wno-format
CFLAGS += -DStandAlone
#LDFLAGS =-lusb-1.0 -lrt -lX11 -lXmu -lxmltok
LDFLAGS = 
   

SOURCES = ChartFixer.c


# SUBDIRS := GTK DecTrack
# include $(SUBDIRS)

OBJECTS=$(SOURCES:.cxx=.o)
# OBJECTS+=$(SOURCES:.c=.o)

EXECUTABLE=ChartFixer

all: $(SOURCES) $(EXECUTABLE) $(OBJECTS) $(OBJECTSPP)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm *.o $(EXECUTABLE)
