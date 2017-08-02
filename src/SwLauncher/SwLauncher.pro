TEMPLATE = app
TARGET = SwLauncher
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
} 


HEADERS +=

SOURCES += main.cpp

FORMS +=

RESOURCES +=

INCLUDEPATH += ./ \
    ../SwCore
    
DESTDIR = ./../../$$BIN_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"

CONFIG(debug, debug|release) {
	LIBS += -lSwCored 
} 

CONFIG(release, debug|release) {	
	LIBS += -lSwCore 
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)