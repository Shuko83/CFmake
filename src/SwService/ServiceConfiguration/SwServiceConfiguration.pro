TEMPLATE = lib
TARGET = SwServiceConfiguration

################################################################################
# PROJECT
################################################################################

PROJECTS_PATH = $$PWD/../../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml widgets

CONFIG += StdAfx.h
PRECOMPILED_HEADER = StdAfx.h
win32-msvc* {
PRECOMPILED_SOURCE = StdAfx.cpp
}

QMAKE_CXXFLAGS += /MP
				
HEADERS += *.h 


SOURCES += *.cpp 

    
FORMS +=

RESOURCES += 

DEFINES += 

INCLUDEPATH += ./ \
		../../SwCore/Component/Base/\
		../../SwCore/Component/Interfaces/\
		../../SwCore/Component/Pin/\
		../../SwCore/Component/Services/\
		../../SwCore/Main/ \
		../../SwCore/Main/Plugin/ \
		../../SwCore/Main/Services/Management/ \
		../../SwCore/Properties/ \
		../../SwCore/SwRef/ \
		../../SwCore/Tools/ \
		../../SwCore/Tools/Exception/ \
		../../SwCore/Tools/Log/ \
		../../SwCore/Tools/Signal/ \
		../../SwCore/Types/ \
		../../SwCore/Types/UserType/ \
		../../SwCore/ 


win32:DEFINES +=_CRT_SECURE_NO_WARNINGS

win32:{
    LIBS += 
}    

DESTDIR = ./../../../$$LIB_PATH/service
DLLDESTDIR=  ./../../../$$BIN_PATH/service

LIBS += -L"../../../$$LIB_PATH"



CONFIG(debug, debug|release) {	
	LIBS += \
			-lSwCored
			
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += \
			-lSwCore
			
	TARGET_EXT = .swdl
}

################################################################################
# SPECIFIC INCLUDES
################################################################################

INCLUDEPATH += \
	

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)