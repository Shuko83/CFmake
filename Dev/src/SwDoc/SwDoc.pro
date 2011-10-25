TEMPLATE = lib
TARGET = SwDoc
QMAKE_CXXFLAGS += /MP
QT += core \
    sql
HEADERS +=  *.h 
SOURCES +=  *.cpp 
FORMS += _resources/*.ui
RESOURCES += SwDoc.qrc
DEFINES += SWDOC_LIB
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire

INCLUDEPATH += ./ \
    ./_intermediaire \
	../../../../QDjango/Stable/src/
	
    
CONFIG(debug, debug|release) {
  win32:DESTDIR = ./../../lib/vc/debug
  win32:DLLDESTDIR=  ./../../bin/vc/debug
  win32:TARGET = $$join(TARGET,,,d)
  win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../../../QDjango/Stable/lib/vc/debug" -lQDjangod
	LIBS += -L"../../lib/vc/debug" -lSwCored
	TARGET_EXT = .dlld
} 
CONFIG(release, debug|release) {
  win32:DESTDIR = ./../../lib/vc/release
  win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../../../QDjango/Stable/lib/vc/release" -lQDjango
	LIBS += -L"../../lib/vc/release" -lSwCore
	TARGET_EXT = .dll
}
