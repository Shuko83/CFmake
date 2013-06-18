TEMPLATE = lib
TARGET = SwExecution
PROJECTS_PATH = ../../../..

QT += core xml
	
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
    ISwClockProvider.h \
    ExecutableComponent.h 
    
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
    _SwSwitchExecutor.cpp \
    ExecutableComponent.cpp 

    
    
FORMS +=

RESOURCES += _resources/SwExecutionRsc.qrc

DEFINES += SWEXECUTION_LIB
INCLUDEPATH += ./ \
    ../SwCore/

    
CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug

	LIBS += -L"../../lib/vc/debug" -lSwCored
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../lib/vc/release" -lSwCore
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)