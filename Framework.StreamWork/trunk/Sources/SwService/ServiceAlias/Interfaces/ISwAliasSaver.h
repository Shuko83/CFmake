#pragma once

/*
* INCLUDES GLOBAUX
*/
#include <QString>

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @def		EXTENSION_FILE_ALIAS
	 * @brief	Définition de l'extension à utiliser pour la sauvegarde et le chargement d'alias dans le service
	 **/
#define EXTENSION_FILE_ALIAS QString("sal")

	/**
	 * @class	ISwAliasSaver	ISwAliasSaver.h	"C:\tt4\Framework\StreamWork\Sources\SwService\SwServiceAlias\Interfaces\ISwAliasSaver.h"
	 * @brief	Interface de gestion de la sauvegarde et du chargement des alias
	 **/
	class ISwAliasSaver
	{
	public:
		/**
		 * @fn		virtual QString SwServiceAlias::ISwAliasSaver::getXmlSchemaPath()
		 * @brief	Récupérer le chemin absolu vers le fichier XSD
		 *
		 * @return	QString	Chemin absolu vers le fichier XSD
		 **/
		virtual QString getXmlSchemaPath() const = 0;

		/**
		 * @fn		virtual QString SwServiceAlias::ISwAliasUser::getRoamingPath()
		 * @brief	Retourner le chemin du dossier de sauvegarde utilisé par le service
		 *
		 * @return	QString	Le chemin du dossier de sauvegarde
		 **/
		virtual QString getRoamingPath() const = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasSaver::setXmlSchemaPath(QString path)
		 * @brief	Définir le chemin absolu vers le fichier XSD
		 *
		 * @param	path	Chemin absolu vers le fichier XSD
		 **/
		virtual void setXmlSchemaPath(QString path) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasSaver::loadAlias(QString path)
		 * @brief	Charger des alias dans le service depuis un fichier
		 *
		 * @param	path	Chemin absolu vers le fichier
		 **/
		virtual void loadAlias(QString path) = 0;

		/**
		* @fn		virtual void SwServiceAlias::ISwAliasSaver::loadAliasFromATO(QString path)
		* @brief	Charger des alias dans le service depuis un fichier ATO
		*
		* @param	path	Chemin absolu vers le fichier ATO
		**/
		virtual void loadAliasFromATO(QString path) = 0;

		/**
		 * @fn		virtual void SwServiceAlias::ISwAliasSaver::saveAlias(QString path)
		 * @brief	Sauvegarder les alias du service dans un fichier
		 *
		 * @param	path	Chemin absolu vers le fichier
		 **/
		virtual void saveAlias(QString path) const = 0;
	};
}  // namespace SwServiceAlias