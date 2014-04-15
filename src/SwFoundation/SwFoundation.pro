TEMPLATE = lib
TARGET = SwFoundation
PROJECTS_PATH = ../../../..

QT += core xml
	
HEADERS +=  *.h 
    
SOURCES +=  *.cpp 
    
FORMS +=

DEFINES += SWFOUNDATION_LIB

INCLUDEPATH += ./ \
    ../SwCore/ \
	../SwGui/ \
	../SwExecution

    
CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug
	
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwExecutiond
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwExecution
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)