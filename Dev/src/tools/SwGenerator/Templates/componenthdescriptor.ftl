<#include "cpp_macros.ftl">
<#-- component header description -->
<@hheader name_of_file="${component.@Name}" name_of_author="${doc.SwFactoryEntry.@Author}" description="${component.@Description}"/>
<#assign is_executable="${component.@Executable}">
/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
<#if is_executable="true">
#include <ExecutableComponent.h>
<#else>
#include <Component.h>
</#if> 
#include <SwPin.h>
#include <ISwProperty.h>

<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
<#if interface_def["boolean(@Type)"]><#assign interface_type="${interface_def.@Type}"><#else><#assign interface_type="void"></#if>
#include <${interface_type}.h>
</#if>
</#if>
</#list>


//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
<#if is_executable="true">
using namespace StreamWork::SwExecution;
</#if> 

/**
 *	@class ${component.@Name}
 *	@brief ${component.@Description}
 */ 
<#if is_executable="true">
class ${component.@Name} : public ExecutableComponent
<#else>
class ${component.@Name} : public Component
</#if> 
{
protected:


 	//--------------------------------------------------------------
	//Properties
	//--------------------------------------------------------------
<#list component?children as property>
<#if property?node_type = 'element'>
<#if property?node_name = "Properties">
    /** @brief Property value ${property.@PropertyName} */
<#-- Note pour info (le if suivant teste l'existence d'un attribut) -->
<#if property["boolean(@Type)"]><#assign property_type="${property.@Type}"><#else><#assign property_type="String"></#if>
<#switch property_type>
	<#case "Integer">
	int _${property.@PropertyName};
	<#break>
	<#case "UnsignedInteger">
	uint _${property.@PropertyName};
	<#break>
	<#case "Boolean">
	bool _${property.@PropertyName};
	<#break>
	<#case "Double">
	double _${property.@PropertyName};
	<#break>
	<#case "SwEnum">
	SwEnum _${property.@PropertyName};
	<#break>
	<#default>
	QString _${property.@PropertyName};
</#switch>
    /* Property ${property.@PropertyName} */
    ISwProperty * _property_${property.@PropertyName};
</#if>
</#if>
</#list>

 	//--------------------------------------------------------------
	//Handle interfaces
	//--------------------------------------------------------------
<#list component?children as interface_def>
<#if interface_def?node_type = 'element'>
<#if interface_def?node_name = "Interfaces">
    /** @brief Interface ${interface_def.@InterfaceName}*/
<#if interface_def["boolean(@Type)"]><#assign interface_type="${interface_def.@Type}"><#else><#assign interface_type="void"></#if>
	${interface_type} * _i_${interface_def.@InterfaceName};
</#if>
</#if>
</#list>

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------
<#list component?children as pin_def>
<#if pin_def?node_type = 'element'>
<#if pin_def?node_name = "Connectors">
    /* Pin ${pin_def.@ConnectorName}*/
	SwPin * _pin_${pin_def.@ConnectorName};
</#if>
</#if>
</#list>

public:
    /** @brief Constructeur */
    ${component.@Name}();
    /** @brief Destructeur */
    virtual ~${component.@Name}();
    /*! \brief Initialisation du composant*/
    virtual void initializeComponent() throw(SwException);
    /*! \brief evenement de changement de propriete*/
    virtual void eventPropertyChange(ISwProperty * property);
    /*! \brief evenement avant changement de la disponibilité de l'interface*/
    virtual void eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement apres changement de la disponibilité de l'interface*/
    virtual void eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host);
    /*! \brief evenement sur reception d'une data*/
    virtual void eventReceiveData(SwPin * src,SwData_Class * data);
    /*! \brief evenement sur changement d'activation*/
    virtual void eventActivationChanged();
<#if is_executable="true">   
    //----------------------------------------------------
    // Interface ISwExecutable_Service
    //----------------------------------------------------
    /*! \brief Initialisation */
    virtual void Initialize(double start_time,ISwExecution_Service * executor) throw (StreamWork::SwCore::SwException);            
    /*! \brief Demarrage */
    virtual void Start(double current_time) throw (StreamWork::SwCore::SwException);            
    /*! \brief Execution */
    virtual void Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException);            
    /*! \brief Execution */
    virtual void Stop(double current_time);            
</#if>    
    
};
<@hfooter/>