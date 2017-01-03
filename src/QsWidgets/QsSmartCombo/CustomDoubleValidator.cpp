#include "CustomDoubleValidator.h"

#include <QDebug>

//-------------------------------------------------------------------------
CustomDoubleValidator::CustomDoubleValidator(double min, double max, double decimal) : QDoubleValidator(min, max, decimal)
{

}


//-------------------------------------------------------------------------
CustomDoubleValidator::~CustomDoubleValidator()
{
}

//-------------------------------------------------------------------------
QValidator::State CustomDoubleValidator::validate(QString & input, int & /*pos*/) const
{
	State st = Acceptable;
	bool ok;
	double val = input.toDouble(&ok); 
	if (!ok)
	{
		if (input == "")
		{
			st = Intermediate;
		}
		else
		{
			return Invalid;
		}
	}
	

	if (top() <= 0)
	{
		if (val > top())
		{
			st = Intermediate;
		}
	}
	else
	{
		if (val > top())
		{
			return Invalid;
		}
	}

	if (bottom() < 0)
	{
		if (val < bottom())
		{
			return Invalid;
		}
	}
	else
	{
		if (val < bottom())
		{
			st = Intermediate;
		}
	}

	int digitSize = 0;
	if (input.contains("."))
		digitSize= QString(input).remove(QRegExp("^.*\\.")).size();
	if (digitSize > decimals())
	{
		return Invalid;
	}
	return st;
}
