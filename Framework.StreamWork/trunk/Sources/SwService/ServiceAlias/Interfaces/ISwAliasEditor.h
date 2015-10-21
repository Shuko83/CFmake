#pragma once

/*
* INCLUDES LOCAUX
*/
#include "SwSourceType.h"

/*
* INCLUDES GLOBAUX
*/
#include "IAlias.h"
#include "ITrack.h"
#include <QString>
#include <QList>

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @class	ISwAliasEditor	ISwAliasEditor.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\Interfaces\ISwAliasEditor.h"
	 * @brief	Interface de gestion de la modification des alias
	 **/
	class ISwAliasEditor
	{
	public:
		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::addAlias(QString number, Enums::SwSourceType type, QString name)
		 * @brief	Ajouter un alias dans le service en complétant l'identifiant si besoin
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 * @param	name	Le nom de l'alias
		 **/
		virtual void addAlias(QString number, Enums::SwSourceType type, QString name) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::editAlias(QString number, Enums::SwSourceType type, QString name)
		 * @brief	Modifier un alias dans le service en complétant l'identifiant si besoin
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 * @param	name	Le nom de l'alias
		 **/
		virtual void editAlias(QString number, Enums::SwSourceType type, QString name) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::addAliasToTn(gusa::model::interactionInterface::ITrack *track, QString name)
		 * @brief	Ajouter un alias, liés au TN, dans le service (utilisé seulement en fastAction)
		 *
		 * @param	track	La piste de l'alias
		 * @param	name	Le nom de l'alias
		 **/
		virtual void addAliasToTn(gusa::model::interactionInterface::ITrack *track, QString name) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::clearAlias()
		 * @brief	Supprimer tous les alias présents dans le service
		 **/
		virtual void clearAlias() = 0;

		/**
		 * @fn		virtual QList<AliasManager::IAlias*> SwServiceAlias::ISwAliasEditor::getAllAlias()
		 * @brief	Récupérer tous les alias présents dans le service
		 *
		 * @return	QList<AliasManager::IAlias*>	Liste de tous les alias du service
		 **/
		virtual QList<AliasManager::IAlias*> getAllAlias() const = 0;

		/**
		 * @fn		virtual bool SwServiceAlias::ISwAliasEditor::isExist(QString number, Enums::SwSourceType type)
		 * @brief	Indiquer si un alias existe pour l'identifiant de piste indiqué
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 *
		 * @return	bool	@b True si un alias existe, sinon @b False
		 **/
		virtual bool isExist(QString number, Enums::SwSourceType type) const = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::removeAlias(QString number, Enums::SwSourceType type)
		 * @brief	Supprimer un alias du service
		 *
		 * @param	number	L'identifiant ou la plage d'identifiant de la piste
		 * @param	type	Le type d'identifiant
		 **/
		virtual void removeAlias(QString number, Enums::SwSourceType type) = 0;

		/**
		* @fn		virtual QList<Enums::SwSourceType> SwServiceAlias::ISwAliasEditor::getPrimaryOrder()
		* @brief	Récupérer l'ordre d'utilisation des alias pour l'alias principal
		*
		* @return	QList<Enums::SwSourceType>	Liste des types de source pour l'alias principal
		**/
		virtual QList<Enums::SwSourceType> getPrimaryOrder() const = 0;

		/**
		* @fn		virtual QList<Enums::SwSourceType> SwServiceAlias::ISwAliasEditor::setSecondaryOrder()
		* @brief	Récupérer l'ordre d'utilisation des alias pour l'alias secondaire
		*
		* @return	QList<Enums::SwSourceType>	Liste des types de source pour l'alias secondaire
		**/
		virtual QList<Enums::SwSourceType> getSecondaryOrder() const = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::setPrimaryOrder(QList<Enums::SwSourceType> order)
		 * @brief	Définir l'ordre d'utilisation des alias pour l'alias principal
		 *
		 * @param	order	Liste des types de source à utiliser
		 **/
		virtual void setPrimaryOrder(QList<Enums::SwSourceType> order) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasEditor::setSecondaryOrder(QList<Enums::SwSourceType> order)
		 * @brief	Définir l'ordre d'utilisation des alias pour l'alias secondaire
		 *
		 * @param	order	Liste des types de source à utiliser
		 **/
		virtual void setSecondaryOrder(QList<Enums::SwSourceType> order) = 0;
	};
}  // namespace SwServiceAlias