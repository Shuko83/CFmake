TEMPLATE = lib
TARGET = SwEditor
PROJECTS_PATH = $$PWD/../../../..

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

PLUGINS=SW


include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

HEADERS += $$files(*.h)
    
SOURCES += $$files(*.cpp)
    
    
FORMS +=

RESOURCES += _resources/SwEditorRsc.qrc

DEFINES += SWEDITOR_LIB

INCLUDEPATH += ./ \
    ../SwCore \
    ../SwExecution \
    ../SwGui \
    ../StreamWorkEditor2
    
DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../..$$BIN_PATH

LIBS += -L"../../$$LIB_PATH" 
    
CONFIG(debug, debug|release) {
	LIBS += -lSwCored \
    -lSwExecutiond \
    -lSwGuid

	TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {	
	LIBS += -lSwCore \
    -lSwExecution \
    -lSwGui

	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)