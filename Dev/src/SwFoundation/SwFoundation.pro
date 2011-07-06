TEMPLATE = lib
TARGET = SwFoundation
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS +=  SwAssistedComponent.h 

    
SOURCES +=  SwAssistedComponent.cpp 
			
    
FORMS +=
DEFINES += SWFOUNDATION_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore/ \
	../SwGui/ \
	../SwExecution

    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored -lSwGuid -lSwExecutiond
	TARGET_EXT = .dlld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../lib/vc/release" -lSwCore -lSwGui -lSwExecution
	TARGET_EXT = .dll
}
