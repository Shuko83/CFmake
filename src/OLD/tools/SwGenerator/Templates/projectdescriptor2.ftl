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
TEMPLATE = lib
TARGET = ${project_name}
QT += core \
    xml
HEADERS += ${doc.SwFactoryEntry.@Name}PluginFactory_Class.h
HEADERS += ${project_name}Constantes.h
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
HEADERS += ${component.@Name}.h 
</#if>
</#if>
</#list>


SOURCES += ${doc.SwFactoryEntry.@Name}PluginFactory_Class.cpp
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
SOURCES += ${component.@Name}.cpp
</#if>
</#if>
</#list>


OBJECTS_DIR += ./_obj
RCC_DIR += ./_intermediaire
UI_DIR += ./_intermediaire
#Resource file(s)
RESOURCES += ./_resources/"${project_name}Rsc.qrc"
DEFINES += "${project_name}_LIB"
INCLUDEPATH += src
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
INCLUDEPATH += ../SwCore
INCLUDEPATH += ../SwExecution
INCLUDEPATH += ../SwGui
<#else>
INCLUDEPATH += ${sw_dirs_include}/SwCore
INCLUDEPATH += ${sw_dirs_include}/SwExecution
INCLUDEPATH += ${sw_dirs_include}/SwGui
</#if>

CONFIG(debug, debug|release) {
    win32:DESTDIR = ./../../lib/vc/debug
    win32:DLLDESTDIR=  ./../../bin/vc/debug
    win32:TARGET = $$join(TARGET,,,d)
    win32:QMAKE_LFLAGS_DEBUG = /PDB:$$DLLDESTDIR/$$join(TARGET,,,.pdb)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
    LIBS += -L"../../lib/vc/debug/" \
<#else>
    LIBS += -L"${sw_dirs_lib}/debug" \
</#if>
    -lSwCored \
    -lSwExecutiond \
    -lSwGuid
    TARGET_EXT = .swdld
} 
CONFIG(release, debug|release) {
    win32:DESTDIR = ./../../lib/vc/release
    win32:DLLDESTDIR=  ./../../bin/vc/release
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
    LIBS += -L"../../lib/vc/release/" \
 <#else>
    LIBS += -L"${sw_dirs_lib}/release" \
</#if>
    -lSwCore \
    -lSwExecution \
    -lSwGui
    TARGET_EXT = .swdl
}

