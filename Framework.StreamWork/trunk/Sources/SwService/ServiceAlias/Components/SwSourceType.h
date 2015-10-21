#pragma once

/*
* INCLUDES LOCAUX
*/
#include "SwServiceAlias_Constantes.h"

/*
* INCLUDES GLOBAUX
*/
#include <QObject>

/**
 * @namespace	SwServiceAlias
 * @brief		Namespace de définition du service de gestion des alias
 **/
namespace SwServiceAlias
{
	/**
	 * @class	Enums	SwSourceType.h	"C:\tt4\Framework\StreamWork\Sources\SwService\ServiceAlias\Components\SwSourceType.h"
	 * @brief	Classe conteneur pour les énumérateurs du service
	 *
	 * @sa	QObject
	 **/
	class ALIAS_SERVICE_EXPORT Enums : public QObject
	{
		Q_OBJECT;

	public:
		/**
		 * @enum	SwSourceType
		 * @brief	Énumérateur définissant les différents type d'identifiant pour les pistes
		 **/
		enum class SwSourceType : unsigned int
		{
			TN = 0,
			VCS = 1,
			IFF1 = 2,
			IFF2 = 3,
			IFF3 = 4
		};

		Q_ENUM(SwSourceType);
	};
}  // namespace SwServiceAlias