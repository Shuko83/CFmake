# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------
TEMPLATE = lib
TARGET = SwRecord

QT += core xml

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
	../SwExecution \
	../SwGui \
	../SwModel \
	../SwConfiguration \
	../SwFoundation


CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug
	
    LIBS += -L"../../lib/vc/debug" -lSwCored -lSwExecutiond -lSwGuid -lSwConfigurationd -lSwFoundationd
    TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
    LIBS += -L"../../lib/vc/release" -lSwCore -lSwExecution -lSwGui  -lSwConfiguration -lSwFoundation
    TARGET_EXT = .swdl
}

include("C:/Projects/Utilities/QtCommonPri/base.pri")