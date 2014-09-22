TEMPLATE = lib
TARGET = SwGui
PROJECTS_PATH = $$PWD/../../../..

PLUGINS=SW

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core gui xml network designer widgets
CONFIG +=  plugin
CONFIG += StdAfx.h
PRECOMPILED_HEADER = StdAfx.h
win32-msvc* {
PRECOMPILED_SOURCE = StdAfx.cpp
}

	
HEADERS += *.h
SOURCES += *.cpp
FORMS += *.ui
	
RESOURCES += _resources/SwGuiRsc.qrc

DEFINES += SWGUI_LIB

INCLUDEPATH += ./\
    ../SwCore/ \
    ../SwExecution/ \
	../SwFoundation/ \
	../SWService/ServiceShortcut/ \
	../SwService/ServiceImpl/ \
    
DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"

CONFIG(debug, debug|release) {
	
	LIBS += -lSwCored -lSwFoundationd
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {	
	LIBS += -lSwCore -lSwFoundation
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)