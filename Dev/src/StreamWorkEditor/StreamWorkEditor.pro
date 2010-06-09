TEMPLATE = app
TARGET = StreamWorkEditor
QMAKE_CXXFLAGS += /MP
QT += core \
    gui \
    xml
HEADERS += SwSplash.h
SOURCES += SwSplash.cpp \
    main.cpp
FORMS +=
RESOURCES += _resources/StreamWorkEditor.qrc
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

#Windows resource file
win32:RC_FILE = StreamWorkEditor.rc
