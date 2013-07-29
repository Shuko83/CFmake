/********************************************************************
	filename: 	SwIntegerEnum.h
	author:		QBN
*********************************************************************/

#ifndef STREAMWORK_SWCORE_SWINTEGERENUM_H
#define STREAMWORK_SWCORE_SWINTEGERENUM_H
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
		*	@brief Class QVariant encapsulant un entier ayant certaines valeurs défini dans dans une énumeration
		*	La valeur interne peut prendre des valeurs non défini dans l'enum, si elles rentrent dans les bornes
		*/
        class BUILD_SWCORE SwIntegerEnum 
		{
        private:
			QMap<int, QString> _enum; /** @brief map des valeurs d'enum, correspondance valeur => nom*/
			int _min; /** @brief valeur minimum */
			int _max; /** @brief valeur maximum */
			int _currentValue; /** @brief valeur courante de l'enum */
			QValidator * _validator; /** @brief validateur interne permetant de valider la saisie texte sur la valeur entiere de l'enum */

        public:
			/*! \brief Constructeur de base */
			SwIntegerEnum();
			/*! \brief Constructeur de copie, necessaire pour enregistrer en QVariant /!\ le pointeur sur le validator est copié */
			SwIntegerEnum(const SwIntegerEnum & source);
			/*! \brief Destructeur */
			virtual ~SwIntegerEnum();

			/** @brief Permet d'affecter le QValidator à utiliser
			*	@param QValidator * => validator : this ne gere pas la destruction du validator */			
			void setValidator(QValidator * validator);
			
			/** @brief mutateur valeur minimum */
			void setMin(int min);
			/** @brief mutateur valeur maximum */
			void setMax(int max);

			/** @brief accesseur valeur maximum */
			int getMax();
			/** @brief accesseur valeur minimum */
			int getMin();

			/** @brief permet de valider qu'une chaine de caractere correspond à une valeur de cette enum 
			*	Appel en premier le validateur setté via la methode setValidator(...) puis transforme la chaine en entier 
			*   puis verifie que l'entier défini par la chaine de caractere est compris entre les bornes min et max.
			*	@param QString & => input : chaine à tester
			*	@param int => pos : position dans la chaine (utilisé uniquement pas le validateur)	*/
			QValidator::State validate ( QString & input, int & pos ) const	;		

			/** @brief permet de tester si la valeur entiere à nom associé défini dans l'enum
			*	@param int => val : valeur à tester	*/
			bool isValidEnum(int val);

			/** @brief accesseur sur la valeur courante de l'enum */
			int toInt() const;
			/** @brief mutateur sur la valeur courante de l'enum, verifie que la valeur est entre les bornes*/
			void fromInt(int val);

			/** @brief permet ajouter une valeur d'enum (association valuer / nom) */
			void addKey(int val, QString & name);
			/** @brief supprime une valeur d'enum */
			void removeKey(int val);

			/** @brief accesseur à la map de l'enum */
			QMap<int, QString> & getValues();
			
			/** @brief permet de transformer une chaine de caractere vers une valeur d'enum
			*	Si la chaine est un nom de l'enum, la valeur courante prends la valeur correspondant au nom,
			*	Sinon on verifie que la chaine est un entier valide via la methode 'validate' 
			*	et on affecte à la valeur courante l'entier contenu dans la chaine de caractere
			*	@param QString => chaine de caractere à utiliser pour affecter la valeur courante de l'enum */
			void fromString(QString &);

			/** @brief permet de transformer en chaine de caractere
			*	@return QString =>	Si la valeur est contenu dans la map d'enum, on retourne le nom associé
			*						sinon on retourne la valeur d'enume courante entiere sous forme de chaine de caractere */
			QString toString();

        };
	}
}

/** @brief enregistrement QVariant */
Q_DECLARE_METATYPE(StreamWork::SwCore::SwIntegerEnum)

/** @brief operateur de serialisation */
BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIntegerEnum &myObj);

/** @brief operateur de deserialisation */
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIntegerEnum &myObj);
//---------------------------------------------------------------------------
#endif