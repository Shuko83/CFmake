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

HEADERS += _SwRecordPluginFactory_Class.h \
	SwRecordConstantes.h \
	_RecordManager.h \
	_RecordPoint.h \
	_ReplayManager.h \
	ISwRecordManager.h \
	ISwRecordManagerListener.h \
	ISwReplayControler.h \
	ISwReplayListener.h \
	ISwRecordPoint.h \
	ISwServiceRecording.h \
	ISwRecordDataCodec.h \
	ISwRecordDataCodecFactory.h \
	_SwServiceRecording.h \
	SwRecordDataCodecAdapter.h \
	_SwRecordDataCodecDefault.h \
	_SwRecordDataCodecDefaultFactory.h \
	SwRecordDataCodecFactoryTemplate.h \
	_TimeLine.h \
	_ExecutionKey.h \
	_RecordWidget.h

SOURCES += _SwRecordPluginFactory_Class.cpp \
	_RecordManager.cpp \
	_RecordPoint.cpp \
	_ReplayManager.cpp \
	_SwServiceRecording.cpp \
	SwRecordDataCodecAdapter.cpp \
	_SwRecordDataCodecDefault.cpp \
	_SwRecordDataCodecDefaultFactory.cpp \
	_TimeLine.cpp \
	_ExecutionKey.cpp \
	_RecordWidget.cpp 

RESOURCES += ./_resources/SwRecordRsc.qrc

DEFINES += SwRecord_LIB

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