/********************************************************************
filename: 	SwString.h
author:		QBN
*********************************************************************/

#ifndef STREAMWORK_SWCORE_SWSTRING_H
#define STREAMWORK_SWCORE_SWSTRING_H
/*
* INCLUDES GLOBAUX
*/
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDataStream>
#include <QValidator>

/*
* INCLUDES LOCAUX
*/
#include "SwCoreConstantes.h"
#include "SwRefPtr.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		*	@brief Class QVariant encapsulant uen chaine de caractere, contenant un validator
		*/
		class BUILD_SWCORE SwString 
		{
		private:
			QString _string; /** @brief Valeur interne de la chaine de caractere */
			QValidator * _validator; /** @brief validateur utiliser pour verifier que la chaine de caractere est valide */

		public:
			/*! \brief Constructeur de base */
			SwString();
			/*! \brief Constructeur de copie */
			SwString(const SwString & source);
			/*! \brief Destructeur */
			virtual ~SwString();
			/*! \brief Operateur de comparaison*/
			bool operator==(const SwString& source) const;

			/** @brief Permet d'affecter le QValidator à utiliser
			*	@param QValidator * => validator : this ne gere pas la destruction du validator */			
			void setValidator(QValidator * validator);

			/** @brief Test si une chaine de caractere est valide selon le validator setté via setValidator()
			*	@return QValidator::State =>  valid si pas de validateur setté, la valeur de retour du validator sinon */
			QValidator::State validate ( QString & input, int & pos ) const	;

			/** @brief Permet de récupérer le QValidator affecté
			*	@return QValidator * => validator  */			
			QValidator * getValidator();

			/** @brief Affecte la valeur interne de la chaine de caractere
			*	@param QString & => utilisé pour affecter à la chaine interne si valide */			
			void fromString(const QString &);

			/** @brief renvoi la valeur interne de la chaine de caractere
			*	@return QString => valeur interne de la chaine de caractere */			
			const QString& toString() const;

		};
	}
}

/** @brief enregistrement QVariant */
Q_DECLARE_METATYPE(StreamWork::SwCore::SwString)

/** @brief operateur de serialisation */
BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwString &myObj);

/** @brief operateur de deserialisation */
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwString &myObj);

#endif