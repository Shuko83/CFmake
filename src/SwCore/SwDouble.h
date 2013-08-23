/*!
\brief Classe pour la gestion double avec min, max et pas
\author QBN
*/

#ifndef _STREAMWORK_SWCORE_SWDOUBLE_H
#define _STREAMWORK_SWCORE_SWDOUBLE_H
/*
* INCLUDES GLOBAUX
*/
#include <QVariant>
#include <QDataStream>
#include <QDoubleValidator>

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
		*	@brief Class Encapsulant une SwDouble pour la transformer en QDoubleValidator
		*/
		class SwDoubleQValidatorDecorator : public QDoubleValidator
		{
			Q_OBJECT
		public:
			SwDoubleQValidatorDecorator(double min, double max, double step, double currentValue, QObject *parent = 0);
			virtual ~SwDoubleQValidatorDecorator();

			/** @brief setter sur le pas */
			void setStep(double step);
			/** @brief setter sur la currentValue du Double concerné */
			void setCurrentValue(double currentValue);

			/** @brief méthode surchargée du validateur */
			virtual State 	validate ( QString & input, int & pos ) const;
		private:
			double _step;				/** @brief valeur du pas */
			double _currentValue;		/** @brief valeur courrante du Double */
		};


		/*!
		*	@brief Class QVariant encapsulant un double avec borne et pas
		*/
		class BUILD_SWCORE SwDouble {

		public:
			/*! \brief Constructeur de base */
			SwDouble();
			/*! \brief Constructeur de copie, necessaire pour enregistrer en QVariant */
			SwDouble(const SwDouble & source);
			/*! \brief Destructeur */
			virtual ~SwDouble();

			/** @brief mutateur de la valeur interne */
			void setValue(double);
			/** @brief accesseur sur la valeur */
			double getValue() const;
			/** @brief accesseur sur le pas */
			double getStep();
			/** @brief accesseur sur la valeur maximum */
			double getMaximum();
			/** @brief accesseur sur la valeur minimum */
			double getMinimum();
			/** @brief accesseur sur la precision */
			int getPrecision();

			/** @brief accesseur du QDoubleValidator affecté */
			SwDoubleQValidatorDecorator* getValidator();

			/** @brief mutateur sur le pas */
			void setStep(double);
			/** @brief mutateur sur la valeur maximum */
			void setMaximum(double);
			/** @brief mutateur sur la valeur minimum */
			void setMinimum(double);
			/** @brief mutateur de la précision */
			void setPrecision(double);

		private :
			double _val; /** @brief valeur de l'entier, init = 0.0 */
			double _step; /** @brief valeur du pas, init = 0.1 */
			double _max; /** @brief valeur maximum, init = std::numeric_limits<double>::max() */
			double _min; /** @brief valeur minimum, init = std::numeric_limits<double>::min() */
			int _precision; /** @brief valeur de la precision, init = 15 */ 
			SwDoubleQValidatorDecorator * _doubleValidator; /** @brief validateur utiliser pour verifier que le double est valide */
		};
	}
}

/** @brief enregistrement QVariant */
Q_DECLARE_METATYPE(StreamWork::SwCore::SwDouble)

/** @brief operateur de serialisation */
BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwDouble &myObj);
/** @brief operateur de deserialisation */
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwDouble &myObj);

//---------------------------------------------------------------------------
#endif
