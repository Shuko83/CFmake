TEMPLATE = lib
TARGET = SwModel2
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets
} 
	
HEADERS += $$files(*.h)
		
SOURCES += $$files(*.cpp)
	
RESOURCES += _resources/SwModel2Rsc.qrc

DEFINES += SWMODEL2_LIB

INCLUDEPATH += ./ \
	../SwCore \
	../SwExecution
	
DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH" 

CONFIG(debug, debug|release) {
	LIBS += -lSwCored -lSwExecutiond
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {	
	LIBS +=	-lSwCore -lSwExecution
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)