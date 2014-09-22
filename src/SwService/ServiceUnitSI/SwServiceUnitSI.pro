TEMPLATE = lib
TARGET = SwServiceUnitSI

################################################################################
# PROJECT
################################################################################

PROJECTS_PATH = $$PWD/../../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml

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
		../../SwCore/ \
		../../SwCore/Component/Base/\
		../../SwCore/Component/Interfaces/\
		../../SwCore/Component/Pin/\
		../../SwCore/Main/ \
		../../SwCore/Main/Plugin/ \
		../../SwCore/Main/Services/Management/ \
		../../SwCore/SwRef/ \
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

LIBS += -L"../../../$$LIB_PATH" \
		-L"$$PROJECTS_PATH/Libraries/GeographicLib/$$LIB_PATH"
		

CONFIG(debug, debug|release) {	
	LIBS +=  -lGeographicLibd \
			 -lSwCored
			 
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lGeographicLib \
			-lSwCore
			
	TARGET_EXT = .swdl
}

################################################################################
# SPECIFIC INCLUDES
################################################################################

INCLUDEPATH += \
		$$PROJECTS_PATH/Libraries/GeographicLib/src

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)