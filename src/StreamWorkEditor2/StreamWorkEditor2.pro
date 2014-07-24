TEMPLATE = app
TARGET = StreamWorkEditor2
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core gui xml opengl widgets printsupport

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
	_resources/PluginOverview.ui
	
RESOURCES += _resources/StreamWorkEditor2.qrc

INCLUDEPATH += ./ \
    ../SwCore \
   ../SwGui \
    ../SwExecution \
	  ../SwDoc \
	  ../SwModel2

DEFINES += SW_EDITOR
    
VERSION = 1.2.3 

DESTDIR = .\\..\\..\\$$BIN_PATH
DLLDESTDIR=  .\\..\\..\\$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"

CONFIG(debug, debug|release) {	
	LIBS += -lSwCored -lSwGuid -lSwDocd
	CONFIG += console
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore -lSwGui -lSwDoc
	QMAKE_POST_LINK = "$${QMAKE_COPY} ..\\..\\..\\..\\Libraries\\ExceptionManager\\$$BIN_PATH\\ExceptionManager.dll ..\\..\\$$BIN_PATH"
}

win32:RC_FILE = StreamWorkEditor2.rc

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)

#degeulace:!!!!!
CONFIG(debug, debug|release) {
	INCLUDEPATH += $$PWD/../SwDoc/$$GEN_FILES/ui
} 

CONFIG(release, debug|release) {
	INCLUDEPATH += $$PWD/../SwDoc/$$GEN_FILES/ui	
}
