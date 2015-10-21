#pragma once

/*
* INCLUDES GLOBAUX
*/
#include "Error.h"

/**
* @namespace	SwServiceAlias
* @brief		Namespace de définition du service de gestion des alias
**/
namespace SwServiceAlias
{
	/**
	 * @class	SwUnloadedATOAliasError	SwUnloadedATOAliasError.h	"C:\tt4\Framework\StreamWork\Sources\SwService\ServiceAlias\Components\SwUnloadedATOAliasError.h"
	 * @brief	Classe d'erreur utilisée lors d'une erreur dans le chargement d'un fichier ATO
	 *
	 * @sa	alert::Error
	 **/
	class SwUnloadedATOAliasError : public alert::Error
	{
		DECLARE_CATEGORY("AliasError::AliasFileError");

	public:
		/**
		 * @fn		SwServiceAlias::SwUnloadedATOAliasError(alert::ProcessId processId, QString path)
		 * @brief	Constructeur par défaut
		 *
		 * @param	processId	Identifiant du processus émetteur
		 * @param	path		Chemin du fichier ATO
		 **/
		SwUnloadedATOAliasError(alert::ProcessId processId, QString path);

		/**
		 * @fn		virtual  SwServiceAlias::SwUnloadedATOAliasError::~SwUnloadedATOAliasError()
		 * @brief	Destructeur par défaut
		 **/
		virtual ~SwUnloadedATOAliasError();
	};
}  // namespace SwServiceAlias