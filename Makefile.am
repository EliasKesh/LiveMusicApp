bin_PROGRAMS = LiveMusicApp
#SUBDIRS = src ChartFixer
# VPATH = src:ChartFixer

LiveMusicApp_SOURCES = src/AlsaUtils.c \
src/HTML.c \
src/PrefsFile.c \
src/Timers.c \
src/Chorder.c \
src/LiveMusicApp.c \
src/PrefsGui.c \
src/transport.c \
src/Connections.c \
src/MyWidgets.c \
src/RightMenuClick.c \
src/wmctrl.c \
src/HIDGrab.c \
src/Player.c \
src/SooperOSC.c \
src/aconfig.h \
src/DefPrefs.h \
src/LiveMusicApp.h \
src/PrefsFile.h \
src/Timers.h \
src/AlsaUtils.h \
src/GenPrefs.h \
src/MyWidgets.h \
src/PrefsGui.h \
src/Connections.h \
src/HTML.h \
src/Player.h \
src/SooperOSC.h \
ChartFixer/ChartFixer.c \
ChartFixer/ChartFixer.h

LiveMusicApp_CPPFLAGS = -Wno-incompatible-pointer-types -Wformat=0 -Wno-unused-result
LiveMusicApp_CPPFLAGS += $(GTK3_CFLAGS)
LiveMusicApp_CPPFLAGS += $(ALSA_CFLAGS)
LiveMusicApp_CPPFLAGS += $(GLADEUI_CFLAGS)
LiveMusicApp_CPPFLAGS += $(WEBKIT2GTK_CFLAGS)
LiveMusicApp_CPPFLAGS += $(JACK_CFLAGS)
LiveMusicApp_CPPFLAGS += $(LIBLO_CFLAGS)

LiveMusicApp_LDFLAGS = -L/usr/lib/x86_64-linux-gnu/
LiveMusicApp_LDFLAGS = -lxmltok -lxml2
LiveMusicApp_LDFLAGS += $(GTK3_LIBS)
LiveMusicApp_LDFLAGS += $(ALSA_LIBS)
LiveMusicApp_LDFLAGS += $(GLADEUI_v)
LiveMusicApp_LDFLAGS += $(WEBKIT2GTK_LIBS)
LiveMusicApp_LDFLAGS += $(JACK_LIBS)
LiveMusicApp_LDFLAGS += $(LIBLO_LIBS)
