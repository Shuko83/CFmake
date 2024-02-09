#! [BOOST rules]
TARGET = MyApp
TEMPLATE = app

CONFIG += boost   # enables Boost support
BOOST += log      # link to Boost Log library
#! [BOOST rules]

#! [TARGET rules]
TARGET = MyApp[d]
TEMPLATE = app
#! [TARGET rules]

#! [LIBRARIES rules]
add_library( shared, $${INSTALLDIR}/shared, MyLib[d] )
#! [LIBRARIES rules]

#! [INSTALLDIR rules]
TARGET = MyLib
TEMPLATE = lib
CONFIG += shared

target.path = library    # MyLib deployed in delivery/library

include.files = *.h
include.path = include   # header files deployed in delivery/include

INSTALLDIR = delivery
INSTALLS += target include
#! [INSTALLDIR rules]

#! [BOOST reference]
BOOST.CONFIG += static   # Boost libraries will be linked statically
BOOST += log graph       # link to Boost Log and Graph libraries
#! [BOOST reference]

#! [IQUOTEPATH reference]
IQUOTEPATH *= C:/myproject/sources
#! [IQUOTEPATH reference]

#! [LIBRARYPATH reference]
win32: LIBRARYPATH *= "C:/mylibs/extra libs"
linux: LIBRARYPATH *= "/home/user/extra libs"
#! [LIBRARYPATH reference]

#! [PLUGINS reference]
win32: PLUGINS *= "../myplugins/plugin.dll"
linux: PLUGINS *= "../myplugins/libplugin.so"
#! [PLUGINS reference]

#! [PROPERTIES reference]
PROPERTIES.name = MyApp
PROPERTIES.version = 1.2.3
PROPERTIES.company = Diginext
PROPERTIES.domain = diginext.fr
PROPERTIES.copyright = Copyright (C) %y %c
#! [PROPERTIES reference]

#! [QML_FILES reference]
QML_FILES = MainWindow.qml \
            Page1.qml Page2.qml
#! [QML_FILES reference]

#! [QSMAKEPATH reference]
message( $${QSMAKEPATH} )
#! [QSMAKEPATH reference]

#! [TOOLKIT reference]
msvc: greaterThan( TOOLKIT.compiler.version, 2012 }: \
    message( "Compiler MSVC 2013 or later detected" )
#! [TOOLKIT reference]

#! [QUERY_TOOLKIT_PROPERTY reference]
qmake QUERY_TOOLKIT_PROPERTY=mkspec
#! [QUERY_TOOLKIT_PROPERTY reference]

#! [_PRO_FILE_NAME_ reference]
message( $${_PRO_FILE_NAME_} )
#! [_PRO_FILE_NAME_ reference]


#! [BUILD_MODE reference]
CONFIG += debug_and_release
DESTDIR = "builds/${BUILD_MODE}"

# in meta-target generation pass: DESTDIR = "builds"
# in debug target build pass: DESTDIR = "builds/debug"
# in release target build pass: DESTDIR = "builds/release"
#! [BUILD_MODE reference]


#! [LIBRARY_MODE reference]
TEMPLATE = lib
CONFIG += static_and_shared
DESTDIR = "builds/${LIBRARY_MODE}"

# in meta-target generation pass: DESTDIR = "builds"
# in static target build pass: DESTDIR = "builds/static"
# in shared target build pass: DESTDIR = "builds/shared"
#! [LIBRARY_MODE reference]


#! [append() reference]
LIBRARIES = MyLib1 MyLib2
LIBRARIES = $$append( LIBRARY, .dll )   # LIBRARIES = MyLib1.dll MyLib2.dll
#! [append() reference]

#! [prepend() reference]
LIBRARIES = MyLib1.so MyLib2.so
LIBRARIES = $$prepend( LIBRARIES, lib )   # LIBRARIES = libMyLib1.so libMyLib2.so
#! [prepend() reference]

#! [remove() reference]
MESSAGE = This is a test example.
MESSAGE = $$remove( MESSAGE, [ ]?example )
message( $${MESSAGE} )   # prints: "This is a test."
#! [remove() reference]

#! [TARGET() reference]
CONFIG += debug_and_release
TARGET = MyApp[d]

message( $$TARGET() )            # prints: "MyApp"
message( $$TARGET( release ) )   # prints: "MyApp"
message( $$TARGET( debug ) )     # prints: "MyAppd"
#! [TARGET() reference]


#! [add_library() reference]
add_library( static, $${INSTALLDIR}/static, MyLib )           # links to static library "MyLib"
add_library( shared, $${INSTALLDIR}/shared, MyLib1 MyLib2 )   # links to shared libraries "MyLib1" and "MyLib2"
#! [add_library() reference]

#! [add_plugin() reference]
add_plugin( $${INSTALLDIR}/plugins, MyPlugin )   # registers plugin "MyPlugin"
#! [add_plugin() reference]

#! [assert() reference]
assert( !isEmpty( SOURCES ) )
#! [assert() reference]

#! [build_mode() reference]
build_mode( release ): DESTDIR = "builds/release"
build_mode( debug ): DESTDIR = "builds/debug"
#! [build_mode() reference]

#! [build_pass() reference]
TARGET = MyApp
TEMPLATE = app

build_pass(): message( $${TARGET} )        # prints: "MyApp" in debug/release target build pass
build_pass( lib ): message( $${TARGET} )   # never prints: "MyApp"
#! [build_pass() reference]

#! [endsWith() reference]
endsWith( ICONS, .ico ): \
    message( "$${ICONS} is a valid Windows icon file" )
#! [endsWith() reference]

#! [find() reference]
find( QMAKE_ARGS, ^CONFIG ): \
    message( "qmake arguments defines CONFIG variable" )
#! [find() reference]

#! [isNull() reference]
isNull( TARGET ): \
    error( "TARGET variable must be defined" )
#! [isNull() reference]

#! [QT() reference]
QT( gui ): message( "Windowed application detected" )
else:  message( "Console application detected" )
#! [QT() reference]

#! [qtGreaterThan() reference]
qtGreaterThan( 5.2, true ): \
    message( "Qt 5.2 or later detected" )
#! [qtGreaterThan() reference]

#! [startsWith() reference]
win32: !startsWith( _PRO_FILE_PWD_, [A-Z]: ) \
    message( "_PRO_FILE_PWD_ defines an absolute path" )
#! [startsWith() reference]

#! [TEMPLATE() reference]
TEMPLATE( static ): INSTALLDIR = "../delivery/static"
TEMPLATE( shared ): INSTALLDIR = "../delivery/shared"
#! [TEMPLATE() reference]

#! [QUERY_TAMPLATE_PROPERTY reference]
qmake QUERY_TARGET_PROPERTY=template
#! [QUERY_TAMPLATE_PROPERTY reference]
