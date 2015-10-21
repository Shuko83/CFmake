#pragma once

/*
* INCLUDES LOCAUX
*/
#include "ISwAliasUser.h"
#include "ISwAliasSaver.h"
#include "ISwAliasEditor.h"

/*
* INCLUDES GLOBAUX
*/
#include "ISwService.h"

namespace SwServiceAlias
{
	/**
	 * @def		I_SX_ALIAS_MANAGER
	 * @brief	Définition du nom du service de gestion des alias
	 **/
#define I_SX_ALIAS_MANAGER "ISwAliasManager"

	/**
	 * @class	ISwAliasManager	ISwAliasManager.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\Interfaces\ISwAliasManager.h"
	 * @brief	Interface générale du service de gestion des alias
	 *
	 * @sa	StreamWork::SwCore::ISwService
	 * @sa	SwServiceAlias::ISwAliasUser
	 * @sa	SwServiceAlias::ISwAliasSaver
	 * @sa	SwServiceAlias::ISwAliasEditor
	 **/
	class ISwAliasManager : public StreamWork::SwCore::ISwService,
		public ISwAliasUser,
		public ISwAliasSaver,
		public ISwAliasEditor
	{
	public:
		/**
		 * @fn		virtual SwServiceAlias::ISwAliasManager::~ISwAliasManager()
		 * @brief	Destructeur par défaut
		 **/
		virtual ~ISwAliasManager()
		{}

		/**
		 * @fn		void SwServiceAlias::SxAliasManager::Liberate()
		 * @brief	Est appelé uniquement par le service manager auprès duquel le service est enregistré
		 *			lorsque ce premier se détruit ou une opération de dé-enregistrement du service est réalisée
		 *
		 * @sa	StreamWork::SwCore::ISwService::Liberate()
		 **/
		void Liberate() override
		{}

		/**
		 * @fn		QString SwServiceAlias::SxAliasManager::GetServiceName()
		 * @brief	Renvoyer le nom du service
		 *
		 * @return	QString	Le nom du service
		 *
		 * @sa	StreamWork::SwCore::ISwService::GetServiceName()
		 **/
		QString GetServiceName() override
		{
			return QString(I_SX_ALIAS_MANAGER);
		}

		/**
		 * @fn		QString SwServiceAlias::SxAliasManager::GetServiceRealName()
		 * @brief	Renvoyer le nom du service réel (le nom de l'interface)
		 *
		 * @return	QString	Le nom du service réel (le nom de l'interface)
		 *
		 * @sa	StreamWork::SwCore::ISwService::GetServiceRealName()
		 **/
		QString GetServiceRealName() override
		{
			return QString(typeid(this).name());
		}
	};
}  // namespace SwServiceAlias