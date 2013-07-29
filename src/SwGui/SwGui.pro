TEMPLATE = lib
TARGET = SwGui
PROJECTS_PATH = ../../../..

QT += core gui xml network
CONFIG      += designer plugin
	
HEADERS += *.h 	
SOURCES += *.cpp 	 

FORMS += _SwGuiCssDialog.ui \
	_QRcViewerUi.ui
	
RESOURCES += _resources/SwGuiRsc.qrc

DEFINES += SWGUI_LIB

INCLUDEPATH += ./\
    ../SwCore/ \
    ../SwExecution/ \
	../SwFoundation/ 
    
CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug
	
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwFoundationd
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwFoundation
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)