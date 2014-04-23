TEMPLATE = lib
TARGET = SwDoc
PROJECTS_PATH = ../../../..

QT += core sql

HEADERS +=  *.h 

SOURCES +=  *.cpp 

FORMS += _resources/*.ui

RESOURCES += SwDoc.qrc

DEFINES += SWDOC_LIB

INCLUDEPATH += ./ \
	../../../QDjango/src/
	
    
CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug

	LIBS += -L"../../../QDjango/lib/vc/debug" -lQDjangod
	LIBS += -L"../../lib/vc/debug" -lSwCored
	TARGET_EXT = .dlld
	
	QMAKE_POST_LINK = "xcopy /Y .\\_resources\\libmySQL.dll ..\\..\\bin\\vc\\debug\\" &
	QMAKE_POST_LINK += "xcopy /Y .\\_resources\\sqldrivers\\qsqlmysqld4.dll ..\\..\\bin\\vc\\debug\\sqldrivers\\" 
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../../QDjango/lib/vc/release" -lQDjango
	LIBS += -L"../../lib/vc/release" -lSwCore
	TARGET_EXT = .dll

	QMAKE_POST_LINK = "xcopy /Y .\\_resources\\libmySQL.dll ..\\..\\bin\\vc\\debug\\" &
	QMAKE_POST_LINK += "xcopy /Y .\\_resources\\sqldrivers\\qsqlmysql4.dll ..\\..\\bin\\vc\\debug\\sqldrivers\\"
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)

include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)