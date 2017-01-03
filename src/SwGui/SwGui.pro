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

	
HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)
FORMS += $$files(*.ui)
HEADERS += $$files(ComponentTools/*.h)
SOURCES += $$files(ComponentTools/*.cpp)
FORMS += $$files(ComponentTools/*.ui)
	
RESOURCES += _resources/SwGuiRsc.qrc

DEFINES += SWGUI_LIB

INCLUDEPATH += ./\
    ./ComponentTools \
    ../SwCore/ \
    ../SwExecution/ \
	../SwFoundation/ \
	../SwService/ServiceShortcut/ \
	../SwService/ServiceTools/ \
	../SwService/ServiceMainWindow/ \
	$$PROJECTS_PATH/Libraries/Qst/src/QsGui/ \
	$$PROJECTS_PATH/Libraries/Qst/src/QsCore/ \
	$$PROJECTS_PATH/Libraries/Components/src/CssLoader
    
DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"
LIBS += -L"../../$$LIB_PATH/service"
LIBS += -L"$$PROJECTS_PATH/Libraries/Qst/$$LIB_PATH"

CONFIG(debug, debug|release) {
	
	LIBS += -lSwCored -lSwFoundationd -lQsGuid -lSwServiceMainWindowd
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {	
	LIBS += -lSwCore -lSwFoundation -lQsGui -lSwServiceMainWindow
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)