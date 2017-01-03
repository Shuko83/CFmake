TEMPLATE = lib
TARGET = SwServiceShortcut

################################################################################
# PROJECT
################################################################################

PROJECTS_PATH = $$PWD/../../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core widgets 

CONFIG += StdAfx.h
PRECOMPILED_HEADER = StdAfx.h
win32-msvc* {
PRECOMPILED_SOURCE = StdAfx.cpp
}

QMAKE_CXXFLAGS += /MP
				
HEADERS += $$files(*.h )


SOURCES += *.cpp 

    
FORMS +=

RESOURCES += 

DEFINES += 

INCLUDEPATH += ./ \
		../../SwCore/ \
		../../SwCore/Component/Base/\
		../../SwCore/Component/Interfaces/\
		../../SwCore/Component/Pin/\
		../../SwCore/Main/ \
		../../SwCore/Main/Plugin/ \
		../../SwCore/Main/Services/Management/ \
		../../SwCore/SwRef/ \
		../../SwCore/Tools/ \
		../../SwCore/Tools/ \
		../../SwCore/Tools/Exception/ \
		../../SwCore/Tools/Log/ \
		../../SwCore/Tools/Signal/ \
		../../SwCore/Types/ \
		../../SwCore/Types/UserType/ 


win32:DEFINES +=_CRT_SECURE_NO_WARNINGS

win32:{
    LIBS += 
}    

DESTDIR = ./../../../$$LIB_PATH/service
DLLDESTDIR=  ./../../../$$BIN_PATH/service

LIBS += -L"../../../$$LIB_PATH"
		

CONFIG(debug, debug|release) {	
	LIBS += -lSwCored
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore
	TARGET_EXT = .swdl
}

################################################################################
# SPECIFIC INCLUDES
################################################################################

INCLUDEPATH += 
	

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)