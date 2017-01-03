#pragma once

#include <QDoubleValidator>

/**
* @class CustomDoubleValidator
* @brief Validateur pour les Doubles hérité de QDoubleValidator mais qui est pourri à la base
*		 gère nos cas particuliers de validation d'un champ double.
* @sa QDoubleValidator
*/
class CustomDoubleValidator : public QDoubleValidator
{
public:
	CustomDoubleValidator(double min, double max, double decimal);
	~CustomDoubleValidator();


protected:
	virtual State validate(QString & input, int & pos) const;
};

