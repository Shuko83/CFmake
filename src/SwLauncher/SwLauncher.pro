TEMPLATE = app
TARGET = SwLauncher

QT += core xml

HEADERS +=

SOURCES += main.cpp

FORMS +=

RESOURCES +=

INCLUDEPATH += ./ \
    ../SwCore
    
CONFIG(debug, debug|release) {
	DESTDIR = ./../../bin/vc/debug/
	DLLDESTDIR = ./../../bin/vc/debug/
	LIBS += -L"../../lib/vc/debug" -lSwCored 
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../bin/vc/release/
	DLLDESTDIR = ./../../bin/vc/release/	
	LIBS += -L"../../lib/vc/release" -lSwCore 
}

include("C:/Projects/Utilities/QtCommonPri/base.pri")