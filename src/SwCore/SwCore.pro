TEMPLATE = lib
TARGET = SwCore

PROJECTS_PATH = ../../../..

QT += core xml

CONFIG += StdAfx.h
PRECOMPILED_HEADER = StdAfx.h
win32-msvc* {
PRECOMPILED_SOURCE = StdAfx.cpp
}


HEADERS += *.h 

SOURCES += *.cpp 
    
FORMS +=

RESOURCES += _resources/SwCoreRsc.qrc

DEFINES += SWCORE_LIB

win32:DEFINES += _WIN32_PLATEFORM_ 
win32:DEFINES +=_CRT_SECURE_NO_WARNINGS

unix:DEFINES += _LINUX_PLATEFORM_

win32:{
    LIBS += -lPsapi -limagehlp
}    

CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)