TEMPLATE = app
TARGET = SwGenerator
QMAKE_CXXFLAGS += /MP
QT += core \
    xml
HEADERS +=  *.h 
SOURCES +=  *.cpp 
FORMS += *.ui
RESOURCES += SwGenerator.qrc
DEFINES += SWGENERATOR_EXE
MOC_DIR += ./_intermediaire
OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
INCLUDEPATH += ./ \
    ./_intermediaire \
	../../../QCTemplate/src/
    
CONFIG(debug, debug|release) {
	win32:DLLDESTDIR=  ./../../bin/vc/debug
	win32:DESTDIR=  ./../../bin/vc/debug
	win32:TARGET = $$join(TARGET,,,d)
	win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
	LIBS += -L"../../../QCTemplate/lib/vc/debug" -llibctemplated -lQCTemplated
	
	QMAKE_POST_LINK = xcopy /Y ..\\..\\..\\QCTemplate\\bin\\vc\\debug\\*.dll .\\..\\..\\bin\\vc\\debug\\
} 

CONFIG(release, debug|release) {
	win32:DESTDIR = ./../../bin/vc/release
	win32:DLLDESTDIR=  ./../../bin/vc/release
	LIBS += -L"../../../QCTemplate/lib/vc/release" -llibctemplate -lQCTemplate
	
	QMAKE_POST_LINK = xcopy /Y ..\\..\\..\\QCTemplate\\bin\\vc\\release\\*.dll .\\..\\..\\bin\\vc\\release\\
}
