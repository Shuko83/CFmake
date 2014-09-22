TEMPLATE = lib
TARGET = SwServiceAlert

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

LIBS += -L"$$PROJECTS_PATH/Libraries/AlertManager/$$LIB_PATH" \
		-L"../../../$$LIB_PATH"
		

CONFIG(debug, debug|release) {	
	LIBS += -lAlertManagerd \
			-lSwCored
	
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	
	LIBS += -lAlertManager \
			-lSwCore
	
	TARGET_EXT = .swdl
}

################################################################################
# SPECIFIC INCLUDES
################################################################################

INCLUDEPATH += \
	$$PROJECTS_PATH/Libraries/AlertManager/src/Interface/\
	$$PROJECTS_PATH/Libraries/AlertManager/src/
	

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)