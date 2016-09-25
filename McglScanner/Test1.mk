##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Test1
ConfigurationName      :=Debug
WorkspacePath          := "/home/osboxes/.codelite/MCGL/MCGL"
ProjectPath            := "/home/osboxes/MCGLWS/Test1"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=osboxes.org
Date                   :=06/15/16
CodeLitePath           :="/home/osboxes/.codelite"
LinkerName             :=/usr/bin/arm-linux-gnueabihf-g++ 
SharedObjectLinkerName :=/usr/bin/arm-linux-gnueabihf-g++ -shared -fPIC
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
OutputFile             :=/home/osboxes/PCduino_shared/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Test1.txt"
PCHCompileFlags        :=-I/usr/include/tinyxml2
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)/usr/arm-linux-gnueabihf/include $(IncludeSwitch)/usr/include/tinyxml2 
IncludePCH             :=  -include mcglPC.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)uuid 
ArLibs                 :=  "uuid" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/usr/arm-linux-gnueabihf/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/arm-linux-gnueabihf-ar rcu
CXX      := /usr/bin/arm-linux-gnueabihf-g++ 
CC       := /usr/bin/arm-linux-gnueabihf-gcc 
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/arm-linux-gnueabihf-as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/mcgl_fat32.cpp$(ObjectSuffix) $(IntermediateDirectory)/mcgl_xml.cpp$(ObjectSuffix) $(IntermediateDirectory)/mcgl_prop.cpp$(ObjectSuffix) 



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
	$(CXX) $(SourceSwitch) mcglPC.h $(PCHCompileFlags)



##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/Test1/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/mcgl_fat32.cpp$(ObjectSuffix): mcgl_fat32.cpp $(IntermediateDirectory)/mcgl_fat32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/Test1/mcgl_fat32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mcgl_fat32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mcgl_fat32.cpp$(DependSuffix): mcgl_fat32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mcgl_fat32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mcgl_fat32.cpp$(DependSuffix) -MM "mcgl_fat32.cpp"

$(IntermediateDirectory)/mcgl_fat32.cpp$(PreprocessSuffix): mcgl_fat32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mcgl_fat32.cpp$(PreprocessSuffix) "mcgl_fat32.cpp"

$(IntermediateDirectory)/mcgl_xml.cpp$(ObjectSuffix): mcgl_xml.cpp $(IntermediateDirectory)/mcgl_xml.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/Test1/mcgl_xml.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mcgl_xml.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mcgl_xml.cpp$(DependSuffix): mcgl_xml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mcgl_xml.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mcgl_xml.cpp$(DependSuffix) -MM "mcgl_xml.cpp"

$(IntermediateDirectory)/mcgl_xml.cpp$(PreprocessSuffix): mcgl_xml.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mcgl_xml.cpp$(PreprocessSuffix) "mcgl_xml.cpp"

$(IntermediateDirectory)/mcgl_prop.cpp$(ObjectSuffix): mcgl_prop.cpp $(IntermediateDirectory)/mcgl_prop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/osboxes/MCGLWS/Test1/mcgl_prop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mcgl_prop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mcgl_prop.cpp$(DependSuffix): mcgl_prop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mcgl_prop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mcgl_prop.cpp$(DependSuffix) -MM "mcgl_prop.cpp"

$(IntermediateDirectory)/mcgl_prop.cpp$(PreprocessSuffix): mcgl_prop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mcgl_prop.cpp$(PreprocessSuffix) "mcgl_prop.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) "../../.codelite/MCGL/MCGL/.build-debug/Test1"
	$(RM) mcglPC.h.gch


