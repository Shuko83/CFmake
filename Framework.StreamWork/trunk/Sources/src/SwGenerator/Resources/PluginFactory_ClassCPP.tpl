/**
 * @file {{PLUGIN_NAME}}PluginFactory_Class.cpp
 * @brief Plugin Factory
 * @version 1.0
 * @date {{DATE}}
 * @author {{AUTHOR}}
 */


#include <QRegExp>
#include <QDateTime>
#include "{{PLUGIN_NAME}}PluginFactory_Class.h"
{{#SECTION_INCLUDE_CLASS}}#include "{{INCLUDE_FILE}}"
{{/SECTION_INCLUDE_CLASS}}

/**
 * Include Composant
 */
//Here ! 
 
 
//-------------------------------------------------------------------------
{{PLUGIN_NAME}}PluginFactory_Class::{{PLUGIN_NAME}}PluginFactory_Class():SwPluginFactory_Class() 
{

}

//-------------------------------------------------------------------------
{{PLUGIN_NAME}}PluginFactory_Class::~{{PLUGIN_NAME}}PluginFactory_Class() 
{

}

//-------------------------------------------------------------------------
void {{PLUGIN_NAME}}PluginFactory_Class::Initialize() 
{
	/*
		RegisterComponent("NomDeMonComposant","Details du composant");
	*/
	{{#FOR_REGISTER_COMPONENT}}{{CONTENT}}{{/FOR_REGISTER_COMPONENT}}
}

//-------------------------------------------------------------------------
void {{PLUGIN_NAME}}PluginFactory_Class::Liberate() 
{

}

//-------------------------------------------------------------------------
SwComponent_Class * {{PLUGIN_NAME}}PluginFactory_Class::CreateInstanceOf(QString name) 
{
    /*
	if (name=="NomDeMonComposant") 
	{
        return new sx::carto::cartoConfiguration::SxCartoConfigurationWidget;
    }  
	*/
	{{#FOR_CREATEINSTANCEOF}}{{CONTENT}}{{/FOR_CREATEINSTANCEOF}}
	
  
   return NULL;
}

//-------------------------------------------------------------------------
SwData_Class * {{PLUGIN_NAME}}PluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) 
{
    return NULL;
}

//-------------------------------------------------------------------------
QIcon {{PLUGIN_NAME}}PluginFactory_Class::CreateIconOf(QString name) const 
{
    QIcon ico;
	
    /*
	if (name=="NomDeMonComposant") 
	{
        return QIcon(":/PathToImage.png");
    } 
	*/ 
	
	{{#FOR_CREATEICONOF}}{{CONTENT}}{{/FOR_CREATEICONOF}}
	
    return ico;
}

//-------------------------------------------------------------------------
QString {{PLUGIN_NAME}}PluginFactory_Class::GetPluginVersion()
{
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

//---------------------------------------------------------------------------------
double {{PLUGIN_NAME}}PluginFactory_Class::GetPluginCompilationDate()
{
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
	
	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime,"hh:mm:ss");
	
	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);
	
	return (finalDateTime.toMSecsSinceEpoch() / 1000.0);//retourne la date en secondes depuis le 1er janvier 1970
}

