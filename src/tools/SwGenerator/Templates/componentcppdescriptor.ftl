<#-- factory body description -->
<#include "cpp_macros.ftl">
<#-- factory header description -->
<@cppheader name_of_file="${component.@Name}" name_of_author="${doc.SwFactoryEntry.@Author}" description="${component.@Description}"/>
<#assign is_executable="${component.@Executable}">

#include <SwApplication.h>
#include <SwMacros.h>
#include "${component.@Name}.h"


/** @brief Constructeur */
<#if is_executable="true">   
${component.@Name}::${component.@Name}():ExecutableComponent() {
<#else>
${component.@Name}::${component.@Name}():Component() {
</#if>


	//RESET Interfaces
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
    _i_${interface_def.@InterfaceName}=NULL; /* Remplacer null par le bon handle pour les interfaces fournies */
</#if>
</#if>
</#list>

	//RESET Pins
<#list component?children as pin_def>
<#if pin_def?node_type = 'element'>
<#if pin_def?node_name = "Connectors">
    _pin_${pin_def.@ConnectorName}=NULL;
</#if>
</#if>
</#list>

	//RESET Properties
<#list component?children as property>
<#if property?node_type = 'element'>
<#if property?node_name = "Properties">
<#if property["boolean(@Type)"]><#assign property_type="${property.@Type}"><#else><#assign property_type="String"></#if>
<#switch property_type>
	<#case "Integer">
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}=${property.@DefaultValue};
<#else>
	_${property.@PropertyName}=0;
</#if>
	<#break>
	<#case "UnsignedInteger">
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}=${property.@DefaultValue};
<#else>
	_${property.@PropertyName}=0;
</#if>
	<#break>
	<#case "Boolean">
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}=${property.@DefaultValue};
<#else>
	_${property.@PropertyName}=false;
</#if>
	<#break>
	<#case "Double">
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}=${property.@DefaultValue};
<#else>
	_${property.@PropertyName}=0.0;
</#if>
	<#break>
	<#case "SwEnum">
	//Ajouter vos clefs ici de cette maniere _${property.@PropertyName}.AddKey(index,"valeur chaine");
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}.FromInt(${property.@DefaultValue});
<#else>
	_${property.@PropertyName}.FromInt(0);
</#if>
	<#break>
	<#default>
<#if property["boolean(@DefaultValue)"]>
	_${property.@PropertyName}="${property.@DefaultValue}";
<#else>
	_${property.@PropertyName}="";
</#if>
</#switch>
	_property_${property.@PropertyName}=NULL;
</#if>
</#if>
</#list>
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE

}
/** @brief Destructeur */
${component.@Name}::~${component.@Name}() {

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins
<#list component?children as pin_def>
<#if pin_def?node_type = 'element'>
<#if pin_def?node_name = "Connectors">
    _pins_service->UnregisterPin(_pin_${pin_def.@ConnectorName});
    delete _pin_${pin_def.@ConnectorName};
</#if>
</#if>
</#list>

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
}
/*! \brief Initialisation du composant*/
void ${component.@Name}::initializeComponent() throw(SwException) {
    QVariant tmp;
    
    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
<#if interface_def["boolean(@IsProvider)"]><#assign is_provider="${interface_def.@IsProvider}"><#else><#assign is_provider="false"></#if>
<#if is_provider="yes">
<#if interface_def["boolean(@Type)"]><#assign interface_type="${interface_def.@Type}"><#else><#assign interface_type="void"></#if>
    getIProviderService().RegisterProvidedInterface<${interface_type}>("${interface_def.@InterfaceName}",_i_${interface_def.@InterfaceName});
</#if>
</#if>
</#if>
</#list>

    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
<#if interface_def["boolean(@IsProvider)"]><#assign is_provider="${interface_def.@IsProvider}"><#else><#assign is_provider="false"></#if>
<#if is_provider="no">
<#if interface_def["boolean(@Type)"]><#assign interface_type="${interface_def.@Type}"><#else><#assign interface_type="void"></#if>
    getIConsumerService().RegisterConsumedInterface<${interface_type}>("${interface_def.@InterfaceName}",&_i_${interface_def.@InterfaceName});
</#if>
</#if>
</#if>
</#list>

    //--------------------------------------
    //Definition Pins
    //--------------------------------------
<#list component?children as pin_def>
<#if pin_def?node_type = 'element'>
<#if pin_def?node_name = "Connectors">
<#if pin_def["boolean(@Type)"]><#assign pin_type="${pin_def.@Type}"><#else><#assign pin_type="none"></#if>
    _pin_${pin_def.@ConnectorName}=new SwPin(_pins_service,"${pin_def.@ConnectorName}","${pin_type}");
    getPinsService().RegisterPin(_pin_${pin_def.@ConnectorName});
    enableListeningForPin(_pin_${pin_def.@ConnectorName};
</#if>
</#if>
</#list>

    //--------------------------------------
    //Definition Properties
    //--------------------------------------
<#list component?children as property>
<#if property?node_type = 'element'>
<#if property?node_name = "Properties">
<#if property["boolean(@Type)"]><#assign property_type="${property.@Type}"><#else><#assign property_type="String"></#if>
<#switch property_type>
    <#case "Integer">
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<int>("${property.@PropertyName}");
    <#break>
    <#case "UnsignedInteger">
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<uint>("${property.@PropertyName}");
    <#break>
    <#case "Boolean">
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<bool>("${property.@PropertyName}");
    <#break>
    <#case "Double">
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<double>("${property.@PropertyName}");
    <#break>
    <#case "SwEnum">
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<SwEnum>("${property.@PropertyName}");
    <#break>
    <#default>
    _property_${property.@PropertyName}=getPropertiesService().CreateProperty<QString>("${property.@PropertyName}");
</#switch>
    if (_property_${property.@PropertyName}==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register ${property.@PropertyName} property\n"));
    }
<#if property_type="SwEnum">
    tmp.setValue(_${property.@PropertyName});
    _property_${property.@PropertyName}->SetValue(QVariant(tmp));
<#else>
    _property_${property.@PropertyName}->SetValue(QVariant(_${property.@PropertyName}));
</#if>
    enableListeningChangeForProperty(_property_${property.@PropertyName});
</#if>
</#if>
</#list>
    //--------------------------------------
    //Autres
    //--------------------------------------

}
/*! \brief evenement de changement de propriete*/
void ${component.@Name}::eventPropertyChange(ISwProperty * property){
<#list component?children as property>
<#if property?node_type = 'element'>
<#if property?node_name = "Properties">
<#if property["boolean(@Type)"]><#assign property_type="${property.@Type}"><#else><#assign property_type="String"></#if>
    //Test si c'est la propriete ${property.@PropertyName} qui a changé
    if (property==_property_${property.@PropertyName}) {
<#switch property_type>
    <#case "Integer">
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<int>();
    <#break>
    <#case "UnsignedInteger">
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<uint>();
    <#break>
    <#case "Boolean">
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<bool>();
    <#break>
    <#case "Double">
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<double>();
    <#break>
    <#case "SwEnum">
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<SwEnum>();
    <#break>
    <#default>
    _${property.@PropertyName}=_property_${property.@PropertyName}->GetValue().value<QString>();
</#switch>
    }
</#if>
</#if>
</#list>
}
/*! \brief evenement avant changement de la disponibilité de l'interface*/
void ${component.@Name}::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
<#if interface_def["boolean(@IsProvider)"]><#assign is_provider="${interface_def.@IsProvider}"><#else><#assign is_provider="false"></#if>
<#if is_provider="false">
    if (interface_name==QString("${interface_def.@InterfaceName}")) {
        if (_i_${interface_def.@InterfaceName}!=NULL) {
            //Traiter le fait que l'interface ${interface_def.@InterfaceName} ne va plus être disponible
            //_i_${interface_def.@InterfaceName} va devenir NULL
        }
    }
</#if>
</#if>
</#if>
</#list>
}
/*! \brief evenement apres changement de la disponibilité de l'interface*/
void ${component.@Name}::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host){
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
<#if interface_def["boolean(@IsProvider)"]><#assign is_provider="${interface_def.@IsProvider}"><#else><#assign is_provider="false"></#if>
<#if is_provider="false">
    if (interface_name==QString("${interface_def.@InterfaceName}")) {
        if (_i_${interface_def.@InterfaceName}!=NULL) {
            //Traiter le fait que l'interface ${interface_def.@InterfaceName} est maintenant disponible
        }
    }
</#if>
</#if>
</#if>
</#list>
}
/*! \brief evenement sur reception d'une data
 * @warning
 *  - Si vous gardez une reference sur la donnée reçues au dela de la portée de la methode suivante
 *utiliser un SwRefPtr sur la donnée
 *  - Si vous souhaitez modifier une donnée recue il faut d'abord en faire une copy
*/
void ${component.@Name}::eventReceiveData(SwPin * src,SwData_Class * data){
<#list component?children as pin_def>
<#if pin_def?node_type = 'element'>
<#if pin_def?node_name = "Connectors">
<#if pin_def["boolean(@Type)"]><#assign pin_type="${pin_def.@Type}"><#else><#assign pin_type="none"></#if>
    //Reception d'une donnee sur le pin <c:get select="$pin/@ConnectorName" default="unnamed_pin"/>
    if (src==_pin_${pin_def.@ConnectorName}) {
        //Check si le type est valide
        if (data->GetType()==QString("${pin_type}")) {
            //Traiter la donnee
            //A FAIRE
        }
    }
</#if>
</#if>
</#list>
}
/*! \brief evenement sur changement d'activation*/
void ${component.@Name}::eventActivationChanged(){
    //TO DO
}
 <#if is_executable="true">   
/*! \brief Initialisation */
void ${component.@Name}::Initialize(double start_time,ISwExecution_Service * executor) throw (StreamWork::SwCore::SwException) {

}            
/*! \brief Demarrage */
void ${component.@Name}::Start(double current_time) throw (StreamWork::SwCore::SwException) {

}            
/*! \brief Execution */
void ${component.@Name}::Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException) {

}            
/*! \brief Execution */
void ${component.@Name}::Stop(double current_time) {

}            
</#if>    


