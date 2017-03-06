TEMPLATE = lib
TARGET = SwServiceMainWindow
DEFINES+=SWSERVICEMAINWINDOW_LIB
PROJECTS_PATH = $$PWD/../../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core widgets xml gui


# -- HEADER -- #
HEADERS += $$files(./*.h) 

# -- SOURCES -- #
SOURCES += $$files(./*.cpp)

# -- INCLUDEPATH -- #
INCLUDEPATH += ./

#######################
## Project Libraries ##
#######################

INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsCore/include
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsCore/include/private
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsException/include
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsGui/include
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsLog/include
INCLUDEPATH += ../../SwCore/
INCLUDEPATH += ../../SwCore/Component/Base
INCLUDEPATH += ../../SwCore/Component/Interfaces
INCLUDEPATH += ../../SwCore/Component/Pin
INCLUDEPATH += ../../SwCore/Component/Services
INCLUDEPATH += ../../SwCore/Component/Services/ServiceImpl
INCLUDEPATH += ../../SwCore/Extensions
INCLUDEPATH += ../../SwCore/Main
INCLUDEPATH += ../../SwCore/Main/Connexion
INCLUDEPATH += ../../SwCore/Main/Plugin
INCLUDEPATH += ../../SwCore/Main/Serialization
INCLUDEPATH += ../../SwCore/Main/Services
INCLUDEPATH += ../../SwCore/Main/Services/Management
INCLUDEPATH += ../../SwCore/Properties
INCLUDEPATH += ../../SwCore/SwRef
INCLUDEPATH += ../../SwCore/Tools
INCLUDEPATH += ../../SwCore/Tools/Exception
INCLUDEPATH += ../../SwCore/Tools/Log
INCLUDEPATH += ../../SwCore/Tools/Signal
INCLUDEPATH += ../../SwCore/Types
INCLUDEPATH += ../../SwCore/Types/UserType


DESTDIR = ./../../../$$LIB_PATH/
DLLDESTDIR=  ./../../../$$BIN_PATH/

LIBS += -L"../../../$$LIB_PATH"

###################
## Project Debug ##
###################

CONFIG(debug, debug|release) {
	LIBS += -lSwCored
	
	TARGET_EXT = .swdld
}

#####################
## Project Release ##
#####################

CONFIG(release, debug|release) { 
	LIBS += -lSwCore
	
	TARGET_EXT = .swdl
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)
