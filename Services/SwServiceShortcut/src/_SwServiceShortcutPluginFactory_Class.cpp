#include "_SwServiceShortcutPluginFactory_Class.h"
#include "target_info.h"
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"

#include <QRegularExpression>

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
_SwServiceShortcutPluginFactory_Class::_SwServiceShortcutPluginFactory_Class()
	: SwProtectedPluginFactory_Class()
	, _serviceShortcuts(nullptr)
{

}

//----------------------------------------------------------------------------------------------
_SwServiceShortcutPluginFactory_Class::~_SwServiceShortcutPluginFactory_Class() 
{
	delete _serviceShortcuts;
}

//----------------------------------------------------------------------------------------------
void _SwServiceShortcutPluginFactory_Class::Initialize() 
{
	_serviceShortcuts = new _SwServiceShortcuts();
	SW_APP->RegisterService(_serviceShortcuts);
}

//----------------------------------------------------------------------------------------------
void _SwServiceShortcutPluginFactory_Class::Liberate() 
{
    try
    {
        SW_APP->UnregisterService(_serviceShortcuts->GetServiceName());
    }
    catch (SwException& e)
    {
        qCritical() << e.GetReason();
    }
}

///----------------------------------------------------------------------------------------------
SwComponent_Class * _SwServiceShortcutPluginFactory_Class::CreateInstanceOf(QString name) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * _SwServiceShortcutPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) 
{
    return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon _SwServiceShortcutPluginFactory_Class::CreateIconOf(QString name) const 
{
    QIcon ico;
    return ico;
}

//----------------------------------------------------------------------------------------------
QString _SwServiceShortcutPluginFactory_Class::GetPluginVersion()
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
QDateTime _SwServiceShortcutPluginFactory_Class::GetPluginCompilationDate()
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
QString _SwServiceShortcutPluginFactory_Class::GetPluginName() const
{
	return target_info::name();
}

//----------------------------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwServiceShortcutPluginFactory_Class;
}
