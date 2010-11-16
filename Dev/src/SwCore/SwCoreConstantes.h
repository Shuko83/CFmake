/*!
\file SwCoreConstantes.h
\date 28/04/2006
\brief Constantes de SwCore
\author  Big
\version 1.0
 */

#ifndef _SwCoreConstantes_H
#define _SwCoreConstantes_H

#include <QGlobal.h>
//Export via les DLL

#ifdef SWCORE_LIB
# define BUILD_SWCORE Q_DECL_EXPORT
#else
# define BUILD_SWCORE Q_DECL_IMPORT
#endif

#ifdef Q_CC_MSVC
#pragma warning(disable:4290)
#pragma warning(disable:4100)
#endif

//Version
#define CG_STREAMWORK_VERSION "3.0.2"

//PATH Separator
#define CG_PATH_SEPARATOR '.'
//PATH StreamHeader
#define CG_PATH_STREAM_HEADER '$'

//XML
#define CG_SW_XML_DOCUMENT_NODE    "streamwork"
#define CG_SW_XML_GROUP_DOCUMENT_NODE    "streamwork_group"
#define CG_SW_XML_DOCUMENT_NODE_ATT_VERSION    "version"
#define CG_SW_XML_DOCUMENT_NODE_ATT_DATE    "build_date"
#define CG_SW_XML_DOCUMENT_NODE_ATT_TIME    "build_time"

#define CG_SW_XML_PATH_NODE    "path"
#define CG_SW_XML_PATH_NODE_ATT_VALUE    "path_value"

#define CG_SW_XML_COMPONENT_NODE    "component"
#define CG_SW_XML_COMPONENT_NODE_ATT_NAME    "name"
#define CG_SW_XML_COMPONENT_NODE_ATT_ACTIVE    "active"
#define CG_SW_XML_COMPONENT_NODE_ATT_DESC    "description"
#define CG_SW_XML_COMPONENT_NODE_ATT_FACTORY_NAME    "factory_name"
#define CG_SW_XML_COMPONENT_NODE_ATT_FACTORY    "factory"

#define CG_SW_XML_SERVICE_NODE    "service"
#define CG_SW_XML_SERVICE_ATT_NAME    "name"


#endif

