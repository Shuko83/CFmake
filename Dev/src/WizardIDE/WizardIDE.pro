TEMPLATE = lib
TARGET = WizardIDE
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += NodeComponents.h \
    NodeConnectors.h \
    NodeDescription.h \
    NodeInterfaces.h \
    NodeProperties.h \
    NodePropertiesModelImpl.h\
    NodeSwFactoryEntry.h \
    SwSimpleComponent_Class.h \
    SwWizard.h \
    SwWizardFrontEnd.h \
    SwWizardNodeModel.h \
    SwWizardNodeTreeView.h \
    WizardIDEPluginFactory_Class.h \
    SwWizardConstantes.h
SOURCES += NodeComponents.cpp \
    NodeConnectors.cpp \
    NodeDescription.cpp \
    NodeInterfaces.cpp \
    NodeProperties.cpp \
    NodePropertiesModelImpl.cpp\
    NodeSwFactoryEntry.cpp \
    SwSimpleComponent_Class.cpp \
    SwWizard.cpp \
    SwWizardFrontEnd.cpp \
    SwWizardNodeModel.cpp \
    SwWizardNodeTreeView.cpp \
    WizardIDEPluginFactory_Class.cpp
FORMS +=
RESOURCES += _resources/WizardIDERsc.qrc
DEFINES += WIZARDIDE_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./\
    ./_intermediaire \
	../SwCore \
	../SwGui \
	../SwDatasBase \
	../SwExecution
	

	
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwGuid \
    -lSwExecutiond
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../lib/vc/release" \
    -lSwCore \
    -lSwGui \
    -lSwExecution
	TARGET_EXT = .swdl
}
