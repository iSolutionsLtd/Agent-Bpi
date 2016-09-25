##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=McglMng
ConfigurationName      :=Debug
WorkspacePath          := "/home/osboxes/.codelite/MCGL/MCGL"
ProjectPath            := "/home/osboxes/MCGLWS/McglMng"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=osboxes.org
Date                   :=09/13/16
CodeLitePath           :="/home/osboxes/.codelite"
LinkerName             :=/home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-g++ 
SharedObjectLinkerName :=/home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-g++ -shared -fPIC
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
OutputFile             :=/home/osboxes/buildroot-2016.05/mcgl_bpi_rootfs/usr/mcgl/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="McglMng.txt"
PCHCompileFlags        :=-I ../McglShared -I /usr/include/tinyxml2
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            := $(IncludeSwitch)/home/osboxes/buildroot-2016.05/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include  $(IncludeSwitch). $(IncludeSwitch).../McglShared $(IncludeSwitch)/home/osboxes/buildroot-2016.05/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/include $(IncludeSwitch)/usr/arm-linux-gnueabihf/include $(IncludeSwitch)/usr/include/tinyxml2 
IncludePCH             :=  -include mcglPC.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)wiringPi $(LibrarySwitch)pthread $(LibrarySwitch)rt 
ArLibs                 :=  "wiringPi" "pthread" "rt" 
LibPath                :=$(LibraryPathSwitch)/home/osboxes/buildroot-2016.05/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/lib $(LibraryPathSwitch)/home/osboxes/buildroot-2016.05/output/host/usr/arm-buildroot-linux-gnueabihf/sysroot/usr/lib  $(LibraryPathSwitch). $(LibraryPathSwitch)/home/osboxes/buildroot-2016.05/output/target/lib $(LibraryPathSwitch)/home/osboxes/buildroot-2016.05/output/target/usr/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-ar rcu
CXX      := /home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-g++
CC       := /home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-gcc 
CXXFLAGS :=  -g -O0 -Wall -Wno-reorder $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -Wno-reorder $(Preprocessors)
ASFLAGS  := 
AS       := /home/osboxes/buildroot-2016.05/output/host/usr/bin/arm-buildroot-linux-gnueabihf-as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/McglShared_SharedMemory.cpp$(ObjectSuffix) $(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(ObjectSuffix) $(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(ObjectSuffix) 



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

# PreCompiled Header
mcglPC.h.gch: mcglPC.h
	$(CXX) $(SourceSwitch) mcglPC.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/McglMng/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(ObjectSuffix): ../McglShared/SharedMemory.cpp $(IntermediateDirectory)/McglShared_SharedMemory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/McglShared/SharedMemory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(DependSuffix): ../McglShared/SharedMemory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(DependSuffix) -MM "../McglShared/SharedMemory.cpp"

$(IntermediateDirectory)/McglShared_SharedMemory.cpp$(PreprocessSuffix): ../McglShared/SharedMemory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/McglShared_SharedMemory.cpp$(PreprocessSuffix) "../McglShared/SharedMemory.cpp"

$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(ObjectSuffix): ../McglShared/mcgl_hw.cpp $(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/McglShared/mcgl_hw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(DependSuffix): ../McglShared/mcgl_hw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(DependSuffix) -MM "../McglShared/mcgl_hw.cpp"

$(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(PreprocessSuffix): ../McglShared/mcgl_hw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/McglShared_mcgl_hw.cpp$(PreprocessSuffix) "../McglShared/mcgl_hw.cpp"

$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(ObjectSuffix): ../McglShared/mcgl_timer.cpp $(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/McglShared/mcgl_timer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(DependSuffix): ../McglShared/mcgl_timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(DependSuffix) -MM "../McglShared/mcgl_timer.cpp"

$(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(PreprocessSuffix): ../McglShared/mcgl_timer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/McglShared_mcgl_timer.cpp$(PreprocessSuffix) "../McglShared/mcgl_timer.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) "../../.codelite/MCGL/MCGL/.build-debug/McglMng"
	$(RM) mcglPC.h.gch


