TEMPLATE = lib
TARGET = SwConfiguration
PROJECTS_PATH = $$PWD/../../../..

QT += core xml gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

PLUGINS=SW

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

HEADERS += ISwConfiguration.h \
    ISwConfigurationListener.h \
    SwConfigurationConstantes.h \
    _SwConfigurationPluginFactory_Class.h \
    _SwConfigurationManager.h \
    _SwConfiguration.h \
    _SwConfigurationExportedEntity.h \
    _SwConfigurationExportedExecution.h \
    _SwConfigurationExportedPerspective.h \
    _SwConfigurationExportedProperty.h \
    _SwConfigurationSelector.h \
    _SwConfigurationStreamTreeModel.h \
    _SwConfigurationEntitiesModel.h \
    _SwConfigurationTreeView.h \
    _SwManagedConfiguration.h \
    _SwConfigurationManagerGui.h \
    _SwManagedConfigurationsList.h \
    ISwConfigurationEditor.h \
    ISwConfigurationSaver.h \
    _ISwManagedConfigurationsList.h \
    _SwLaunchManager.h \
    _ISwConfigurationLaunchManager.h \
    _ISwConfigurationLaunchEditor.h \
    ISwConfigurationController.h \
    ISwConfigurationNameListener.h \
    ISwConfigurationNameProvider.h \
    ISwConfigurationControllerListener.h \
    _SwControllerAction.h \
    _SwControllerActionStart.h \
    _SwControllerActionStop.h  \
    _SwControllerActionRestart.h  \
    _SwControllerActionSave.h  \
    _SwControllerActionLoad.h  \     
    _SwControllerActionExportConfig.h \
    _SwConfigurationExportedOwnerConfigurable.h
    
SOURCES += _SwConfigurationPluginFactory_Class.cpp \
    _SwConfigurationManager.cpp \
    _SwConfiguration.cpp \
    _SwConfigurationExportedEntity.cpp \
    _SwConfigurationExportedExecution.cpp \
    _SwConfigurationExportedPerspective.cpp \
    _SwConfigurationExportedProperty.cpp \
    _SwConfigurationSelector.cpp \
    _SwConfigurationStreamTreeModel.cpp \
    _SwConfigurationEntitiesModel.cpp \
    _SwConfigurationTreeView.cpp \
    _SwManagedConfiguration.cpp \
    _SwConfigurationManagerGui.cpp \
    _SwManagedConfigurationsList.cpp\
    _SwLaunchManager.cpp \
    _SwControllerAction.cpp \
    _SwControllerActionStart.cpp \
    _SwControllerActionStop.cpp \
    _SwControllerActionRestart.cpp \
    _SwControllerActionSave.cpp \    
    _SwControllerActionExportConfig.cpp \
    _SwControllerActionLoad.cpp \    
    _SwConfigurationExportedOwnerConfigurable.cpp
    
FORMS += _SwConfigurationManagerGui.ui

RESOURCES += _resources/SwConfigurationRsc.qrc

DEFINES += SWCONFIGURATION_LIB

INCLUDEPATH += ./ \
    ../SwCore \
	../SwService/ServiceConfiguration \
    ../SwExecution \
    ../SwGui \
    ../SwModel2 \

DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../..$$BIN_PATH
LIBS += -L"../../$$LIB_PATH"
	
CONFIG(debug, debug|release) {
	LIBS += -lSwCored -lSwExecutiond -lSwGuid
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {  
	LIBS += -lSwCore -lSwExecution -lSwGui 
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)