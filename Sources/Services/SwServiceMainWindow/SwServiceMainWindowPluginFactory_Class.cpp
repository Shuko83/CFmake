#include "SwServiceMainWindowPluginFactory_Class.h"
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "info_SwServiceMainWindow.h"

#include <QRegularExpression>

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
SwServiceMainWindowPluginFactory_Class::SwServiceMainWindowPluginFactory_Class() :SwProtectedPluginFactory_Class()
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
QDateTime SwServiceMainWindowPluginFactory_Class::GetPluginCompilationDate()
{
	//recup de la date ? partir de __DATE__
	QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));

	//recup de l'heure ? partir de __TIME__
	QString strTime = __TIME__;
	QTime time = time.fromString(strTime, "hh:mm:ss");

	QDateTime finalDateTime;
	//set la date dans QDateTime
	finalDateTime.setDate(date);
	//set le time dans QDateTime
	finalDateTime.setTime(time);

	return finalDateTime;
}

//----------------------------------------------------------------------------------------------
QString SwServiceMainWindowPluginFactory_Class::GetPluginName() const
{
	return SwServiceMainWindow::name();
}

//----------------------------------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new SwServiceMainWindowPluginFactory_Class;
}
