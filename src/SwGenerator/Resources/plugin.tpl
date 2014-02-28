
################################################################################
# PROJECT
################################################################################
PROJECTS_NAME ={{PLUGIN_NAME}}
PROJECTS_PATH = ../../../../../../..   
STARLINX_PATH = ../../../../..
DEV_PATH = ../../../..
PROJECTS_TYPE = lib
SUB_DIR = /plugins 

################################################################################
# CONSTANTES
################################################################################
LIB_CSTE = $$PROJECTS_NAME
LIB_CSTE = $$join(LIB_CSTE,,,_LIB)
ADDITIONNAL_DEFINES = $$LIB_CSTE QT_DLL QT_XML_LIB

################################################################################
# SPECIFIC INCLUDES
################################################################################

ADDITIONNAL_INCLUDES += \
	{{#FOR_ADDITIONNAL_INCLUDES}}{{CONTENT}}{{/FOR_ADDITIONNAL_INCLUDES}}
    
    
################################################################################
# SPECIFIC LIBS
################################################################################    

ADDITIONNAL_LIBS = \
	{{#FOR_ADDITIONNAL_LIBS}}{{CONTENT}}{{/FOR_ADDITIONNAL_LIBS}}
        
        
ADDITIONNAL_LIBSD =  \
	{{#FOR_ADDITIONNAL_LIBSD}}{{CONTENT}}{{/FOR_ADDITIONNAL_LIBSD}}


ADDITIONNAL_LIBS_PATH = \
	{{#FOR_ADDITIONNAL_LIBS_PATH}}{{CONTENT}}{{/FOR_ADDITIONNAL_LIBS_PATH}}
    
    
ADDITIONNAL_LIBSD_PATH = \
	{{#FOR_ADDITIONNAL_LIBSD_PATH}}{{CONTENT}}{{/FOR_ADDITIONNAL_LIBSD_PATH}}


################################################################################
# DO NOT MODIFY BEYOND THIS
################################################################################    
#Include file(s)
COMMONPRJFILE = $$DEV_PATH/plugin_common.pri
include($$COMMONPRJFILE)
include(plugin_files.pri)