#Add some defines
#DEFINES+= _CRT_SECURE_NO_WARNINGS	

#Specific library
#windows:LIBS += advapi32 psapi user32 winmm

#removing source 
#HEADERS -= ../borlandUi/CommonIhmRegister.h \
#		   ../borlandUi/IhmRegister.h \

#SOURCES -= ../borlandUi/IhmRegister.cpp \
#		   ../ProductRegistration.cpp

#Block if you have precompil header		   
#PRECOMPILED_HEADER = ../pch/StdAfx.h
#HEADERS -= ../pch/StdAfx.h
#SOURCES -= ../pch/StdAfx.h.cpp

#Ignore normal warning
#QMAKE_CXXFLAGS += -wd4251 -wd4189

#This warning are special because Qt put them specificaly so to remove : 
#QMAKE_CXXFLAGS_WARN_ON -= -w34100  -w34189

#iIgnore pdb from static lib
#QMAKE_LFLAGS_EXE += /ignore:4099

#Kill the application in case of APP before compiling (& is important because the copy of header is done at the prelink)
#QMAKE_PRE_LINK = "taskkill /f /im $(TargetFileName) 2>NUL | set errorlevel=0 &"	

#Console in release
#CONFIG( release, debug|release ): CONFIG *= console

#ignore warning for the lib cmt
#QMAKE_LFLAGS += /NODEFAULTLIB:libcmt

#ignore warning for the lib MSVCRT
#QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:MSVCRT
