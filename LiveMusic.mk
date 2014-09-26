##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=LiveMusic
ConfigurationName      :=Debug
WorkspacePath          := "/mnt/Personal/home/elias/workspace/EliasMusic"
ProjectPath            := "/mnt/Personal/home/elias/workspace/GTKMidiUI"
IntermediateDirectory  :=../GTKMidiUI/Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Elias Kesh
Date                   :=08/10/14
CodeLitePath           :="/mnt/Personal/home/elias/.codelite"
LinkerName             :=/usr/bin/gcc
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./LiveMusicApp
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
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -ggdb  -Wno-cast-qual $(Preprocessors)
CFLAGS   :=  -pg -g `pkg-config --cflags gtk+-2.0` -ggdb -Wno-cast-qual $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/PrefsFile.c$(ObjectSuffix) $(IntermediateDirectory)/GTKMidiUI.c$(ObjectSuffix) $(IntermediateDirectory)/AlsaUtils.c$(ObjectSuffix) $(IntermediateDirectory)/HTML.c$(ObjectSuffix) $(IntermediateDirectory)/Connections.c$(ObjectSuffix) $(IntermediateDirectory)/ThirdParty_wmctrl.c$(ObjectSuffix) $(IntermediateDirectory)/PrefsGui.c$(ObjectSuffix) $(IntermediateDirectory)/Chorder.c$(ObjectSuffix) 



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
	@test -d ../GTKMidiUI/Debug || $(MakeDirCommand) ../GTKMidiUI/Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/PrefsFile.c$(ObjectSuffix): PrefsFile.c $(IntermediateDirectory)/PrefsFile.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/PrefsFile.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PrefsFile.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PrefsFile.c$(DependSuffix): PrefsFile.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PrefsFile.c$(ObjectSuffix) -MF$(IntermediateDirectory)/PrefsFile.c$(DependSuffix) -MM "PrefsFile.c"

$(IntermediateDirectory)/PrefsFile.c$(PreprocessSuffix): PrefsFile.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PrefsFile.c$(PreprocessSuffix) "PrefsFile.c"

$(IntermediateDirectory)/GTKMidiUI.c$(ObjectSuffix): GTKMidiUI.c $(IntermediateDirectory)/GTKMidiUI.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/GTKMidiUI.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GTKMidiUI.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GTKMidiUI.c$(DependSuffix): GTKMidiUI.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GTKMidiUI.c$(ObjectSuffix) -MF$(IntermediateDirectory)/GTKMidiUI.c$(DependSuffix) -MM "GTKMidiUI.c"

$(IntermediateDirectory)/GTKMidiUI.c$(PreprocessSuffix): GTKMidiUI.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GTKMidiUI.c$(PreprocessSuffix) "GTKMidiUI.c"

$(IntermediateDirectory)/AlsaUtils.c$(ObjectSuffix): AlsaUtils.c $(IntermediateDirectory)/AlsaUtils.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/AlsaUtils.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AlsaUtils.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AlsaUtils.c$(DependSuffix): AlsaUtils.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AlsaUtils.c$(ObjectSuffix) -MF$(IntermediateDirectory)/AlsaUtils.c$(DependSuffix) -MM "AlsaUtils.c"

$(IntermediateDirectory)/AlsaUtils.c$(PreprocessSuffix): AlsaUtils.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AlsaUtils.c$(PreprocessSuffix) "AlsaUtils.c"

$(IntermediateDirectory)/HTML.c$(ObjectSuffix): HTML.c $(IntermediateDirectory)/HTML.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/HTML.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/HTML.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/HTML.c$(DependSuffix): HTML.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/HTML.c$(ObjectSuffix) -MF$(IntermediateDirectory)/HTML.c$(DependSuffix) -MM "HTML.c"

$(IntermediateDirectory)/HTML.c$(PreprocessSuffix): HTML.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/HTML.c$(PreprocessSuffix) "HTML.c"

$(IntermediateDirectory)/Connections.c$(ObjectSuffix): Connections.c $(IntermediateDirectory)/Connections.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/Connections.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Connections.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Connections.c$(DependSuffix): Connections.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Connections.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Connections.c$(DependSuffix) -MM "Connections.c"

$(IntermediateDirectory)/Connections.c$(PreprocessSuffix): Connections.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Connections.c$(PreprocessSuffix) "Connections.c"

$(IntermediateDirectory)/ThirdParty_wmctrl.c$(ObjectSuffix): ThirdParty/wmctrl.c $(IntermediateDirectory)/ThirdParty_wmctrl.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/ThirdParty/wmctrl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThirdParty_wmctrl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThirdParty_wmctrl.c$(DependSuffix): ThirdParty/wmctrl.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThirdParty_wmctrl.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ThirdParty_wmctrl.c$(DependSuffix) -MM "ThirdParty/wmctrl.c"

$(IntermediateDirectory)/ThirdParty_wmctrl.c$(PreprocessSuffix): ThirdParty/wmctrl.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThirdParty_wmctrl.c$(PreprocessSuffix) "ThirdParty/wmctrl.c"

$(IntermediateDirectory)/PrefsGui.c$(ObjectSuffix): PrefsGui.c $(IntermediateDirectory)/PrefsGui.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/PrefsGui.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PrefsGui.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PrefsGui.c$(DependSuffix): PrefsGui.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PrefsGui.c$(ObjectSuffix) -MF$(IntermediateDirectory)/PrefsGui.c$(DependSuffix) -MM "PrefsGui.c"

$(IntermediateDirectory)/PrefsGui.c$(PreprocessSuffix): PrefsGui.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PrefsGui.c$(PreprocessSuffix) "PrefsGui.c"

$(IntermediateDirectory)/Chorder.c$(ObjectSuffix): Chorder.c $(IntermediateDirectory)/Chorder.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/mnt/Personal/home/elias/workspace/GTKMidiUI/Chorder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Chorder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Chorder.c$(DependSuffix): Chorder.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Chorder.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Chorder.c$(DependSuffix) -MM "Chorder.c"

$(IntermediateDirectory)/Chorder.c$(PreprocessSuffix): Chorder.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Chorder.c$(PreprocessSuffix) "Chorder.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/PrefsFile.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/PrefsFile.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/PrefsFile.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/GTKMidiUI.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AlsaUtils.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/HTML.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/HTML.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/HTML.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Connections.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Connections.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Connections.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ThirdParty_wmctrl.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/PrefsGui.c$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Chorder.c$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Chorder.c$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Chorder.c$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../EliasMusic/.build-debug/LiveMusic"


