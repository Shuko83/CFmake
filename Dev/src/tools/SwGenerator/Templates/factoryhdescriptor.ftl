<#include "cpp_macros.ftl">
<#-- factory header description -->
<@hheader name_of_file="${classename}" name_of_author="${doc.SwFactoryEntry.@Author}" description="Plugin Factory"/>

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
#include "${project_name}Constantes.h"

/*
  * INCLUDES LOCAUX
  */
#include <SwPluginFactory_Class.h>

using namespace StreamWork::SwCore;

/**
 *	@class ${classename}
 *	@brief classe definissant un plugin usine
 */
class ${classename} : public SwPluginFactory_Class
{
protected:
    /** @brief instanciation d'un composant */
    SwComponent_Class * CreateInstanceOf(QString name) ;
    /** @brief acces a l'icone d'un composant */
    QIcon CreateIconOf(QString name) const;
    /** @brief instanciation d'une data */
    SwData_Class * CreateInstanceOf(const SwUUID & type_id);
public:
    /** @brief Constructeur */
    ${classename}();
    /** @brief Destructeur */
    virtual ~${classename}();
    /** @brief Initialisation */
    void Initialize();
    /** @brief Liberation */
    void Liberate();
    /** @brief Acces a la version du plugin */
    QString GetPluginVersion();
};

#ifndef QT_NO_DEBUG
    extern "C" ${project_name}_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
        return new ${classename}();
    }
#else 
    extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
        return new ${classename}();    
    }
#endif



<@hfooter/>