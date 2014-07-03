TEMPLATE = lib
TARGET = SwDataBase
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

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

DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../..$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"

INCLUDEPATH += ./ \
    ../SwCore \
	../SwExecution
	
CONFIG(debug, debug|release) {
	LIBS += -lSwCored -lSwExecutiond
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore -lSwExecution
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)