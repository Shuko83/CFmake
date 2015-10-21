#pragma once

/*
* INCLUDES GLOBAUX
*/
#include "IAliasObserver.h"
#include "ITrack.h"
#include <QString>

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @class	ISwAliasUser	ISwAliasUser.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\Interfaces\ISwAliasUser.h"
	 * @brief	Interface de gestion de la lecture des alias
	 **/
	class ISwAliasUser
	{
	public:
		/**
		 * @fn		virtual QString SwServiceAlias::ISwAliasUser::getPrimaryAlias(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Récupérer l'alias principal lié à la piste indiquée en prenant en compte l'ordre de priorité des alias
		 *
		 * @param	track	Une piste
		 *
		 * @return	QString	L'alias lié à la piste
		 **/
		virtual QString getPrimaryAlias(gusa::model::interactionInterface::ITrack *track) const = 0;

		/**
		 * @fn		virtual QString SwServiceAlias::ISwAliasUser::getSecondaryAlias(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Récupérer l'alias secondaire lié à la piste indiquée en prenant en compte l'ordre de priorité des alias
		 *
		 * @param	track	Une piste
		 *
		 * @return	QString	L'alias lié à la piste
		 **/
		virtual QString getSecondaryAlias(gusa::model::interactionInterface::ITrack *track) const = 0;

		/**
		 * @fn		virtual bool SwServiceAlias::ISwAliasUser::isExist(gusa::model::interactionInterface::ITrack *track)
		 * @brief	Indiquer si un alias existe pour la piste indiquée
		 *
		 * @param	track	Une piste
		 *
		 * @return	bool	@b True si un alias existe, sinon @b False
		 **/
		virtual bool isExist(gusa::model::interactionInterface::ITrack *track) const = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasUser::registerObserver(IAliasObserver *obs)
		 * @brief	Inscrire l'observateur indiqué dans le service
		 *
		 * @param	obs	L'observateur à inscrire
		 **/
		virtual void registerObserver(AliasManager::IAliasObserver *obs) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasUser::unregisterObserver(IAliasObserver *obs)
		 * @brief	Désinscrire l'observateur indiqué dans le service
		 *
		 * @param	obs	L'observateur à désinscrire
		 **/
		virtual void unregisterObserver(AliasManager::IAliasObserver *obs) = 0;
	};
}  // namespace SwServiceAlias