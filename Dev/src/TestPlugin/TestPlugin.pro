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
HEADERS += TestComponentReceiverVassisted.h
HEADERS += DataTestCodec.h 
HEADERS += DataTestCodecFactory.h 
HEADERS += DataTestCodecITest.h 
HEADERS += TestAssistedComponent.h 
HEADERS += TestComponentEmitterVassisted.h 

SOURCES += TestPluginPluginFactory_Class.cpp
SOURCES += TestComponent.cpp
SOURCES += DataTest.cpp 
SOURCES += TestComponentEmitter.cpp 
SOURCES += TestComponentReceiver.cpp
SOURCES += TestComponentReceiverVassisted.cpp
SOURCES += DataTestCodec.cpp 
SOURCES += DataTestCodecFactory.cpp 
SOURCES += DataTestCodecITest.cpp 
SOURCES += TestAssistedComponent.cpp 
SOURCES += TestComponentEmitterVassisted.cpp 

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
INCLUDEPATH += ../SwFoundation



CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwExecutiond \
	-lSwFoundationd \
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
	-lSwFoundation \
    -lSwGui \
    -lSwRecord
    TARGET_EXT = .swdl
}

