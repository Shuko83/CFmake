TEMPLATE = lib
TARGET = WizardIDE
PROJECTS_PATH = ../../../..

QT += core xml

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

INCLUDEPATH += ./\
	../SwCore \
	../SwGui \
    ../SwGui/_intermediaire \
	../SwDatasBase \
	../SwExecution
	
CONFIG(debug, debug|release) {
	DESTDIR = ./../../lib/vc/debug
	DLLDESTDIR=  ./../../bin/vc/debug

	LIBS += -L"../../lib/vc/debug" \
    -lSwCored \
    -lSwGuid \
    -lSwExecutiond
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../lib/vc/release
	DLLDESTDIR=  ./../../bin/vc/release
	
	LIBS += -L"../../lib/vc/release" \
    -lSwCore \
    -lSwGui \
    -lSwExecution
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)