#include "_SwServiceToolsPluginFactory_Class.h"

#include "SwApplication.h"


using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
_SwServiceToolsPluginFactory_Class::_SwServiceToolsPluginFactory_Class():SwPluginFactory_Class() 
{

}

//----------------------------------------------------------------------------------------------
_SwServiceToolsPluginFactory_Class::~_SwServiceToolsPluginFactory_Class() 
{
SW_APP->UnregisterService(_serviceRefProfiler->GetServiceName());
	delete _serviceRefProfiler;
SW_APP->UnregisterService(_serviceParameters->GetServiceName());
	delete _serviceParameters;
SW_APP->UnregisterService(_serviceExtensions->GetServiceName());
	delete _serviceExtensions;
SW_APP->UnregisterService(_serviceCodeTimer->GetServiceName());
	delete _serviceCodeTimer; 
SW_APP->UnregisterService(_serviceFileEditorManager->GetServiceName());
	delete _serviceFileEditorManager;

}

//----------------------------------------------------------------------------------------------
void _SwServiceToolsPluginFactory_Class::Initialize() 
{
	_serviceRefProfiler = new _SwServiceRefProfiler();
	SW_APP->RegisterService(_serviceRefProfiler);
	_serviceParameters = new _SwServiceParametersImpl();
	SW_APP->RegisterService(_serviceParameters);
	_serviceExtensions = new _SwServiceExtensionsImpl();
	SW_APP->RegisterService(_serviceExtensions);
	_serviceCodeTimer = new _SwServiceCodeTimer();
	SW_APP->RegisterService(_serviceCodeTimer);
	_serviceFileEditorManager = new _SwFileEditorManager();
	SW_APP->RegisterService(_serviceFileEditorManager);

}

//----------------------------------------------------------------------------------------------
void _SwServiceToolsPluginFactory_Class::Liberate() 
{

}

///----------------------------------------------------------------------------------------------
SwComponent_Class * _SwServiceToolsPluginFactory_Class::CreateInstanceOf(QString name) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * _SwServiceToolsPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon _SwServiceToolsPluginFactory_Class::CreateIconOf(QString name) const 
{
    QIcon ico;
    return ico;
}

//----------------------------------------------------------------------------------------------
QString _SwServiceToolsPluginFactory_Class::GetPluginVersion()
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
double _SwServiceToolsPluginFactory_Class::GetPluginCompilationDate()
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
