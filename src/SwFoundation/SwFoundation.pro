TEMPLATE = lib
TARGET = SwFoundation
PROJECTS_PATH = ../../../..

QT += core xml
	
HEADERS +=  SwAssistedComponent.h 
    
SOURCES +=  SwAssistedComponent.cpp 
    
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
	TARGET_EXT = .dlld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwExecution
	TARGET_EXT = .dll
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)