TEMPLATE = app
TARGET = SwGenerator
PROJECTS_PATH = ../../../..

QT += core xml

HEADERS +=  *.h 

SOURCES +=  *.cpp 

FORMS += *.ui

RESOURCES += SwGenerator.qrc

DEFINES += SWGENERATOR_EXE

INCLUDEPATH += ../../../QCTemplate/src/
	
CONFIG(debug, debug|release) {
	DLLDESTDIR=  ./../../bin/vc/debug
	DESTDIR=  ./../../bin/vc/debug
	
	LIBS += -L"../../../QCTemplate/lib/vc/debug" -llibctemplated -lQCTemplated
	QMAKE_POST_LINK = "xcopy /Y ..\\..\\..\\QCTemplate\\bin\\vc\\debug\\*.dll .\\..\\..\\bin\\vc\\debug\\"
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../bin/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../../QCTemplate/lib/vc/release" -llibctemplate -lQCTemplate
	QMAKE_POST_LINK = "xcopy /Y ..\\..\\..\\QCTemplate\\bin\\vc\\release\\*.dll .\\..\\..\\bin\\vc\\release\\"
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)