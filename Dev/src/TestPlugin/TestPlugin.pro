# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------
TEMPLATE = lib
TARGET = TestPlugin
QT += core \
    xml
HEADERS += TestPluginPluginFactory_Class.h
HEADERS += TestPluginConstantes.h
HEADERS += TestComponent.h 
HEADERS += TestComponentEmitter.h 
HEADERS += DataTest.h 
HEADERS += TestComponentReceiver.h
HEADERS += DataTestCodec.h 
HEADERS += DataTestCodecFactory.h 
HEADERS += DataTestCodecITest.h 

SOURCES += TestPluginPluginFactory_Class.cpp
SOURCES += TestComponent.cpp
SOURCES += DataTest.cpp 
SOURCES += TestComponentEmitter.cpp 
SOURCES += TestComponentReceiver.cpp
SOURCES += DataTestCodec.cpp 
SOURCES += DataTestCodecFactory.cpp 
SOURCES += DataTestCodecITest.cpp 


OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
#Resource file(s)
RESOURCES += ./_resources/TestPluginRsc.qrc
DEFINES += TestPlugin_LIB
INCLUDEPATH += ./
INCLUDEPATH += ../SwCore
INCLUDEPATH += ../SwExecution
INCLUDEPATH += ../SwGui
INCLUDEPATH += ../SwRecord
INCLUDEPATH += ../SwConfiguration



CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwExecutiond \
    -lSwGuid \
    -lSwRecordd
    TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
    LIBS += -L"../../lib/vc/release" \
    -lSwCore \
    -lSwExecution \
    -lSwGui \
    -lSwRecord
    TARGET_EXT = .swdl
}

