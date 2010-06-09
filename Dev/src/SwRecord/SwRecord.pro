# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------
TEMPLATE = lib
TARGET = SwRecord
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += _SwRecordPluginFactory_Class.h
HEADERS += SwRecordConstantes.h
HEADERS += _RecordManager.h 
HEADERS += _RecordPoint.h 
HEADERS += _ReplayManager.h 
HEADERS += ISwRecordManager.h 
HEADERS += ISwReplayManager.h 
HEADERS += ISwRecordPoint.h 
HEADERS += ISwServiceRecording.h 
HEADERS += ISwRecordDataCodec.h 
HEADERS += ISwRecordDataCodecFactory.h 
HEADERS += _SwServiceRecording.h 
HEADERS += SwRecordDataCodecAdapter.h
HEADERS += _SwRecordDataCodecDefault.h
HEADERS += _SwRecordDataCodecDefaultFactory.h
HEADERS += SwRecordDataCodecFactoryTemplate.h
HEADERS += _TimeLine.h
HEADERS += _ExecutionKey.h
HEADERS += _RecordWidget.h
HEADERS += _ReplayWidget.h

SOURCES += _SwRecordPluginFactory_Class.cpp
SOURCES += _RecordManager.cpp
SOURCES += _RecordPoint.cpp
SOURCES += _ReplayManager.cpp
SOURCES += _SwServiceRecording.cpp
SOURCES += SwRecordDataCodecAdapter.cpp
SOURCES += _SwRecordDataCodecDefault.cpp
SOURCES += _SwRecordDataCodecDefaultFactory.cpp
SOURCES += _TimeLine.cpp
SOURCES += _ExecutionKey.cpp
SOURCES += _RecordWidget.cpp
SOURCES += _ReplayWidget.cpp


OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
#Resource file(s)
RESOURCES += ./_resources/SwRecordRsc.qrc
DEFINES += SwRecord_LIB
INCLUDEPATH += ./
INCLUDEPATH += ../SwCore
INCLUDEPATH += ../SwExecution
INCLUDEPATH += ../SwGui
INCLUDEPATH += ../SwConfiguration
win32:DESTDIR = ./../../lib/vc/release/
win32:DLLDESTDIR=  ./../../bin/vc/release/


CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug/
  win32:DLLDESTDIR=  ./../../bin/vc/debug/
    TARGET = $$join(TARGET,,,d)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    LIBS += -L"../../lib/vc/debug" -lSwCored -lSwExecutiond -lSwGuid
    TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
    LIBS += -L"../../lib/vc/release" -lSwCore -lSwExecution -lSwGui
    TARGET_EXT = .swdl
}

