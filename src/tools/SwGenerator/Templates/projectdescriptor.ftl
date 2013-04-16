<#-- Project Qt generation -->

<#assign project_name= doc.SwFactoryEntry.@Name>

<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
<#-- Assignation des repertoires en distribution interne-->
<#assign sw_dirs="../..">
<#assign sw_dirs_bin_win32="..\\..\\bin">
<#assign add_lib_dir="..\\..\\..\\lib">
<#assign import_lib_dir="lib">
<#assign src_dir="src">
<#assign rsc_dir="resources">


<#else>

<#-- Assignation des repertoires en distribution externe-->
<#assign sw_dirs="$"+"(SW_DIR)">
<#assign sw_dirs_bin_win32="$(SW_DIR)\\bin">
<#assign add_lib_dir="$$SW_DIR/lib">
<#assign import_lib_dir="lib">
<#assign src_dir="src">
<#assign rsc_dir="resources">
</#if>

<#assign sw_dirs_bin="${sw_dirs}/bin/vc">
<#assign sw_dirs_src="${sw_dirs}/src">
<#assign sw_dirs_lib="${sw_dirs}/lib/vc">
<#assign sw_dirs_include="${sw_dirs}/src">

<#-- Les donnees generees commencent ici-->

# -----------------------------------------------------------
# StreamWork Qt Plugin Project / Generated File
# -----------------------------------------------------------

<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
CONFIG+= install
</#if>

CONFIG+= plugin

win32 {

	message("win 32 build")
	DEFINES+= WIN32
}


unix {

	DEFINES+= UNIX

}


# -----------------------------------------------------------
# This is .pri file
# -----------------------------------------------------------


VPATH += ./src
VPATH += ./tmp
VPATH += ./tmp/moc
VPATH += ./include

INCLUDEPATH += ./include
INCLUDEPATH += ./tmp
INCLUDEPATH += ./tmp/moc



<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
INCLUDEPATH += ../SwCore
INCLUDEPATH += ../SwExecution
INCLUDEPATH += ../SwGui
<#else>
INCLUDEPATH += ${sw_dirs_include}/SwCore
INCLUDEPATH += ${sw_dirs_include}/SwExecution
INCLUDEPATH += ${sw_dirs_include}/SwGui
</#if>

DEPENDPATH += .
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
DEPENDPATH += ../SwCore
DEPENDPATH += ../SwExecution
DEPENDPATH += ../SwGui
<#else>
</#if>


MOC_DIR += ./tmp/moc
OBJECTS_DIR += ./tmp/obj
UI_DIR += ./tmp
RCC_DIR += ./tmp

TARGET = ${project_name}
DEFINES += "${project_name}_LIB" \
    QT_THREAD_SUPPORT \
    QT_CORE_LIB \
    UNICODE \
    BUILD_LIB
TEMPLATE = lib

QT += xml


CONFIG(debug, debug|release) {
     DESTDIR = ../../lib/vc/debug/
     DLLDESTDIR = ../../bin/vc/debug/
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
     LIBS += -L"../../lib/vc/debug/" 
<#else>
     LIBS += -L"${sw_dirs_lib}/debug" 
</#if>
    -lSwCore -lSwExecution -lSwGui
} else {
     DESTDIR = ../../lib/vc/release/
     DLLDESTDIR = ../../bin/vc/release/
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
     LIBS += -L"../../lib/vc/release/" 
<#else>
     LIBS += -L"${sw_dirs_lib}/release" \
</#if>
    -lSwCore -lSwExecution -lSwGui
    DEFINES += QT_NO_DEBUG \
    NDEBUG
}

#Header files

HEADERS += ${doc.SwFactoryEntry.@Name}PluginFactory_Class.h
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
HEADERS += ${component.@Name}.h
</#if>
</#if>
</#list>
HEADERS += ${project_name}Constantes.h

#Source files
SOURCES += ${doc.SwFactoryEntry.@Name}PluginFactory_Class.cpp
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
SOURCES += ${component.@Name}.cpp
</#if>
</#if>
</#list>


#Resource file(s)
RESOURCES += ./resources/"${project_name}Rsc.qrc"

