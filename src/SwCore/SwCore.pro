TEMPLATE = lib
TARGET = SwCore

################################################################################
# PROJECT
################################################################################

PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core xml widgets

CONFIG += StdAfx.h
PRECOMPILED_HEADER = StdAfx.h
win32-msvc* {
PRECOMPILED_SOURCE = StdAfx.cpp
}

QMAKE_CXXFLAGS += /MP
				
HEADERS += *.h \
		Component/Base/*.h\
		Component/Interfaces/*.h\
		Component/Pin/*.h\
		Component/Services/*.h\
		Component/Services/ServiceImpl/*.h\
		Extensions/*.h\
		Main/*.h\
		Main/Connexion/*.h\
		Main/Plugin/*.h\
		Main/Serialization/*.h\
		Main/Services/*.h\
		Main/Services/Management/*.h\
		Main/Services/ServiceConfiguration/*.h\
		Main/Services/ServiceImpl/*.h\
		Main/Services/ServiceShortcut/*.h\
		Main/Services/ServiceUnitSI/*.h\
		Properties/*.h\
		SwRef/*.h\
		Tools/*.h\
		Tools/Exception/*.h\
		Tools/Log/*.h\
		Tools/Signal/*.h\
		Types/*.h\
		Types/UserType/*.h\

SOURCES += *.cpp \
		Component/Base/*.cpp\
		Component/Pin/*.cpp\
		Component/Services/ServiceImpl/*.cpp\
		Extensions/*.cpp\
		Main/*.cpp\
		Main/Connexion/*.cpp\
		Main/Plugin/*.cpp\
		Main/Serialization/*.cpp\
		Main/Services/Management/*.cpp\
		Main/Services/ServiceConfiguration/*.cpp\
		Main/Services/ServiceImpl/*.cpp\
		Main/Services/ServiceShortcut/*.cpp\
		Main/Services/ServiceUnitSI/*.cpp\
		Properties/*.cpp\
		SwRef/*.cpp\
		Tools/*.cpp\
		Tools/Exception/*.cpp\
		Tools/Log/*.cpp\
		Types/*.cpp\
		Types/UserType/*.cpp\
    
FORMS +=

RESOURCES += _resources/SwCoreRsc.qrc

DEFINES += SWCORE_LIB

INCLUDEPATH += ./ \
		Component/\
		Component/Base/\
		Component/Interfaces/\
		Component/Pin/\
		Component/Services/\
		Component/Services/ServiceImpl/\
		Extensions/ \
		Main/ \
		Main/Connexion/ \
		Main/Plugin/ \
		Main/Serialization/ \
		Main/Services/ \
		Main/Services/Management/ \
		Main/Services/ServiceConfiguration/ \
		Main/Services/ServiceImpl/ \
		Main/Services/ServiceShortcut/ \
		Main/Services/ServiceUnitSI/\
		Properties/ \
		SwRef/ \
		Tools/ \
		Tools/Exception/ \
		Tools/Log/ \
		Tools/Signal/ \
		Types/ \
		Types/UserType/ \

win32:DEFINES +=_CRT_SECURE_NO_WARNINGS

win32:{
    LIBS += -lPsapi -limagehlp
}    

DESTDIR = ./../../$$LIB_PATH
DLLDESTDIR=  ./../../$$BIN_PATH

LIBS += -L"$$PROJECTS_PATH/Libraries/GeographicLib/$$LIB_PATH"

CONFIG(debug, debug|release) {	
	LIBS += -lGeographicLibd 
} 

CONFIG(release, debug|release) {
	LIBS += -lGeographicLib
}

################################################################################
# SPECIFIC INCLUDES
################################################################################

INCLUDEPATH += \
	$$PROJECTS_PATH/Libraries/GeographicLib/src 
	

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)