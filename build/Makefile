#
# 
#

.PHONY : macos ios Qt

TARGET := undefined
system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET = mac
else
ifeq ($(system), MINGW)
	TARGET = windows
else
ifeq ($(system), Linux)
	TARGET = linux
	LINUXMIDI ?= yes
endif
endif
endif


OUT  := GUIDOEngine.framework GUIDOEngine.tgz libGUIDOEngine.a
MAC  ?= MacOS
IOS  := ios

# try to detect Visual Studio version used
ifeq ($(TARGET), windows)
 MSVC ?= C:\Program Files (x86)\Microsoft Visual Studio 12.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= Visual Studio 12 2013
  WIN64VS ?= $(WIN32VS) Win64
  endif
 else
 MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= Visual Studio 10
  WIN64VS ?= $(WIN32VS) Win64
 endif
endif

LINUX := linux
C++11 ?= yes
CMAKEOPT ?= -DMIDIEXPORT=yes -DC++11=$(C++11)

all :
	make $(TARGET)
	make tools
	make Qt

undefined:
	$(error System is undefined, not target available)

help :
	@echo GUIDOEngine makefile - Targets are :
	@echo "   all (default): build the GUIDOEngine library,"	
	@echo "                        the guido tools,"	
	@echo "                        the Guido Qt library, applications and tools"	
	@echo "   tools        : build guido tools"	
	@echo "   Qt           : build the Guido Qt library, applications and tools"	
	@echo "   java         : build the jni and java libraries"	
	@echo "   js           : build the javascript library (requires emscripten)"	
	@echo "   android      : build the GUIDOEngine static library for Android"	
	@echo 
	@echo "Platform targets to build the GUIDOEngine library are:"	
	@echo "   macos [C++11=yes/no (default to no)]"	
	@echo "   mac     (build also ios library)"	
	@echo "   windows (build 32 and 64 bits library)"	
	@echo "   linux"	
	@echo "the platform targets is automatically evaluated by the default target."	
	@echo "NOTE: windows targets are based on $(WIN32VS)"	

#===============================================================
# building the library on Mac OS X
#===============================================================
.PHONY : macos mac11 $(IOS) $(MAC)/Release/GUIDOEngine.framework $(IOS)/Release-iphoneos/libGUIDOEngine.a

mac : 
	make macos
	make $(IOS)	

macos : $(MAC)/guidolib.xcodeproj GUIDOEngine.framework GUIDOEngine.tgz

GUIDOEngine.framework :  MacOS/Release/GUIDOEngine.framework
	rm -rf GUIDOEngine.framework
	cp -R macos/Release/GUIDOEngine.framework .

$(MAC)/Release/GUIDOEngine.framework :
	xcodebuild -jobs 8 -project $(MAC)/guidolib.xcodeproj -target ALL_BUILD -configuration Release

$(MAC)/guidolib.xcodeproj : CMakeLists.txt
	[ -d $(MAC) ] || mkdir $(MAC)
	cd $(MAC) && cmake .. -G Xcode $(CMAKEOPT)

GUIDOEngine.tgz : GUIDOEngine.framework
	tar czf GUIDOEngine.tgz GUIDOEngine.framework

mac11:
	make macos MAC=mac11 C++11=yes
	cp -R $(MAC)/Release/GUIDOEngine.framework mac11
	cd mac11 && tar czf GUIDOEngine.tgz GUIDOEngine.framework

#===============================================================
# building the library for iOS
#===============================================================
$(IOS) : $(IOS)/guidolib.xcodeproj libGUIDOEngine.a

libGUIDOEngine.a : $(IOS)/guidolib.xcodeproj $(IOS)/Release-iphoneos/libGUIDOEngine.a
	cp $(IOS)/Release-iphoneos/libGUIDOEngine.a .

$(IOS)/Release-iphoneos/libGUIDOEngine.a :
	xcodebuild -jobs 8 -project $(IOS)/guidolib.xcodeproj -target ALL_BUILD -configuration Release
 
ioscmake:
$(IOS)/guidolib.xcodeproj : CMakeLists.txt
	[ -d $(IOS) ] || mkdir $(IOS)
	cd $(IOS) && cmake .. -G Xcode -DMIDIEXPORT=no -DC++11=yes -DIOS=yes

macclean :
	rm -rf $(IOS) $(MAC) $(OUT)

iosclean :
	rm -rf $(IOS) libGUIDOEngine.a

#===============================================================
# building the library on windows
#===============================================================
.PHONY : win32 win64 win32/Release/GUIDOEngine.dll win64/Release/GUIDOEngine64.dll

windows :
	make win32
	make win64

win32 : win32/guidolib.sln GUIDOEngine.dll GUIDOEngine.lib 

GUIDOEngine.lib : GUIDOEngine.dll

GUIDOEngine.dll : win32/Release/GUIDOEngine.dll
	cp win32/Release/GUIDOEngine.dll .
	cp win32/Release/GUIDOEngine.lib .

win32/Release/GUIDOEngine.dll : win32/guidolib.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/guidolib.sln /maxcpucount:4 /p:Configuration=Release"

win32cmake:
win32/guidolib.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(WIN32VS)" $(CMAKEOPT)


win64 : win64/guidolib.sln GUIDOEngine64.dll GUIDOEngine64.lib 

GUIDOEngine64.lib : GUIDOEngine64.dll

GUIDOEngine64.dll : win64/Release/GUIDOEngine64.dll
	cp win64/Release/GUIDOEngine64.dll .
	cp win64/Release/GUIDOEngine64.lib .

win64/Release/GUIDOEngine64.dll : win64/guidolib.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win64/guidolib.sln /maxcpucount:4 /p:Configuration=Release"

win64cmake:
win64/guidolib.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(WIN64VS)" $(CMAKEOPT) -DMIDIEXPORT=yes

winclean:
	rm -rf win32 win64 GUIDOEngine*.lib GUIDOEngine.dll

#===============================================================
# building the library on linux
#===============================================================
.PHONY : $(LINUX) libGUIDOEngine.so

linux : $(LINUX)/Makefile libGUIDOEngine.so

libGUIDOEngine.so :  $(LINUX)/Makefile  $(LINUX)/libGUIDOEngine.so
	rm -rf GUIDOEngine*.so*
	make -C $(LINUX)
	cp -R $(LINUX)/libGUIDOEngine*.so* .

$(LINUX)/libGUIDOEngine.so : 
	make -C $(LINUX)
	
linuxcmake:
$(LINUX)/Makefile : CMakeLists.txt
	[ -d $(LINUX) ] || mkdir $(LINUX)
	cd $(LINUX) && cmake .. -G "Unix Makefiles" -DMIDIEXPORT=$(LINUXMIDI)

linuxclean:
	rm -rf $(LINUX)

#===============================================================
# building the javascript engine
#===============================================================
js:
	make -C ../lang/javascript -j 4


#===============================================================
# building the Guido tools
#===============================================================
clean :
	make macclean
	make winclean
	make linuxclean
	make -C ../src/tools/build clean
	make -C ../environments/Qt clean
	make -C ../lang/java/build clean

tools :
	make -C ../src/tools/build

#===============================================================
# building the Qt resources
#===============================================================
Qt :
	make -C ../environments/Qt

#===============================================================
# building the jni and java library
#===============================================================
java :
	make -C ../lang/java/build

#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C ../platforms/android/guido-engine-android -j 4
	cp ../platforms/android/guido-engine-android/libs/armeabi/libGUIDOEngine.so ../platforms/android
