TEMPLATE = lib
TARGET = SwExecution
PROJECTS_PATH = $$PWD/../../../..

QT += core xml widgets gui

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)
	
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

DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"
	
CONFIG(debug, debug|release) {
	LIBS += -lSwCored
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore
	TARGET_EXT = .swdl
}


include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)