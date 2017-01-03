# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------
TEMPLATE = lib
TARGET = SwRecord
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
} 

HEADERS += $$files(*.h)

SOURCES += $$files(*.cpp)

RESOURCES += ./_resources/SwRecordRsc.qrc

DEFINES += SWRECORD_LIB

INCLUDEPATH += ./ \
	../SwCore \
	../SwService/ServiceShortcut \
	../SwExecution \
	../SwGui \
	../SwModel2 \
	../SwConfiguration \
	../SwFoundation

DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"

CONFIG(debug, debug|release) {	
    LIBS += -lSwCored -lSwExecutiond -lSwGuid -lSwConfigurationd -lSwFoundationd
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {	
    LIBS += -lSwCore -lSwExecution -lSwGui  -lSwConfiguration -lSwFoundation
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)