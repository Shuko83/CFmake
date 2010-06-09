TEMPLATE = lib
TARGET = SwExecution
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += ISwExecutable_Service.h \
    ISwExecution_Service.h \
    SwExecutionConstantes.h \
    _SwExecutionPluginFactory_Class.h \
    _SwExecutionSetup.h \
    _SwExecution_Service.h \
    _SwExecutor.h \
    _SwExecutorListEditor.h \
    _SwExecutorStreamTreeModel.h \
    _SwSimpleExecutable.h \
    _SwSwitchExecution_Service.h \
    _SwSwitchExecutionList.h \
    _SwSwitchExecutionSetup.h \
    _SwSwitchExecutor.h \
    ISwSwitchExecutorListener.h \
    ISwSwitchExecutorListProvider.h \
    ISwSupportReplay.h \
    ISwClockProvider.h
    
SOURCES += _SwExecutionPluginFactory_Class.cpp \
    _SwExecutionSetup.cpp \
    _SwExecution_Service.cpp \
    _SwExecutor.cpp \
    _SwExecutorListEditor.cpp \
    _SwExecutorStreamTreeModel.cpp \
    _SwSimpleExecutable.cpp \
    _SwSwitchExecution_Service.cpp \
    _SwSwitchExecutionList.cpp \
    _SwSwitchExecutionSetup.cpp \
    _SwSwitchExecutor.cpp
    
    
FORMS +=
RESOURCES += _resources/SwExecutionRsc.qrc
DEFINES += SWEXECUTION_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore/

win32:DESTDIR = ./../../lib/vc/release/
win32:DLLDESTDIR=  ./../../bin/vc/release/
    
    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug/
  win32:DLLDESTDIR=  ./../../bin/vc/debug/
  TARGET = $$join(TARGET,,,d)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
	LIBS += -L"../../lib/vc/release" -lSwCore
	TARGET_EXT = .swdl
}
