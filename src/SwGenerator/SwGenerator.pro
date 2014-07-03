TEMPLATE = app
TARGET = SwGenerator
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
} 

HEADERS +=  *.h 

SOURCES +=  *.cpp 

FORMS += *.ui

RESOURCES += SwGenerator.qrc

DEFINES += SWGENERATOR_EXE

include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)

INCLUDEPATH += ../../../QCTemplate/src/

DESTDIR = ./../../$$BIN_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../../QCTemplate/$$LIB_PATH"
	
CONFIG(debug, debug|release) {
	
	LIBS += -lQCTemplated
} 

CONFIG(release, debug|release) {
	
	LIBS += -lQCTemplate
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)