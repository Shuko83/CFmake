TEMPLATE = lib
TARGET = SwEditor
PROJECTS_PATH = $$PWD/../../../..

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

PLUGINS=SW

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

HEADERS += ISwEditorGraphicItem.h \
    ISwEditorGraphicScene.h \
    ISwEditorInterfacesGraphicItemManager.h \
    ISwEditorPinGraphicItemManager.h \
    ISwEditorStreamNavigation.h \
    ISwEditorStreamOperations.h \
    ISwEditorStreamsManager.h \
    _ISwEditorGraphicItemListener.h \
    _ISwEditorInterfaceGraphicItemListener.h \
    _ISwEditorPinGraphicItemListener.h \
    _SwEditorApplicativeCore.h \
    _SwEditorCompStreamTabBar.h \
    _SwEditorCompToolbox.h \
    _SwEditorGIInterfaceMenu.h \
    _SwEditorGIMenu.h \
    _SwEditorGIPinMenu.h \
    _SwEditorGraphicItem.h \
    _SwEditorGraphicScene.h \
    _SwEditorInterfaceGraphicItem.h \
    _SwEditorInterfacesGraphicItemManager.h \
    _SwEditorInterfacesLayerManager.h \
    _SwEditorNavigator_Class.h \
    _SwEditorPinGraphicItem.h \
    _SwEditorPinGraphicItemManager.h \
    _SwEditorPinsLayerManager.h \
    _SwEditorPluginFactory_Class.h \
    _SwEditorQGraphicsView.h \
    _SwEditorSelection_Class.h \
    _SwEditorStreamNavigatorActions.h \
    _SwEditorStreamOperationsActions.h \
    _SwEditorStreamTreeModel.h \
    _SwEditorStreamView.h \
    _SwEditorStreamsManagerActions.h \
    _SwRouting_ToolBox.h \
    _SwStreamNavigationActions.h \
    _SwStreamOperationsActions.h \
    _SwStreamsActions.h \
    _SwStreamsTabBar.h \
	../StreamWorkEditor2/PluginsListModel.h \
	../StreamWorkEditor2/ComponentListModel.h
    
SOURCES += _SwEditorApplicativeCore.cpp \
    _SwEditorCompStreamTabBar.cpp \
    _SwEditorCompToolbox.cpp \
    _SwEditorGIInterfaceMenu.cpp \
    _SwEditorGIMenu.cpp \
    _SwEditorGIPinMenu.cpp \
    _SwEditorGraphicItem.cpp \
    _SwEditorGraphicScene.cpp \
    _SwEditorInterfaceGraphicItem.cpp \
    _SwEditorInterfacesGraphicItemManager.cpp \
    _SwEditorInterfacesLayerManager.cpp \
    _SwEditorNavigator_Class.cpp \
    _SwEditorPinGraphicItem.cpp \
    _SwEditorPinGraphicItemManager.cpp \
    _SwEditorPinsLayerManager.cpp \
    _SwEditorPluginFactory_Class.cpp \
    _SwEditorQGraphicsView.cpp \
    _SwEditorSelection_Class.cpp \
    _SwEditorStreamNavigatorActions.cpp \
    _SwEditorStreamOperationsActions.cpp \
    _SwEditorStreamTreeModel.cpp \
    _SwEditorStreamView.cpp \
    _SwEditorStreamsManagerActions.cpp \
    _SwRouting_ToolBox.cpp \
    _SwStreamNavigationActions.cpp \
    _SwStreamOperationsActions.cpp \
    _SwStreamsActions.cpp \
    _SwStreamsTabBar.cpp \
	../StreamWorkEditor2/PluginsListModel.cpp \
	../StreamWorkEditor2/ComponentListModel.cpp
    
    
FORMS +=

RESOURCES += _resources/SwEditorRsc.qrc

DEFINES += SWEDITOR_LIB

INCLUDEPATH += ./ \
    ../SwCore \
    ../SwExecution \
    ../SwGui \
    ../StreamWorkEditor2
    
DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../..$$BIN_PATH

LIBS += -L"../../$$LIB_PATH" 
    
CONFIG(debug, debug|release) {
	LIBS += -lSwCored \
    -lSwExecutiond \
    -lSwGuid

	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {	
	LIBS += -lSwCore \
    -lSwExecution \
    -lSwGui

	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)