TEMPLATE = lib
TARGET = SwDataBase
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml
	
HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp)
RESOURCES += _resources/SwDatasBaseRsc.qrc

DEFINES += SWDATABASE_LIB

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