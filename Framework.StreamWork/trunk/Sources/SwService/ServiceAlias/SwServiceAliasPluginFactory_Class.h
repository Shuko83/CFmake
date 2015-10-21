#pragma once

/*
* INCLUDES LOCAUX
*/
#include "SwAliasManager.h"

/*
* INCLUDES GLOBAUX
*/
#include "SwPluginFactory_Class.h"

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @class	SwServiceAliasPluginFactory_Class	SwServiceAliasPluginFactory_Class.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\SwServiceAliasPluginFactory_Class.h"
	 * @brief	Classe instanciant le service d'alias pour Starlinx
	 *
	 * @sa	StreamWork::SwCore::SwPluginFactory_Class
	 **/
	class SwServiceAliasPluginFactory_Class : public StreamWork::SwCore::SwPluginFactory_Class
	{
	public:
		/**
		 * @fn		SwServiceAlias::SwServiceAliasPluginFactory_Class::SwServiceAliasPluginFactory_Class()
		 * @brief	Constructeur par défaut
		 **/
		SwServiceAliasPluginFactory_Class();

		/**
		 * @fn		SwServiceAlias::SwServiceAliasPluginFactory_Class::~SwServiceAliasPluginFactory_Class()
		 * @brief	Destructeur par défaut
		 **/
		~SwServiceAliasPluginFactory_Class();

		/**
		 * @fn		void SwServiceAlias::SwServiceAliasPluginFactory_Class::Initialize()
		 * @brief	Initialisation du service
		 *
		 * @sa	StreamWork::SwCore::SwPluginFactory_Class::Initialize()
		 **/
		void Initialize() override;

		/**
		 * @fn		void SwServiceAlias::SwServiceAliasPluginFactory_Class::Liberate()
		 * @brief	Libération du service
		 *
		 * @sa	StreamWork::SwCore::SwPluginFactory_Class::Liberate()
		 **/
		void Liberate() override;

		/**
		 * @fn		QString SwServiceAlias::SwServiceAliasPluginFactory_Class::GetPluginVersion()
		 * @brief	Retourner le numéro de version du service de gestion des alias
		 *
		 * @return	QString	Numéro de version du service
		 *
		 * @sa	StreamWork::SwCore::SwPluginFactory_Class::GetPluginVersion()
		 **/
		QString GetPluginVersion() override;

		/**
		 * @fn		double SwServiceAlias::SwServiceAliasPluginFactory_Class::GetPluginCompilationDate()
		 * @brief	Retourne la date de la dernière compilation du service de gestion des alias
		 *
		 * @return	double	Date de la dernière compilation du service
		 *
		 * @sa	StreamWork::SwCore::SwPluginFactory_Class::GetPluginCompilationDate()
		 **/
		double GetPluginCompilationDate() override;

	private:
		/**
		 * @var		_aliasService
		 * @brief	Pointeur vers le service d'alias
		 **/
		SwAliasManager *_aliasService;
	};
}  // namespace SwServiceAlias

#ifndef QT_NO_DEBUG
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterfaceD() {
	return new SwServiceAlias::SwServiceAliasPluginFactory_Class();
}

#else
extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface() {
	return new SwServiceAlias::SwServiceAliasPluginFactory_Class();
}
#endif