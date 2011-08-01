TEMPLATE = app
TARGET = StreamWorkEditor2
QMAKE_CXXFLAGS += /MP
QT += core \
    gui \
    xml \
    opengl
HEADERS += SwSplash.h
HEADERS += MainWindow.h
HEADERS += StreamView.h
HEADERS += StreamScene.h
HEADERS += StreamControler.h
HEADERS += PluginsListModel.h
HEADERS += ComponentListModel.h
HEADERS += WidgetFactory.h
HEADERS += IAnchorable.h
HEADERS += IAnchorableObserver.h
HEADERS += AnchorableContainer.h
HEADERS += VerticalMultiAnchorableContainer.h
HEADERS += ComponentGraphicItem.h
HEADERS += ConnectorGraphicItem.h
HEADERS += LinkGraphicItem.h
HEADERS += GraphicsResources.h
HEADERS += MenuManager.h
HEADERS += PropertiesWidget.h
HEADERS += ../SwGui/_SwGuiPluginsFrontEnd.h
HEADERS += ../SwGui/SwGuiDefaultItemDelegate.h
HEADERS += ../SwGui/SwGuiEnumComboBox.h
HEADERS += ../SwGui/_QRcViewer.h
HEADERS += Arranger.h
HEADERS += GraphicsLoaderV1.h
HEADERS += QStreamTreeModel.h
HEADERS += ISelectionObserver.h
HEADERS += ViewNavigator.h
HEADERS += IStreamViewObserver.h
HEADERS += InterestArea.h
HEADERS += IStreamControlerObserver.h
HEADERS += QInterestAreaTreeModel.h
HEADERS += LogView.h
HEADERS += EditorPropertiesItemDelegate.h
HEADERS += EditionService.h
HEADERS += ManageLinkColor.h

SOURCES += SwSplash.cpp
SOURCES += main.cpp
SOURCES += MainWindow.cpp
SOURCES += StreamView.cpp
SOURCES += StreamScene.cpp
SOURCES += StreamControler.cpp
SOURCES += PluginsListModel.cpp
SOURCES += ComponentListModel.cpp
SOURCES += WidgetFactory.cpp
SOURCES += AnchorableContainer.cpp
SOURCES += VerticalMultiAnchorableContainer.cpp
SOURCES += ComponentGraphicItem.cpp
SOURCES += ConnectorGraphicItem.cpp
SOURCES += LinkGraphicItem.cpp
SOURCES += GraphicsResources.cpp
SOURCES += MenuManager.cpp
SOURCES += PropertiesWidget.cpp
SOURCES += ../SwGui/_SwGuiPluginsFrontEnd.cpp
SOURCES += ../SwGui/SwGuiDefaultItemDelegate.cpp
SOURCES += ../SwGui/SwGuiEnumComboBox.cpp
SOURCES += ../SwGui/_QRcViewer.cpp
SOURCES += Arranger.cpp
SOURCES += GraphicsLoaderV1.cpp
SOURCES += QStreamTreeModel.cpp
SOURCES += ViewNavigator.cpp
SOURCES += InterestArea.cpp
SOURCES += QInterestAreaTreeModel.cpp
SOURCES += LogView.cpp
SOURCES += EditorPropertiesItemDelegate.cpp
SOURCES += EditionService.cpp
SOURCES += ManageLinkColor.cpp


FORMS += _resources/LogView.ui
FORMS += ../SwGui/_QRcViewerUi.ui
RESOURCES += _resources/StreamWorkEditor2.qrc
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore \
    ../SwGui \
    ../SwGui/_intermediaire \
    ../WizardIDE \
    ../SwExecution
    
DEFINES += SW_EDITOR
    
VERSION = 1.2.3 

win32:DESTDIR = ./../../bin/vc/release/

CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../bin/vc/debug/
    TARGET = $$join(TARGET,,,d)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored -lWizardIDEd
} 
CONFIG(release, debug|release) {
	LIBS += -L"../../lib/vc/release" -lSwCore -lWizardIDE
}

#Windows resource file
win32:RC_FILE = StreamWorkEditor2.rc
