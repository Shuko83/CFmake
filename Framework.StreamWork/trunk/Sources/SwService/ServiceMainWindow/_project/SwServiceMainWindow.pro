#################################################################################################################
#																												#
# 											GENERATED FILE														# 
# 											DO NOT MODIFY														#
#																												#
#################################################################################################################

PROJECT.TYPE 		= SHARED_LIB
PROJECT.NAME 		= SwServiceMainWindow
PROJECT.ARTIFACT 	= ../../../../Delivery/SwService/SwServiceMainWindow

# Define BuildConfig
PROJECT.ARCH	 	= x86
PROJECT.OS			= win
PROJECT.COMPILO 	= msvc
PROJECT.COMPILOVER	= 2013
PROJECT.QT		 	= QT5.5.0

TARGET_EXT 	= .swdl
QT += core widgets xml gui

#####################
## Project Sources ##
#####################

# -- HEADER -- #
HEADERS += $$files(../*.h) 
HEADERS += $$files(../*.hpp) 

# -- SOURCES -- #

SOURCES += $$files(../*.c)
SOURCES += $$files(../*.cpp)
SOURCES += $$files(../*.cxx)
SOURCES += $$files(../*.cc)

# -- INCLUDEPATH -- #
INCLUDEPATH += ../

#######################
## Project Libraries ##
#######################

INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsCore/include
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsCore/include/private
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsException/include
INCLUDEPATH += ../../../../../../Libraries/Qst/Delivery/QsGui/include
INCLUDEPATH += ../../../../Delivery/SwCore/include
INCLUDEPATH += ../../../../Delivery/SwCore/include/Component/Base
INCLUDEPATH += ../../../../Delivery/SwCore/include/Component/Interfaces
INCLUDEPATH += ../../../../Delivery/SwCore/include/Component/Pin
INCLUDEPATH += ../../../../Delivery/SwCore/include/Component/Services
INCLUDEPATH += ../../../../Delivery/SwCore/include/Component/Services/ServiceImpl
INCLUDEPATH += ../../../../Delivery/SwCore/include/Extensions
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main/Connexion
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main/Plugin
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main/Serialization
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main/Services
INCLUDEPATH += ../../../../Delivery/SwCore/include/Main/Services/Management
INCLUDEPATH += ../../../../Delivery/SwCore/include/Properties
INCLUDEPATH += ../../../../Delivery/SwCore/include/SwRef
INCLUDEPATH += ../../../../Delivery/SwCore/include/Tools
INCLUDEPATH += ../../../../Delivery/SwCore/include/Tools/Exception
INCLUDEPATH += ../../../../Delivery/SwCore/include/Tools/Log
INCLUDEPATH += ../../../../Delivery/SwCore/include/Tools/Signal
INCLUDEPATH += ../../../../Delivery/SwCore/include/Types
INCLUDEPATH += ../../../../Delivery/SwCore/include/Types/UserType

LIBRARYPATH += ../../../../Delivery/SwCore/win-msvc2013-x86-QT5.5.0/${BUILD_MODE}

###################
## Project Debug ##
###################

CONFIG(debug, debug|release) {
	LIBS += -lSwCored
}

#####################
## Project Release ##
#####################

CONFIG(release, debug|release) { 
	LIBS += -lSwCore
}

include(../../../../../../Libraries/Qst/Delivery/qst_rules.pri)

