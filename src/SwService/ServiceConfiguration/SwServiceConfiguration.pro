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
				
HEADERS += $$files(*.h)
SOURCES += $$files(*.cpp) 

INCLUDEPATH += ./ \
		../../SwCore/Component/Base/\
		../../SwCore/Component/Interfaces/\
		../../SwCore/Component/Pin/\
		../../SwCore/Component/Services/\
		../../SwCore/Main/ \
		../../SwCore/Main/Connexion \
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
		../../SwCore\Component\Services\ServiceImpl \
		../../SwCore/  \
		../../SwFoundation/ \
		../../SwService\ServiceShortcut \
		../../SwExecution \
		../../SwGui \
	


win32:DEFINES +=_CRT_SECURE_NO_WARNINGS


DESTDIR = ./../../../$$LIB_PATH/service
DLLDESTDIR=  ./../../../$$BIN_PATH/service

LIBS += -L"../../../$$LIB_PATH"

CONFIG(debug, debug|release) {	
	LIBS += -lSwCored -lSwFoundationd
	TARGET_EXT = .swdld
} 

CONFIG(release, debug|release) {
	LIBS += -lSwCore -lSwFoundation
	TARGET_EXT = .swdl
}

	

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)