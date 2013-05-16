TEMPLATE = lib
TARGET = SwDataBase
QT += core xml
	
HEADERS += SwDataBasicBuffer_Class.h \
    SwDatasBaseConstantes.h \
    _SwDatasBaseBasicRouter.h \
    _SwDatasBasePluginFactory_Class.h \
	_SwSimpleDataExecutable.h
SOURCES += SwDataBasicBuffer_Class.cpp \
    _SwDatasBaseBasicRouter.cpp \
    _SwDatasBasePluginFactory_Class.cpp \
	_SwSimpleDataExecutable.cpp
	
FORMS +=

RESOURCES += _resources/SwDatasBaseRsc.qrc

DEFINES += SWDATASBASE_LIB

INCLUDEPATH += ./ \
    ../SwCore \
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

include("C:/Projects/Utilities/QtCommonPri/base.pri")