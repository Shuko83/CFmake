<#include "cpp_macros.ftl">
<#-- component header description -->
<@hheader name_of_file=doc.SwFactoryEntry.@Name+"Constantes" name_of_author="${doc.SwFactoryEntry.@Author}" description="Declaration de constantes"/>

/*
  * INCLUDES LOCAUX
  */

#include <Qt/qglobal.h>
//Export via les DLL
#include <SwMacros.h>


#ifdef ${doc.SwFactoryEntry.@Name}_LIB
#define ${doc.SwFactoryEntry.@Name}_EXPORT Q_DECL_EXPORT
#define ${doc.SwFactoryEntry.@Name}_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_EXPORT const StreamWork::SwCore::SwUUID name;
#else
#define ${doc.SwFactoryEntry.@Name}_EXPORT Q_DECL_IMPORT
#define ${doc.SwFactoryEntry.@Name}_DECLARE_UUID(name) SW_EXTERN_C Q_DECL_IMPORT const StreamWork::SwCore::SwUUID name;
#endif


<@hfooter/>