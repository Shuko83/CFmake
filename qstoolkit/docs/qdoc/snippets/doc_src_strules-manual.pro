#! [standard rules]
# Target Definition

TARGET = MyLib
TEMPLATE = lib
CONFIG += shared

INSTALLDIR = ../../Delivery/AppDir

# Sources Definition

HEADERS = ...
SOURCES = ...

# ST Rules Include

include( $${QSMAKEPATH}/rules/standard_project.pri )
#! [standard rules]


#! [BUILD_MKSPEC reference]
DESTDIR = "builds/${BUILD_MKSPEC}"   # application based on Qt: DESTDIR = "builds/win-msvc2013-x64-QT5.6.3"
CONFIG -= qt                         # application no more based on Qt: DESTDIR = "builds/win-msvc2013-x64"
#! [BUILD_MKSPEC reference]


#! [application example]
# Target Definition

TARGET = UDLTracksEmitter
TEMPLATE = app

# Qt Configuration

CONFIG += no_output_dir
QT += xml widgets

# C++ Compiler Options

INCLUDEPATH *= ${DELIVERY}/CODX/UDL/include/elements \
               ${DELIVERY}/CODX/UDL/include/messages

# Sources Definition

HEADERS *= CoreApplication.h \
           MainWindow.h \
           UDLProcessor.h

SOURCES *= CoreApplication.cpp
           MainWindow.cpp \
           UDLProcessor.cpp

# Resources Definition

FORMS *= MainWindow.ui
RESOURCES *= UDLTracksEmitter.qrc

# Dependencies Definition

add_library( shared, ${DELIVERY}/CODX/UDL/$${TOOLKIT.mkspec}/${BUILD_MODE}, CodxUDLElements[d] \
                                                                            CodxUDLMessages[d] )

# ST Rules Include

include( $${QSMAKEPATH}/rules/standard_project.pri )
#! [application example]

#! [library example]
# Target Definition

TARGET = CodxUDLMessages
TEMPLATE = lib
CONFIG += shared

INSTALLDIR = ${DELIVERY}/CODX/UDL

# Qt Configuration

CONFIG -= qt

# C++ Compiler Options

INCLUDEPATH *= ../license \
               ../elements

# Sources Definition

HEADERS *= $$files( $${PWD}/*.h )
SOURCES *= $$files( $${PWD}/*.cpp )

# Dependencies Definition

add_library( static, ${DELIVERY}/CODX/UDL/${BUILD_MKSPEC}/${BUILD_MODE}, CodxUDLLicense[d] )
add_library( shared, ${DELIVERY}/CODX/UDL/${BUILD_MKSPEC}/${BUILD_MODE}, CodxUDLElements[d] )

# ST Rules Include

include( $${QSMAKEPATH}/rules/standard_project.pri )
#! [library example]

#! [subdirectories example]
# Target Definition

TARGET = CodxUDL
TEMPLATE = subdirs

# Subdirectories Definition

add_subdir( CodxUDLLicense, licence )
add_subdir( CodxUDLElements, elements, CodxUDLLicense )
add_subdir( CodxUDLMessages, messages, CodxUDLElements )

# ST Rules Include

include( $${QSMAKEPATH}/rules/standard_project.pri )
#! [subdirectories example]
