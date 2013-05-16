# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------
TEMPLATE = lib
TARGET = TestPlugin
QT += core \
    xml
HEADERS += TestPluginPluginFactory_Class.h \
	TestPluginConstantes.h \
	TestComponent.h \
	TestComponentEmitter.h \
	DataTest.h \
	TestComponentReceiver.h \
	TestComponentReceiverVassisted.h \
	DataTestCodec.h \
	DataTestCodecFactory.h \
	DataTestCodecITest.h \
	TestAssistedComponent.h \
	TestComponentEmitterVassisted.h \
	SwRefProfilerUI.h 

SOURCES += TestPluginPluginFactory_Class.cpp \
	TestComponent.cpp \
	DataTest.cpp \
	TestComponentEmitter.cpp \
	TestComponentReceiver.cpp \
	TestComponentReceiverVassisted.cpp \
	DataTestCodec.cpp \
	DataTestCodecFactory.cpp \
	DataTestCodecITest.cpp \
	TestAssistedComponent.cpp \
	TestComponentEmitterVassisted.cpp \
	SwRefProfilerUI.cpp 

RESOURCES += ./_resources/TestPluginRsc.qrc

DEFINES += TestPlugin_LIB

INCLUDEPATH += ./ \
	../SwCore \
	../SwExecution \
	../SwGui \
	../SwRecord \
	../SwConfiguration \
	../SwFoundation



CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug

    LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwExecutiond \
	-lSwFoundationd \
    -lSwGuid \
    -lSwRecordd
    TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
    LIBS += -L"../../lib/vc/release" \
    -lSwCore \
    -lSwExecution \
	-lSwFoundation \
    -lSwGui \
    -lSwRecord
    TARGET_EXT = .swdl
}

include("C:/Projects/Utilities/QtCommonPri/base.pri")