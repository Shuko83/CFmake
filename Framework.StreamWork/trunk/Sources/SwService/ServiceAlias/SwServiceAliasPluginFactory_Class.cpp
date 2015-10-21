/*
* INCLUDES LOCAUX
*/
#include "SwServiceAliasPluginFactory_Class.h"

namespace SwServiceAlias
{
	//---------------------------------------------------------------------------------
	SwServiceAliasPluginFactory_Class::SwServiceAliasPluginFactory_Class()
		: _aliasService(nullptr)
	{}

	//---------------------------------------------------------------------------------
	SwServiceAliasPluginFactory_Class::~SwServiceAliasPluginFactory_Class()
	{
		delete _aliasService;
	}

	//---------------------------------------------------------------------------------
	void SwServiceAliasPluginFactory_Class::Initialize()
	{
		_aliasService = new SwAliasManager();
		// Le service s'enregistre lui-même lorsqu'il est correctement initialisé (attente du service des alertes)
	}

	//---------------------------------------------------------------------------------
	void SwServiceAliasPluginFactory_Class::Liberate()
	{
		SW_APP->UnregisterService(_aliasService->GetServiceName());
	}

	//---------------------------------------------------------------------------------
	QString SwServiceAliasPluginFactory_Class::GetPluginVersion()
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

	//---------------------------------------------------------------------------------
	double SwServiceAliasPluginFactory_Class::GetPluginCompilationDate()
	{
		// recup de la date à partir de __DATE__
		QDate date = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
		// recup de l'heure à partir de __TIME__
		QString strTime = __TIME__;
		QTime time = time.fromString(strTime, "hh:mm:ss");

		QDateTime finalDateTime;
		// set la date dans QDateTime
		finalDateTime.setDate(date);
		// set le time dans QDateTime
		finalDateTime.setTime(time);

		return (finalDateTime.toMSecsSinceEpoch() / 1000.0);  // retourne la date en secondes depuis le 1er janvier 1970
	}
}  // namespace SwServiceAlias