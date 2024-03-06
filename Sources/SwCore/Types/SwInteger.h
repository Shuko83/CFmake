/*!
\brief Classe pour la gestion entiers avec min, max et pas
\author QBN
*/

#ifndef _STREAMWORK_SWCORE_SWINTEGER_H
#define _STREAMWORK_SWCORE_SWINTEGER_H
/*
* INCLUDES GLOBAUX
*/
#include <QVariant>
#include <QDataStream>
#include <QIntValidator>
#include <QValidator>

/*
* INCLUDES LOCAUX
*/
#include "SwCoreConstantes.h"
#include "SwRef/SwRefPtr.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		*	@brief Class Encapsulant une SwInteger pour la transformer en QIntValidator
		*/
		class SwIntegerQValidatorDecorator : public QIntValidator
		{
			Q_OBJECT
		public:
			SwIntegerQValidatorDecorator(int min, int max, int step, int currentValue, QObject *parent = 0);
			virtual ~SwIntegerQValidatorDecorator();

			/** @brief setter sur le pas */
			void setStep(int step);
			/** @brief setter sur la currentValue du Int concerné */
			void setCurrentValue(int currentValue);

			/** @brief méthode surchargée du validateur */
			virtual State validate ( QString & input, int & pos ) const;
		private:
			int _step;				/** @brief valeur du pas */
			int _currentValue;		/** @brief valeur courrante du Int */
		};


		/*!
		*	@brief Class QVariant encapsulant un entier avec borne et pas
		*/
		class BUILD_SWCORE SwInteger {

		public:
			/** \brief Constructeur de base */
			SwInteger();
			/*! \brief Constructeur de copie, necessaire pour enregistrer en QVariant */
			SwInteger(const SwInteger & source);
			/*! \brief Destructeur */
			virtual ~SwInteger();
			/*! \brief Operateur de comparaison*/
			bool operator==(const SwInteger& source) const;

			/** @brief mutateur de la valeur interne */
			void setValue(int);
			/** @brief accesseur sur la valeur */
			int getValue() const;
			/** @brief accesseur sur le pas */
			int getStep();
			/** @brief accesseur sur la valeur maximum */
			int getMaximum();
			/** @brief accesseur sur la valeur minimum */
			int getMinimum();

			/** @brief accesseur du QIntValidator affecté */
			SwIntegerQValidatorDecorator* getValidator();

			/** @brief mutateur sur le pas */
			void setStep(int);
			/** @brief mutateur sur la valeur maximum */
			void setMaximum(int);
			/** @brief mutateur sur la valeur minimum */
			void setMinimum(int);


		private :
			int _val; /** @brief valeur de l'entier, init = 0 */
			int _step; /** @brief valeur du pas, init = 1 */
			int _max; /** @brief valeur maximum, init = std::numeric_limits<int>::max() */
			int _min; /** @brief valeur minimum, init = std::numeric_limits<int>::min() */
			SwIntegerQValidatorDecorator * _intValidator; /** @brief validateur utiliser pour verifier que le int est valide */

		};
	}
}

/** @brief enregistrement QVariant */
Q_DECLARE_METATYPE(StreamWork::SwCore::SwInteger)

/** @brief operateur de serialisation */
BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwInteger &myObj);

/** @brief operateur de deserialisation */
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwInteger &myObj);

//---------------------------------------------------------------------------
#endif
