TEMPLATE = lib
TARGET = SwModel2
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += SwModel2Constantes.h \
    _SwExportStreamTreeModel.h \
    _SwModelExportTreeView.h \
    _SwModelExportedEntitiesModel.h \
    _SwModelExportedEntity.h \
    _SwModelExportedOwnerConfigurable.h \
    _SwModelExportedExecutable.h \
    _SwModelExportedInterfaceC.h \
    _SwModelExportedInterfaceP.h \
    _SwModelExportedPin.h \
    _SwModelExportedProperty.h \
    _SwModelExportedActivable.h \
    _SwModelHostExportSelector.h \
    _SwModelHostSelector.h \
    _SwModelHost_Class.h \
    _SwModelPluginFactory_Class.h \
    _SwModelStreamTreeModel.h \
    _SwModel_Class.h \
    _SwModelsList.h  \
    ISwModelHostModifier.h  \
    SwModelsListAccess.h
        
SOURCES += _SwExportStreamTreeModel.cpp \
    _SwModelExportTreeView.cpp \
    _SwModelExportedEntitiesModel.cpp \
    _SwModelExportedEntity.cpp \
    _SwModelExportedOwnerConfigurable.cpp \
    _SwModelExportedExecutable.cpp \    
    _SwModelExportedInterfaceC.cpp \
    _SwModelExportedInterfaceP.cpp \
    _SwModelExportedPin.cpp \
    _SwModelExportedProperty.cpp \
    _SwModelExportedActivable.cpp \
    _SwModelHostExportSelector.cpp \
    _SwModelHostSelector.cpp \
    _SwModelHost_Class.cpp \
    _SwModelPluginFactory_Class.cpp \
    _SwModelStreamTreeModel.cpp \
    _SwModel_Class.cpp \
    _SwModelsList.cpp \
    SwModelsListAccess.cpp
    
FORMS +=
RESOURCES += _resources/SwModel2Rsc.qrc
DEFINES += SWMODEL2_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore \
    ../SwExecution
    
    
    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwExecutiond
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
  LIBS += -L"../../lib/vc/release" \
    -lSwCore \
    -lSwExecution
	TARGET_EXT = .swdl

}
