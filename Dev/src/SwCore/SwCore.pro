TEMPLATE = lib
TARGET = SwCore
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += LibIndeSig.h \
    ISwRef.h \
    ISwAdminSetup.h \
    ISwComplexeTypeAdapters.h \
    ISwComplexeTypeAdaptersFactoriesBank.h \
    ISwComplexeTypeAdaptersFactory.h \
    ISwComponentProvider.h \
    ISwController.h \
    ISwCreationPostProcessor.h \  
    ISwActivable.h \
    ISwExecutor.h \
    ISwFinalizer.h \
    ISwFinalizerManager.h \
    ISwHost.h \
    ISwInterfaces_Consumer.h \
    ISwInterfaces_ConsumerObserver.h \
    ISwInterfaces_Provider.h \
    ISwInterfaces_Service.h \
    ISwInterfaces_ServicesListener.h \
    ISwLogRecorder.h \
    ISwLogger.h \
    ISwObservable.h \
    ISwObserver.h \
    ISwPersistent.h \
    ISwPersistentConfigurable.h \
    ISwPin_Listener.h \
    ISwPins_Manager.h \
    ISwPins_ManagerListener.h \
    ISwPluginFactory.h \
    ISwPluginsBank.h \
    ISwProperties.h \
    ISwProperty.h \
    ISwService.h \
    ISwServiceOwner.h \
    ISwServiceOwnerConfigurable.h \
    ISwCheckService.h \
    ISwSimpleTypeAdapter.h \
    ISwVisitor.h \
    ISwObjectFactory.h \
    SwAddress_ToolBox.h \
    SwApplication.h \
    SwBuffer_Toolbox.h \
    SwComponent_Class.h \
    SwController_Class.h \
    SwCoreConstantes.h \
    SwData_Class.h \
    SwDefaultComplexeTypeAdapters.h \
    SwEnum.h \
    SwEnumModel.h \
    SwFileDescriptor.h \
    SwIpV4Address.h \
    SwException.h \
    SwFileLogRecorder_Class.h \
    SwInterfaces_Consumer_Class.h \
    SwInterfaces_Provider_Class.h \
    SwLoader_Class.h \
    SwLogger_Class.h \
    SwMacros.h \
    SwNamed_Class.h \
    SwPin.h \
    SwPins_Manager_Class.h \
    SwPluginFactory_Class.h \
    SwProperties_Class.h \
    SwPropertiesModelImpl.h \
    SwRef.h \
    SwRefParent.h \
    SwRefPtr.h \
    SwSaver_Class.h \
    SwServicesManager_Class.h \
    SwTemplateSimpleTypeAdapter.h \
    SwTime_ToolBox.h \
    _SwComplexeTypeAdaptersFactoriesBankImpl.h \
    _SwConsumedInterfaceContainer_Class.h \
    _SwEnumKeys_Class.h \
    _SwPluginsBank_Class.h \
    _SwPropertyImplForQOject_Class.h \
    _SwPropertyImplSimpleTypeAdapter_Class.h \
    _SwPropertyImpl_Class.h \
    _SwPropertyPersistent_Toolbox.h \
    _SwProvidedInterfaceContainer_Class.h \
    _SwTreeItem.h \
    SwUUID.h \
    ISwFileEditor.h \
    ISwServiceFileEditorManager.h \
    _SwFileEditorManager.h \
    ISwServicesManager_Listener.h \
    SwAlerter_Class.h \
    ISwAlerter.h \
    ISwAlertRecorder.h \
    SwPropertiesPersistentToolbox.h \
    _SwCleanLinksVisitor.h \
    Component.h

SOURCES += SwAddress_ToolBox.cpp \
    SwApplication.cpp \
    SwBuffer_Toolbox.cpp \
    SwComponent_Class.cpp \
    SwController_Class.cpp \
    SwData_Class.cpp \
    SwDefaultComplexeTypeAdapters.cpp \
    SwEnum.cpp \
    SwEnumModel.cpp \
    SwFileDescriptor.cpp \
    SwIpV4Address.cpp \
    SwException.cpp \
    SwFileLogRecorder_Class.cpp \
    SwInterfaces_Consumer_Class.cpp \
    SwInterfaces_Provider_Class.cpp \
    SwLoader_Class.cpp \
    SwLogger_Class.cpp \
    SwNamed_Class.cpp \
    SwPin.cpp \
    SwPins_Manager_Class.cpp \
    SwPluginFactory_Class.cpp \
    SwProperties_Class.cpp \
    SwPropertiesModelImpl.cpp \
    SwRef.cpp \
    SwRefParent.cpp \
    SwSaver_Class.cpp \
    SwServicesManager_Class.cpp \
    SwTime_ToolBox.cpp \
    _SwComplexeTypeAdaptersFactoriesBankImpl.cpp \
    _SwConsumedInterfaceContainer_Class.cpp \
    _SwEnumKeys_Class.cpp \
    _SwPluginsBank_Class.cpp \
    _SwPropertyImplForQOject_Class.cpp \
    _SwPropertyImplSimpleTypeAdapter_Class.cpp \
    _SwPropertyImpl_Class.cpp \
    _SwPropertyPersistent_Toolbox.cpp \
    _SwProvidedInterfaceContainer_Class.cpp \
    _SwTreeItem.cpp \
    SwUUID.cpp \
    _SwFileEditorManager.cpp \
    SwAlerter_Class.cpp \
    SwPropertiesPersistentToolbox.cpp \
    _SwCleanLinksVisitor.cpp \
    Component.cpp

    
    
    
FORMS +=
RESOURCES += _resources/SwCoreRsc.qrc
DEFINES += SWCORE_LIB
win32:DEFINES += _WIN32_PLATEFORM_ 
win32:DEFINES +=_CRT_SECURE_NO_WARNINGS
unix:DEFINES += _LINUX_PLATEFORM_
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire
win32:{
    LIBS += -lPsapi -limagehlp
}    
  
  
    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)	

} 
CONFIG(release, debug|release) {
win32:DESTDIR = ./../../lib/vc/release
win32:DLLDESTDIR=  ./../../bin/vc/release
}
