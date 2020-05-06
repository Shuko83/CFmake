#include "_SwServiceConfigurationPluginFactory_Class.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "SwConfPushButton.h"
#include "info_SwServiceConfiguration.h"

#include <QRegularExpression>

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
_SwServiceConfigurationPluginFactory_Class::_SwServiceConfigurationPluginFactory_Class() :SwPluginFactory_Class()
{

}

//----------------------------------------------------------------------------------------------
_SwServiceConfigurationPluginFactory_Class::~_SwServiceConfigurationPluginFactory_Class()
{
	//SW_APP->UnregisterService(_serviceconfiguration->GetServiceName());
	delete _serviceconfiguration;
}

//----------------------------------------------------------------------------------------------
void _SwServiceConfigurationPluginFactory_Class::Initialize()
{
	_serviceconfiguration = new SwServiceSaveConfiguration();
	SW_APP->RegisterService(_serviceconfiguration);

    RegisterComponent("SwConfPushButton", "Permet d'instancier un PushButton pour declencher la sauvegarde de la conf");
}

//----------------------------------------------------------------------------------------------
void _SwServiceConfigurationPluginFactory_Class::Liberate()
{

}

///----------------------------------------------------------------------------------------------
SwComponent_Class * _SwServiceConfigurationPluginFactory_Class::CreateInstanceOf(QString name)
{
    SW_PUBLISH_COMPONENT("SwConfPushButton",SwConfPushButton)
	return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * _SwServiceConfigurationPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id)
{
	return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon _SwServiceConfigurationPluginFactory_Class::CreateIconOf(QString name) const
{
	QIcon ico;
	return ico;
}

//----------------------------------------------------------------------------------------------
QString _SwServiceConfigurationPluginFactory_Class::GetPluginVersion()
{
	QString build;
	build = QString("%1 %2").arg(__DATE__).arg(__TIME__);
	build = build.toUpper();
	build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
	build += " (Debug)";
#else 
	build+=" (Release)";
#endif
	return build;
}

//----------------------------------------------------------------------------------------------
double _SwServiceConfigurationPluginFactory_Class::GetPluginCompilationDate()
{
	//recup de la date à partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
	//recup de l'heure à partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime, "hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return (finalDateTime.toMSecsSinceEpoch() / 1000.0);//retourne la date en secondes depuis le 1er janvier 1970
}

//----------------------------------------------------------------------------------------------
QString _SwServiceConfigurationPluginFactory_Class::GetPluginName() const
{
	return SwServiceConfiguration::name();
}

//----------------------------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwServiceConfigurationPluginFactory_Class;
}
