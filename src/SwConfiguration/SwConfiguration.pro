TEMPLATE = lib
TARGET = SwConfiguration
PROJECTS_PATH = $$PWD/../../../..

QT += core xml gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} 

PLUGINS=SW

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

HEADERS += $$files(*.h)
    
SOURCES += $$files(*.cpp)
    
FORMS += $$files(*.ui)

RESOURCES += _resources/SwConfigurationRsc.qrc

DEFINES += SWCONFIGURATION_LIB

INCLUDEPATH += ./ \
    ../SwCore \
	../SwService/ServiceConfiguration \
    ../SwExecution \
    ../SwGui \
    ../SwModel2 \
	$$PROJECTS_PATH\Libraries\Qst\src\QsGui \
	$$PROJECTS_PATH\Libraries\Qst\src\QsCore

DESTDIR = ../../$$LIB_PATH
DLLDESTDIR=  ../..$$BIN_PATH
LIBS += -L"../../$$LIB_PATH"
	
CONFIG(debug, debug|release) {
	LIBS += -lSwCored -lSwExecutiond -lSwGuid 
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {  
	LIBS += -lSwCore -lSwExecution -lSwGui 
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)