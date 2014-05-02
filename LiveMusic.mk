##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=LiveMusic
ConfigurationName      :=Debug
WorkspacePath          := "/mnt/Personal/home/elias/workspace/LiveMusic"
ProjectPath            := "/mnt/Personal/home/elias/workspace/GTKMidiUI"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Elias Kesh
Date                   :=05/02/14
CodeLitePath           :="/mnt/Personal/home/elias/.codelite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="LiveMusic.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  `pkg-config --libs gtk+-2.0` -export-dynamic
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/usr/include/libsoup-2.4/ $(IncludeSwitch)/usr/include/libglade-2.0/ $(IncludeSwitch)/usr/include/webkitgtk-1.0/ $(IncludeSwitch)/usr/include/atk-1.0/ $(IncludeSwitch)/usr/include/gdk-pixbuf-2.0/ $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/gtk-2.0/include/ $(IncludeSwitch)/usr/include/pango-1.0/ $(IncludeSwitch)/usr/include/nspr/ $(IncludeSwitch)/usr/include/gtk-2.0/ $(IncludeSwitch)/usr/include/alsa/ $(IncludeSwitch)/usr/include/glib-2.0/ $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include/ $(IncludeSwitch)/usr/include/gtk-2.0/gdk/ $(IncludeSwitch)/usr/include/xulrunner-2.0/cairo/ $(IncludeSwitch)/usr/include/libxml2/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread $(LibrarySwitch)glade-2.0 $(LibrarySwitch)asound $(LibrarySwitch)webkitgtk-1.0 $(LibrarySwitch)X11 $(LibrarySwitch)Xmu $(LibrarySwitch)xmltok $(LibrarySwitch)xmlparse $(LibrarySwitch)xml2 
ArLibs                 :=  "pthread" "glade-2.0" "asound" "webkitgtk-1.0" "X11" "Xmu" "xmltok" "xmlparse" "xml2" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/lib/x86_64-linux-gnu/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g -Wall -O0 -ggdb $(Preprocessors)
CFLAGS   :=  -pg -g `pkg-config --cflags gtk+-2.0` -ggdb $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/PrefsFile$(ObjectSuffix) $(IntermediateDirectory)/GTKMidiUI$(ObjectSuffix) $(IntermediateDirectory)/AlsaUtils$(ObjectSuffix) $(IntermediateDirectory)/HTML$(ObjectSuffix) $(IntermediateDirectory)/Connections$(ObjectSuffix) $(IntermediateDirectory)/ThirdParty_wmctrl$(ObjectSuffix) $(IntermediateDirectory)/PrefsGui$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/PrefsFile$(ObjectSuffix): PrefsFile.c $(IntermediateDirectory)/PrefsFile$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/PrefsFile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PrefsFile$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PrefsFile$(DependSuffix): PrefsFile.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PrefsFile$(ObjectSuffix) -MF$(IntermediateDirectory)/PrefsFile$(DependSuffix) -MM "PrefsFile.c"

$(IntermediateDirectory)/PrefsFile$(PreprocessSuffix): PrefsFile.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PrefsFile$(PreprocessSuffix) "PrefsFile.c"

$(IntermediateDirectory)/GTKMidiUI$(ObjectSuffix): GTKMidiUI.c $(IntermediateDirectory)/GTKMidiUI$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/GTKMidiUI.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GTKMidiUI$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GTKMidiUI$(DependSuffix): GTKMidiUI.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GTKMidiUI$(ObjectSuffix) -MF$(IntermediateDirectory)/GTKMidiUI$(DependSuffix) -MM "GTKMidiUI.c"

$(IntermediateDirectory)/GTKMidiUI$(PreprocessSuffix): GTKMidiUI.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GTKMidiUI$(PreprocessSuffix) "GTKMidiUI.c"

$(IntermediateDirectory)/AlsaUtils$(ObjectSuffix): AlsaUtils.c $(IntermediateDirectory)/AlsaUtils$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/AlsaUtils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AlsaUtils$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AlsaUtils$(DependSuffix): AlsaUtils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AlsaUtils$(ObjectSuffix) -MF$(IntermediateDirectory)/AlsaUtils$(DependSuffix) -MM "AlsaUtils.c"

$(IntermediateDirectory)/AlsaUtils$(PreprocessSuffix): AlsaUtils.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AlsaUtils$(PreprocessSuffix) "AlsaUtils.c"

$(IntermediateDirectory)/HTML$(ObjectSuffix): HTML.c $(IntermediateDirectory)/HTML$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/HTML.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/HTML$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/HTML$(DependSuffix): HTML.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/HTML$(ObjectSuffix) -MF$(IntermediateDirectory)/HTML$(DependSuffix) -MM "HTML.c"

$(IntermediateDirectory)/HTML$(PreprocessSuffix): HTML.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/HTML$(PreprocessSuffix) "HTML.c"

$(IntermediateDirectory)/Connections$(ObjectSuffix): Connections.c $(IntermediateDirectory)/Connections$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/Connections.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Connections$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Connections$(DependSuffix): Connections.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Connections$(ObjectSuffix) -MF$(IntermediateDirectory)/Connections$(DependSuffix) -MM "Connections.c"

$(IntermediateDirectory)/Connections$(PreprocessSuffix): Connections.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Connections$(PreprocessSuffix) "Connections.c"

$(IntermediateDirectory)/ThirdParty_wmctrl$(ObjectSuffix): ThirdParty/wmctrl.c $(IntermediateDirectory)/ThirdParty_wmctrl$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/ThirdParty/wmctrl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThirdParty_wmctrl$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThirdParty_wmctrl$(DependSuffix): ThirdParty/wmctrl.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThirdParty_wmctrl$(ObjectSuffix) -MF$(IntermediateDirectory)/ThirdParty_wmctrl$(DependSuffix) -MM "ThirdParty/wmctrl.c"

$(IntermediateDirectory)/ThirdParty_wmctrl$(PreprocessSuffix): ThirdParty/wmctrl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThirdParty_wmctrl$(PreprocessSuffix) "ThirdParty/wmctrl.c"

$(IntermediateDirectory)/PrefsGui$(ObjectSuffix): PrefsGui.c $(IntermediateDirectory)/PrefsGui$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/PrefsGui.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PrefsGui$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PrefsGui$(DependSuffix): PrefsGui.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PrefsGui$(ObjectSuffix) -MF$(IntermediateDirectory)/PrefsGui$(DependSuffix) -MM "PrefsGui.c"

$(IntermediateDirectory)/PrefsGui$(PreprocessSuffix): PrefsGui.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PrefsGui$(PreprocessSuffix) "PrefsGui.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/PrefsFile$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/PrefsFile$(DependSuffix)
	$(RM) $(IntermediateDirectory)/PrefsFile$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI$(DependSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/HTML$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/HTML$(DependSuffix)
	$(RM) $(IntermediateDirectory)/HTML$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Connections$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Connections$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Connections$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui$(DependSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../LiveMusic/.build-debug/LiveMusic"


