TEMPLATE = lib
TARGET = SwFoundation
DEFINES += SWFOUNDATION_LIB
PROJECTS_PATH = ../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml
	
HEADERS +=  *.h 
    
SOURCES +=  *.cpp 
    
FORMS +=

DEFINES += SWFOUNDATION_LIB

INCLUDEPATH += ./ 
INCLUDEPATH += ../SwCore
INCLUDEPATH += ../SwGui
INCLUDEPATH += ../SwExecution 
INCLUDEPATH += ../SwService/ServiceShortcut 
INCLUDEPATH += ../SwService/ServiceImpl 
INCLUDEPATH += ../SwService/ServiceTools 
INCLUDEPATH += $$PROJECTS_PATH\Modules\Streamwork\src\SwServiceUnitSI 
INCLUDEPATH += $$PROJECTS_PATH\Libraries\ServiceUnitSI\src

DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"../../$$LIB_PATH"
   
CONFIG(debug, debug|release) {	
	LIBS += -lSwCored -lSwExecutiond
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {	
	LIBS += -lSwCore -lSwExecution
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
include($$PROJECTS_PATH/Libraries/StreamWork/baseSwCore.pri)