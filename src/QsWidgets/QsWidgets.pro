TEMPLATE = lib
TARGET = QsWidget
DEFINES*=QSWIDGETS_LIB

################################################################################
# PROJECT
################################################################################
PROJECTS_PATH = $$PWD/../../../..

include($$PROJECTS_PATH/Utilities/QtCommonPri/preDefine.pri)

QT += core core-private designer gui network widgets xml


DEFINES += QT_QTTHUMBWHEEL_EXPORT
CONFIG += plugin


#####################
## Project Sources ##
#####################

# -- HEADER -- #
HEADERS += $$files(*.h) 
HEADERS += $$files(QsSmartCombo/*.h) 
HEADERS += $$files(QsSmartCombo/CustomListWidget/*.h) 
HEADERS += $$files(QsTreeView/*.h) 

# -- SOURCES -- #


SOURCES += $$files(*.cpp)
SOURCES += $$files(QsSmartCombo/*.cpp)
SOURCES += $$files(QsSmartCombo/CustomListWidget/*.cpp)
SOURCES += $$files(QsTreeView/*.cpp)

# -- FORMS -- #
FORMS += $$files(*.ui)
FORMS += $$files(QsSmartCombo/CustomListWidget/*.ui)
FORMS += $$files(QsTreeView/*.ui)

# -- INCLUDEPATH -- #
INCLUDEPATH += ./
INCLUDEPATH += ./QsSmartCombo
INCLUDEPATH += ./QsSmartCombo/CustomListWidget
INCLUDEPATH += ./QsTreeView

# -- RESOURCES -- #
RESOURCES += $$files(_resources/*.qrc)

#######################
## Project Libraries ##
#######################

INCLUDEPATH += $$PROJECTS_PATH/Libraries/GeographicLib/src
INCLUDEPATH += $$PROJECTS_PATH/Libraries/Qst/src/QsCore
INCLUDEPATH += $$PROJECTS_PATH/Libraries/Qst/src/QsCore/private
INCLUDEPATH += $$PROJECTS_PATH/Libraries/Qst/src/QsException
INCLUDEPATH += $$PROJECTS_PATH/Libraries/Qst/src/QsGui
INCLUDEPATH += $$PROJECTS_PATH/Modules/Streamwork/src/SwServiceUnitSI
INCLUDEPATH += $$PROJECTS_PATH/Libraries/ServiceUnitSI/src
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Component/Base
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Component/Interfaces
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Component/Pin
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Component/Services
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Component/Services/ServiceImpl
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Extensions
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main/Connexion
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main/Plugin
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main/Serialization
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main/Services
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Main/Services/Management
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Properties
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/SwRef
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Tools
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Tools/Exception
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Tools/Log
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Tools/Signal
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Types
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore/Types/UserType
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwExecution
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwFoundation
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwService/SwServiceShortcut
INCLUDEPATH += $$PROJECTS_PATH/Libraries/StreamWork/src/SwService/SwServiceTools


###################
## Project Debug ##
###################

CONFIG(debug, debug|release) {
}

#####################
## Project Release ##
#####################

CONFIG(release, debug|release) { 
}

include($$PROJECTS_PATH/Utilities/QtCommonPri/base.pri)

