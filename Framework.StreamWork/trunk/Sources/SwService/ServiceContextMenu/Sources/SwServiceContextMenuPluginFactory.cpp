#include "SwServiceContextMenuPluginFactory.h"
#include "SwApplication.h"
#include "SwMacros.h"

//
#include "ContextMenuEditor.h"

using namespace StreamWork::SwCore;

//----------------------------------------------------------------------------------------------
SwServiceContextMenuPluginFactory_Class::SwServiceContextMenuPluginFactory_Class() :SwPluginFactory_Class()
{
}

//----------------------------------------------------------------------------------------------
SwServiceContextMenuPluginFactory_Class::~SwServiceContextMenuPluginFactory_Class()
{
}

//----------------------------------------------------------------------------------------------
void SwServiceContextMenuPluginFactory_Class::Initialize()
{
	_contextMenuService = new StreamWork::Service::SwServiceContextMenu();
	SW_APP->RegisterService(_contextMenuService);
	RegisterComponent("SwContextMenuEditorComponent", "Permet la personnalisation du menu contextuel");
}

//----------------------------------------------------------------------------------------------
void SwServiceContextMenuPluginFactory_Class::Liberate()
{

}

///----------------------------------------------------------------------------------------------
SwComponent_Class * SwServiceContextMenuPluginFactory_Class::CreateInstanceOf(QString name)
{
	if (name == "SwContextMenuEditorComponent")	
		return new ContextMenuEditor();

	return NULL;
}

//----------------------------------------------------------------------------------------------
SwData_Class * SwServiceContextMenuPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id)
{
	return NULL;
}

//----------------------------------------------------------------------------------------------
QIcon SwServiceContextMenuPluginFactory_Class::CreateIconOf(QString name) const
{
	QIcon ico;
	return ico;
}

//----------------------------------------------------------------------------------------------
QString SwServiceContextMenuPluginFactory_Class::GetPluginVersion()
{
	QString build;
	build = QString("%1 %2").arg(__DATE__).arg(__TIME__);
	build = build.toUpper();
	build.replace(QRegExp("[ |:]+"), ".");
#ifndef QT_NO_DEBUG
	build += " (Debug)";
#else 
	build += " (Release)";
#endif
	return build;
}

//----------------------------------------------------------------------------------------------
double SwServiceContextMenuPluginFactory_Class::GetPluginCompilationDate()
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
