#include "_SwServiceAlertPluginFactory_Class.h"

#include "SwApplication.h"
#include "SwMacros.h"

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
_SwServiceAlertPluginFactory_Class::_SwServiceAlertPluginFactory_Class():SwPluginFactory_Class() 
{

}

//----------------------------------------------------------------------------------------------
_SwServiceAlertPluginFactory_Class::~_SwServiceAlertPluginFactory_Class() 
{
	//SW_APP->UnregisterService(_serviceAlert->GetServiceName());
	delete _serviceAlert;
}

//----------------------------------------------------------------------------------------------
void _SwServiceAlertPluginFactory_Class::Initialize() 
{
	_serviceAlert = new _SwServiceAlert();
    SW_APP->RegisterService(_serviceAlert);
}

//----------------------------------------------------------------------------------------------
void _SwServiceAlertPluginFactory_Class::Liberate() 
{

}

///----------------------------------------------------------------------------------------------
SwComponent_Class * _SwServiceAlertPluginFactory_Class::CreateInstanceOf(QString name) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * _SwServiceAlertPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon _SwServiceAlertPluginFactory_Class::CreateIconOf(QString name) const 
{
    QIcon ico;
    return ico;
}

//----------------------------------------------------------------------------------------------
QString _SwServiceAlertPluginFactory_Class::GetPluginVersion()
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

//----------------------------------------------------------------------------------------------
double _SwServiceAlertPluginFactory_Class::GetPluginCompilationDate()
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
