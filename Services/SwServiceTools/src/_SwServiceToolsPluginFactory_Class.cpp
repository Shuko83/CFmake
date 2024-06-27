#include "_SwServiceToolsPluginFactory_Class.h"

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"

#include "target_info.h"

#include <QDebug>
#include <QRegularExpression>

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
_SwServiceToolsPluginFactory_Class::_SwServiceToolsPluginFactory_Class():SwProtectedPluginFactory_Class()
{

}

//----------------------------------------------------------------------------------------------
_SwServiceToolsPluginFactory_Class::~_SwServiceToolsPluginFactory_Class() 
{
	delete _serviceRefProfiler;
	delete _serviceParameters;
	delete _serviceExtensions;
	delete _serviceCodeTimer;
	delete _serviceFileEditorManager;
}

//----------------------------------------------------------------------------------------------
void _SwServiceToolsPluginFactory_Class::Initialize() 
{
	auto app = SW_APP;	// La fonction GetInstance() est très chronophage
	_serviceRefProfiler = new _SwServiceRefProfiler();
	app->RegisterService(_serviceRefProfiler);
	_serviceParameters = new _SwServiceParametersImpl();
	app->RegisterService(_serviceParameters);
	_serviceExtensions = new _SwServiceExtensionsImpl();
	app->RegisterService(_serviceExtensions);
	_serviceCodeTimer = new _SwServiceCodeTimer();
	app->RegisterService(_serviceCodeTimer);
	_serviceFileEditorManager = new _SwFileEditorManager();
	app->RegisterService(_serviceFileEditorManager);

}

//----------------------------------------------------------------------------------------------
void _SwServiceToolsPluginFactory_Class::Liberate() 
{
    try
    {
        SW_APP->UnregisterService(_serviceFileEditorManager->GetServiceName());
        SW_APP->UnregisterService(_serviceCodeTimer->GetServiceName());
        SW_APP->UnregisterService(_serviceExtensions->GetServiceName());
        SW_APP->UnregisterService(_serviceParameters->GetServiceName());
        SW_APP->UnregisterService(_serviceRefProfiler->GetServiceName());
    }
    catch (SwException& e)
    {
        qCritical() << e.GetReason();
    }
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
    build.replace(QRegularExpression("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
    build+=" (Debug)";
#else 
    build+=" (Release)";
#endif
    return build;
}

//----------------------------------------------------------------------------------------------
QDateTime _SwServiceToolsPluginFactory_Class::GetPluginCompilationDate()
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

	return finalDateTime;
}

//----------------------------------------------------------------------------------------------
QString _SwServiceToolsPluginFactory_Class::GetPluginName() const
{
	return target_info::name();
}

//----------------------------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwServiceToolsPluginFactory_Class;
}
