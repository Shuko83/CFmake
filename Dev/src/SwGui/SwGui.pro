TEMPLATE = lib
TARGET = SwGui
QMAKE_CXXFLAGS += /MP
QT += core \
    gui \
    xml \
    network
HEADERS += ISwAction.h \
    ISwDockWidget.h \
    ISwLayout.h \
    ISwMainWindow.h \
    ISwMenu.h \
    ISwQAbstractItemViewSlots.h \
    ISwToolBar.h \
    ISwWidget.h \
    ISwWidget2.h \
    ISwWidget2_Observer.h \
    ISwHttpServer.h \
    ISwHttpPart.h \
    ISwPerspective.h \
    ISwPerspectiveObserver.h \
    SwGuiConstantes.h \
    SwGuiDefaultItemDelegate.h \
    SwGuiEnumComboBox.h \
    _SwGuiCompDockWidget.h \
    _SwGuiCompFrame.h \
	_SwGuiCompLabel.h \
	_SwGuiCompGroupBox.h \
    _SwGuiCompTabWidget.h \
    _SwGuiCompSplitterWidget.h \
    _SwGuiCompHBoxLayout.h \
    _SwGuiCompMainWindow.h \
    _SwGuiCompMenu.h \
    _SwGuiCompPluginsTree.h \
    _SwGuiCompToPropertiesModel.h \
    _SwGuiCompToolbar.h \
    _SwGuiCompTreeView.h \
    _SwGuiCompVBoxLayout.h \
    _SwGuiCompWidget.h \
    _SwGuiCompHttpServer.h \
    _SwGuiPluginFactory_Class.h \
    _SwGuiPluginsFrontEnd.h \
    _SwPropertiesModelImpl.h \
    _SwQAbstractItemViewSlots_Catcher.h \
    _SwConsoleWidget.h \
    _SwGuiVisualLogConsole.h \
    _SwGuiVisualUpdater.h \
    _SwPerspectivesManager.h \
    SwDefaultPerspective.h \
    _SwPerspectiveButton.h \
    _SwBasicPerspective.h  \
    _SwGuiCompGridLayout.h  \
    _SwGuiCompGridLayoutCell.h  \
    ISwActionList.h \
    ISwActionListListener.h \
    _SwGuiCompActionList.h \
    _SwGUICssDialog.h \
    _SwGuiCssEditor.h \
    _SwGuiCssHighlighter.h \
	_SwGuiConsole.h \
	ISwStackedWidget_Controler.h \
	_SwGuiCompStackedWidget.h \
	ISwStackedWidget_Observer.h \
    _SwGuiCompQActionToStackedWidget.h \
	_SwActionStackedWidget.h \
	_QRcViewer.h \
	ISwShortcut.h \
	ISwDeviceObserver.h \
	ISwShortcutObserver.h \
	ISwServiceShortcuts.h \
    ISwDevice.h \
	_SwServiceShortcuts.h \
	_SwGuiQActionToWidget.h \
	_SwContainerCloseableWidget.h 

	
SOURCES += SwGuiDefaultItemDelegate.cpp \
    SwGuiEnumComboBox.cpp \
    _SwGuiCompDockWidget.cpp \
    _SwGuiCompFrame.cpp \
	_SwGuiCompLabel.cpp \
	_SwGuiCompGroupBox.cpp \
    _SwGuiCompTabWidget.cpp \
    _SwGuiCompSplitterWidget.cpp \
    _SwGuiCompHBoxLayout.cpp \
    _SwGuiCompMainWindow.cpp \
    _SwGuiCompMenu.cpp \
    _SwGuiCompPluginsTree.cpp \
    _SwGuiCompToPropertiesModel.cpp \
    _SwGuiCompToolbar.cpp \
    _SwGuiCompTreeView.cpp \
    _SwGuiCompVBoxLayout.cpp \
    _SwGuiCompWidget.cpp \
    _SwGuiCompHttpServer.cpp \
    _SwGuiPluginFactory_Class.cpp \
    _SwGuiPluginsFrontEnd.cpp \
    _SwPropertiesModelImpl.cpp \
    _SwQAbstractItemViewSlots_Catcher.cpp \
    _SwConsoleWidget.cpp \
    _SwGuiVisualLogConsole.cpp \
    _SwGuiVisualUpdater.cpp \
    _SwPerspectivesManager.cpp \
    SwDefaultPerspective.cpp \
    _SwPerspectiveButton.cpp \
    _SwBasicPerspective.cpp  \
    _SwGuiCompGridLayout.cpp  \
    _SwGuiCompGridLayoutCell.cpp \
    _SwGuiCompActionList.cpp  \
    _SwGUICssDialog.cpp \
    _SwGuiCssEditor.cpp \
    _SwGuiCssHighlighter.cpp \
	_SwGuiConsole.cpp \
	_SwGuiCompStackedWidget.cpp \
	_SwGuiCompQActionToStackedWidget.cpp \
	_SwActionStackedWidget.cpp \
	_QRcViewer.cpp \
	_SwServiceShortcuts.cpp \
	_SwGuiQActionToWidget.cpp \
	_SwContainerCloseableWidget.cpp


FORMS += _SwGuiCssDialog.ui
FORMS += _QRcViewerUi.ui
RESOURCES += _resources/SwGuiRsc.qrc
DEFINES += SWGUI_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./\
    ./_intermediaire \
    ../SwCore/ \
    ../SwExecution/ 
    
    
    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../lib/vc/release" -lSwCore
	TARGET_EXT = .swdl
}
