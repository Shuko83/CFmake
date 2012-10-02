TEMPLATE = lib
TARGET = SwDataBase
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS += SwDataBasicBuffer_Class.h \
    SwDatasBaseConstantes.h \
    _SwDatasBaseBasicRouter.h \
    _SwDatasBasePluginFactory_Class.h \
	_SwSimpleDataExecutable.h
SOURCES += SwDataBasicBuffer_Class.cpp \
    _SwDatasBaseBasicRouter.cpp \
    _SwDatasBasePluginFactory_Class.cpp \
	_SwSimpleDataExecutable.cpp
FORMS +=
RESOURCES += _resources/SwDatasBaseRsc.qrc
DEFINES += SWDATASBASE_LIB
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
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwExecutiond 
	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwExecution 
	TARGET_EXT = .swdl
}
