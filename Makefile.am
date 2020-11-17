bin_PROGRAMS = LiveMusicApp
ACLOCAL_AMFLAGS = -I m4

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
src/APlayMidi.c \
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
src/APlayMidi.h \
src/SooperOSC.h 

LiveMusicApp_CPPFLAGS = -Wno-incompatible-pointer-types -Wformat=0 -Wno-unused-result -Wno-deprecated -Wdeprecated-declarations
LiveMusicApp_CPPFLAGS += $(GTK3_CFLAGS)
LiveMusicApp_CPPFLAGS += $(ALSA_CFLAGS)
LiveMusicApp_CPPFLAGS += $(GLADEUI_CFLAGS)
LiveMusicApp_CPPFLAGS += $(WEBKIT2GTK_CFLAGS)
LiveMusicApp_CPPFLAGS += $(JACK_CFLAGS)
LiveMusicApp_CPPFLAGS += $(LIBLO_CFLAGS)
LiveMusicApp_CPPFLAGS += -DMY_BUILD_NUMBER=$$(cat ./Scripts/BuildNumber)
#LiveMusicApp_CPPFLAGS += -DBUILD_NUMBER1=$(shell ./IncBuildNumber.sh)
LiveMusicApp_CFLAGS = $(LiveMusicApp_CPPFLAGS)


LiveMusicApp_LDFLAGS = -L/usr/lib/x86_64-linux-gnu/
LiveMusicApp_LDFLAGS += -lxmltok -lxml2
LiveMusicApp_LDFLAGS += $(GTK3_LIBS)
LiveMusicApp_LDFLAGS += $(ALSA_LIBS)
LiveMusicApp_LDFLAGS += $(GLADEUI_v)
LiveMusicApp_LDFLAGS += $(WEBKIT2GTK_LIBS)
LiveMusicApp_LDFLAGS += $(JACK_LIBS)
LiveMusicApp_LDFLAGS += $(LIBLO_LIBS)
LiveMusicApp_LDFLAGS += -Xlinker --defsym -Xlinker __Bump_NUMBER=$$(./Scripts/IncBuildNumber.sh)
# LiveMusicApp_LDFLAGS += -Xlinker --defsym -Xlinker __BUILD_DATE=$$(date +'%Y%m%d')



# Copy the preferences.
#Prefsdir = "$(HOME)/.config/LiveMusicApp/"
# Prefs_DATA = LiveMusicRes/* 
# Prefs_DATA = LiveMusicRes/* -c -m 644

Prefsdir = "$(prefix)/share/LiveMusicApp/"
Prefs_DATA = LiveMusicRes/*
Songsdir = "$(prefix)/share/LiveMusicApp/MySongs/"
Songs_DATA = MySongs/background.png \
		MySongs/logo.jpg \
		MySongs/index.html \
		MySongs/LiveMusicAppDocumentation.pdf
		
Tune1dir = "$(prefix)/share/LiveMusicApp/MySongs/BlueRondo"
Tune1_DATA = MySongs/BlueRondo/*

#Songsdir = "$(Prefsdir)/MySongs/"
# Songs_DATA = DefaultSongs/*

# Songsdir = "$(HOME)/MySongs/"
# Songs_DATA = DefaultSongs/*
#Songs_DATA = DefaultSongs/* -c -m 644

# install-data-local:

install-exec-hook:
#	rsync -avrx "./MySongs" $(Prefsdir)
#	rsync -avrx "./LiveMusicRes/" $(Prefsdir)

# 	$(MKDIR_P) $(Prefsdir)
# 	$(MKDIR_P) $(Songsdir)
# 	chmod a+rw $(Songsdir)
# 	chown $$USER:$$USER $(Songsdir)
# 	chmod a+rw $(Prefsdir)
# 	chown $$USER:$$USER $(Prefsdir)
# 	rsync -avrx "./DefaultSongs/" "$(HOME)/MySongs/"
# 	rsync -avrx "./LiveMusicRes/" "$(HOME)/.config/LiveMusicApp/"

clean-local:
	rm -f *.0 src/*.o
