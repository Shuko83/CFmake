TEMPLATE = lib
TARGET = SwConfiguration
QMAKE_CXXFLAGS += /MP
QT += core \
    xml \
    gui
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
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore \
    ../SwExecution \
    ../SwGui \
    ../SwModel \
    ../SwRecord
      
win32:DESTDIR = ./../../lib/vc/release/
win32:DLLDESTDIR=  ./../../bin/vc/release/
              
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug/
  win32:DLLDESTDIR=  ./../../bin/vc/debug/
    TARGET = $$join(TARGET,,,d)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwGuid -lSwExecutiond
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwGui -lSwExecution
	TARGET_EXT = .swdl
}
