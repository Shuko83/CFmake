TEMPLATE = app
TARGET = StreamWorkEditor2
PROJECTS_PATH = $$PWD/../../../..


include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core gui xml opengl widgets printsupport

HEADERS += $$files(*.h)
HEADERS += ../SwModel2/ISwModelHostModifier.h

SOURCES += $$files(*.cpp)

FORMS += _resources/LogView.ui \
	_resources/PluginOverview.ui
	
RESOURCES += _resources/StreamWorkEditor2.qrc

INCLUDEPATH += ./ \
		../SwCore \
		../SwGui \
		../SwExecution \
		../SwDoc \
		../SwModel2 \
		../SwService/ServiceImpl \
		../SwService/ServiceShortcut \
		../SwService/ServiceUnitSI \
		../SwService/ServiceConfiguration   \
		../SwService/ServiceTools \
		$$PROJECTS_PATH/Libraries/Qst/src/QsGui \
		$$PROJECTS_PATH/Libraries/Qst/src/QsCore \
		$$PROJECTS_PATH/Libraries/Qst/src/QsLog

DEFINES += SW_EDITOR
    
VERSION = 1.2.3 

DESTDIR = .\\..\\..\\$$BIN_PATH
DLLDESTDIR=  .\\..\\..\\$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"
LIBS += -L"$$PROJECTS_PATH/Libraries/Qst/$$LIB_PATH"

LIBS += opengl32.lib

CONFIG(debug, debug|release) {	
	LIBS += -lSwCored 
	LIBS += -lSwGuid 
	LIBS += -lQsLogd
	CONFIG += console
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore 
	LIBS += -lSwGui 
	LIBS += -lQsLog
	QMAKE_POST_LINK = "$${QMAKE_COPY} ..\\..\\..\\..\\Libraries\\ExceptionManager\\$$BIN_PATH\\ExceptionManager.dll ..\\..\\$$BIN_PATH"
}

win32:RC_FILE = StreamWorkEditor2.rc

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)