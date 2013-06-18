TEMPLATE = app
TARGET = StreamWorkEditor
QT += core gui xml
PROJECTS_PATH = ../../../..

HEADERS += SwSplash.h
SOURCES += SwSplash.cpp \
		   Main.cpp

FORMS +=

RESOURCES += _resources/StreamWorkEditor.qrc

INCLUDEPATH += ../SwCore
    
CONFIG(debug, debug|release) {
    LIBS += -L"../../lib/vc/debug" -lSwCored 
	DESTDIR = ./../../bin/vc/debug/
} 


CONFIG(release, debug|release) {
	LIBS += -L"../../lib/vc/release" -lSwCore 
	DESTDIR = ./../../bin/vc/release/   
}

#Windows resource file
RC_FILE = StreamWorkEditor.rc
include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)