<#-- factory body description -->
<#include "cpp_macros.ftl">
<#-- factory header description -->
<@cppheader name_of_file="${classename}" name_of_author="${doc.SwFactoryEntry.@Author}" description="Plugin Factory"/>

#include <QRegExp>

#include "${classename}.h"
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
#include "${component.@Name}.h"
</#if>
</#if>
</#list>

/** @brief Constructeur */
${classename}::${classename}():SwPluginFactory_Class() {

}
/** @brief Destructeur */
${classename}::~${classename}() {

}
/** @brief Initialisation */
void ${classename}::Initialize() {
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
	RegisterComponent("${component.@Name}","${component.@Description}");
</#if>
</#if>
</#list>
}
/** @brief Liberation */
void ${classename}::Liberate() {

}
/** @brief instanciation d'un composant */
SwComponent_Class * ${classename}::CreateInstanceOf(QString name) {
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
    if (name=="${component.@Name}") {
        return new ${component.@Name};
    }
</#if>
</#if>
</#list>
    return NULL;
}
/** @brief instanciation d'une data */
SwData_Class * ${classename}::CreateInstanceOf(const SwUUID & type_id) {
    return NULL;
}
/** 
 * @brief acces a l'icone d'un composant
 * @param[in] name nom du composant
 * @return l'incone ou icone vide si non trouvé
 */
QIcon ${classename}::CreateIconOf(QString name) const {
    QIcon ico;
<#list doc.SwFactoryEntry?children as component>
<#if component?node_type = 'element'>
<#if component?node_name = "Components">
    if (name=="${component.@Name}") {
        return ico; //change for personal icon
    }
</#if>
</#if>
</#list>
    return ico;
}
/** 
 * @brief Acces a la version du plugin
 * @return version
 */
QString ${classename}::GetPluginVersion(){
    QString build;

    build=QString("%1 %2").arg(__DATE__).arg(__TIME__);
    build=build.toUpper();
    build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}

