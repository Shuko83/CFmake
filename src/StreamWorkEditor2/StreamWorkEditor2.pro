TEMPLATE = app
TARGET = StreamWorkEditor2

QT += core gui xml opengl

HEADERS += SwSplash.h  \
	MainWindow.h \
	StreamView.h \
	StreamScene.h \
	StreamControler.h \
	PluginsListModel.h \
	ComponentListModel.h \
	WidgetFactory.h \
	IAnchorable.h \
	IAnchorableObserver.h \
	AnchorableContainer.h \
	VerticalMultiAnchorableContainer.h \
	ComponentGraphicItem.h \
	ConnectorGraphicItem.h \
	LinkGraphicItem.h \
	GraphicsResources.h \
	MenuManager.h \
	PropertiesWidget.h \
	../SwGui/_SwGuiPluginsFrontEnd.h \
	../SwGui/SwGuiDefaultItemDelegate.h \
	../SwGui/SwGuiEnumComboBox.h \
	../SwGui/_QRcViewer.h \
	Arranger.h \
	GraphicsLoaderV1.h \
	QStreamTreeModel.h \
	ISelectionObserver.h \
	ViewNavigator.h \
	IStreamViewObserver.h \
	InterestArea.h \
	IStreamControlerObserver.h \
	QInterestAreaTreeModel.h \
	LogView.h \
	EditorPropertiesItemDelegate.h \
	EditionService.h \
	ManageLinkColor.h \
	PluginOverview.h \
	LegendView.h \
	../SwModel2/ISwModelHostModifier.h \
	ModelCreatorHelper.h


SOURCES += SwSplash.cpp \
	main.cpp \
	MainWindow.cpp \
	StreamView.cpp \
	StreamScene.cpp \
	StreamControler.cpp \
	PluginsListModel.cpp \
	ComponentListModel.cpp \
	WidgetFactory.cpp \
	AnchorableContainer.cpp \
	VerticalMultiAnchorableContainer.cpp \
	ComponentGraphicItem.cpp \
	ConnectorGraphicItem.cpp \
	LinkGraphicItem.cpp \
	GraphicsResources.cpp \
	MenuManager.cpp \
	PropertiesWidget.cpp \
	../SwGui/_SwGuiPluginsFrontEnd.cpp \
	../SwGui/SwGuiDefaultItemDelegate.cpp \
	../SwGui/SwGuiEnumComboBox.cpp \
	../SwGui/_QRcViewer.cpp \
	Arranger.cpp \
	GraphicsLoaderV1.cpp \
	QStreamTreeModel.cpp \
	ViewNavigator.cpp \
	InterestArea.cpp \
	QInterestAreaTreeModel.cpp \
	LogView.cpp \
	EditorPropertiesItemDelegate.cpp \
	EditionService.cpp \
	ManageLinkColor.cpp \
	PluginOverview.cpp \
	LegendView.cpp \
	ModelCreatorHelper.cpp 

FORMS += _resources/LogView.ui \
	../SwGui/_QRcViewerUi.ui \
	_resources/PluginOverview.ui
	
RESOURCES += _resources/StreamWorkEditor2.qrc

INCLUDEPATH += ./ \
    ../SwCore \
    ../SwGui \
    ../SwExecution \
	  ../SwDoc \
	  ../SwModel2
    
#    ../SwGui/_intermediaire \

DEFINES += SW_EDITOR
    
VERSION = 1.2.3 



CONFIG(debug, debug|release) {
	DESTDIR = ./../../bin/vc/debug/
	DLLDESTDIR = ./../../bin/vc/debug/
	
	INCLUDEPATH += ../SwDoc/GeneratedFiles/uid
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwDocd
	CONFIG += console
} 

CONFIG(release, debug|release) {
	DESTDIR = ./../../bin/vc/release/
	DLLDESTDIR = ./../../bin/vc/release/

	INCLUDEPATH += ../SwDoc/GeneratedFiles/ui	
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwDoc
}

win32:RC_FILE = StreamWorkEditor2.rc

include("C:/Projects/Utilities/QtCommonPri/base.pri")
