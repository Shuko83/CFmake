#include "SwServiceMainWindowPluginFactory_Class.h"
#include "SwApplication.h"
#include "SwMacros.h"

#include <QRegularExpression>

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
SwServiceMainWindowPluginFactory_Class::SwServiceMainWindowPluginFactory_Class() :SwPluginFactory_Class()
{

}

//----------------------------------------------------------------------------------------------
SwServiceMainWindowPluginFactory_Class::~SwServiceMainWindowPluginFactory_Class()
{
}

//----------------------------------------------------------------------------------------------
void SwServiceMainWindowPluginFactory_Class::Initialize()
{
}

//----------------------------------------------------------------------------------------------
void SwServiceMainWindowPluginFactory_Class::Liberate()
{

}

///----------------------------------------------------------------------------------------------
SwComponent_Class * SwServiceMainWindowPluginFactory_Class::CreateInstanceOf(QString name)
{
	return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * SwServiceMainWindowPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id)
{
	return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon SwServiceMainWindowPluginFactory_Class::CreateIconOf(QString name) const
{
	QIcon ico;
	return ico;
}

//----------------------------------------------------------------------------------------------
QString SwServiceMainWindowPluginFactory_Class::GetPluginVersion()
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
double SwServiceMainWindowPluginFactory_Class::GetPluginCompilationDate()
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
SwPluginFactory_Class *  GetPluginInterface()
{
	static SwServiceMainWindowPluginFactory_Class singleton;
	return &singleton;
}
