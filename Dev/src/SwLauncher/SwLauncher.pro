TEMPLATE = app
TARGET = SwLauncher
QT += core \
	xml
HEADERS +=
SOURCES += main.cpp
FORMS +=
RESOURCES +=
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
    ../SwCore
   
win32:DESTDIR = ./../../bin/vc/release/      
    
CONFIG(debug, debug|release) {
   win32:DESTDIR = ./../../bin/vc/debug/
    TARGET = $$join(TARGET,,,d)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../lib/vc/debug" -lSwCored 
} 

CONFIG(release, debug|release) {
	LIBS += -L"../../lib/vc/release" -lSwCore 
}

