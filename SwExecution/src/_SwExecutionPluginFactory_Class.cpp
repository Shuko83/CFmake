/*!
 \file _SwExecutionPluginFactory_Class.cpp
 \brief Factory d'execution
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
 */

#include <QRegularExpression>
#include <QTime>
#include <QLocale>

#include "_SwExecutionPluginFactory_Class.h"
#include "_SwExecutor.h"
#include "_SwSwitchExecutor.h"
#include "info_SwExecution.h"

using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
_SwExecutionPluginFactory_Class::_SwExecutionPluginFactory_Class() :SwProtectedPluginFactory_Class()
{

}

//-----------------------------------------------------------------------
_SwExecutionPluginFactory_Class::~_SwExecutionPluginFactory_Class() 
{

}

//-----------------------------------------------------------------------
void _SwExecutionPluginFactory_Class::Initialize() 
{
	RegisterComponent("SwExecutor", "Executor component");
	RegisterComponent("SwSwitchExecutor", "SwitchExecutor component");
}

//-----------------------------------------------------------------------
void _SwExecutionPluginFactory_Class::Liberate() 
{

}

//-----------------------------------------------------------------------
SwComponent_Class * _SwExecutionPluginFactory_Class::CreateInstanceOf(QString name) 
{
	if (name == "SwExecutor") 
		return new _SwExecutor;

	else if (name == "SwSwitchExecutor")
		return new _SwSwitchExecutor;

	return NULL;
}

//-----------------------------------------------------------------------
SwData_Class * _SwExecutionPluginFactory_Class::CreateInstanceOf(unsigned int type_id) 
{
	return NULL;
}

//-----------------------------------------------------------------------
QIcon _SwExecutionPluginFactory_Class::CreateIconOf(QString name) const 
{
	QIcon ico;
	if (name == "SwExecutor") 
		return QIcon(":/SwExecution/executor.png");

	else if (name == "SwSwitchExecutor")
		return QIcon(":/SwExecution/switchExecutor.png");

	return ico;
}

//-----------------------------------------------------------------------
QString _SwExecutionPluginFactory_Class::GetPluginVersion()
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

//-----------------------------------------------------------------------
QDateTime _SwExecutionPluginFactory_Class::GetPluginCompilationDate()
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

	return finalDateTime;
}

//-----------------------------------------------------------------------
QString _SwExecutionPluginFactory_Class::GetPluginName() const
{
	return SwExecution::name();
}

//-----------------------------------------------------------------------
SwPluginFactory_Class *  GetPluginInterface()
{
	return new _SwExecutionPluginFactory_Class;
}
