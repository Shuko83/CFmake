TEMPLATE = lib
TARGET = SwDoc
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core sql widgets

HEADERS +=  *.h 

SOURCES +=  *.cpp 

FORMS += _resources/*.ui

RESOURCES += SwDoc.qrc

DEFINES += SWDOC_LIB

INCLUDEPATH += ./ \
	../../../QDjango/src/
	
    
DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIB_PATH ~= s,/,\\,g
BIN_PATH ~= s,/,\\,g

CONFIG(debug, debug|release) {
	LIBS += -L"../../../QDjango\\$$LIB_PATH" -lQDjangod
	LIBS += -L"../../$$LIB_PATH" -lSwCored
} 

CONFIG(release, debug|release) {
	LIBS += -L"../../../QDjango\\$$LIB_PATH" -lQDjango
	LIBS += -L"../../$$LIB_PATH" -lSwCore
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)

include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)