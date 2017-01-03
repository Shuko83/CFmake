TEMPLATE = lib
TARGET = SwExecution
PROJECTS_PATH = $$PWD/../../../..

QT += core xml widgets gui

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)
	
HEADERS += $$files(*.h)
    
SOURCES += $$files(*.cpp)
    
FORMS += $$files(*.ui)

RESOURCES += _resources/SwExecutionRsc.qrc

DEFINES += SWEXECUTION_LIB
INCLUDEPATH += ./ \
    ../SwCore/

DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"
	
CONFIG(debug, debug|release) {
	LIBS += -lSwCored
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore
	TARGET_EXT = .swdl
}


include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)