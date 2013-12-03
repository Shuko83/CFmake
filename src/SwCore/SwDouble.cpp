#include "SwDouble.h"

#include <limits>

using namespace StreamWork::SwCore;

//---------------------------------------------------------------------------------
SwDouble::SwDouble() 
{
	_max = std::numeric_limits<double>::max();
	_min = std::numeric_limits<double>::min();
	_step = 0.1;
	_val = 0;
	_precision = 15;
	_doubleValidator = new SwDoubleQValidatorDecorator(_min, _max, _step, _val);
}

//---------------------------------------------------------------------------------
SwDouble::SwDouble(const SwDouble & source)
{
	if (&source!=this) 
	{
		_max = source._max;
		_min = source._min;
		_step = source._step;
		_val = source._val;
		_precision = source._precision;
		_doubleValidator = new SwDoubleQValidatorDecorator(_min, _max, _step, _val);
	}
}

//---------------------------------------------------------------------------------
SwDouble::~SwDouble() 
{
    //Nothing to do
}

//-------------------------------------------------------------------------
bool SwDouble::operator==( const SwDouble& source ) const
{
	if (&source==this) return true;
	if ( _val == source._val ) return true;
	return false;
}


//---------------------------------------------------------------------------------
void SwDouble::setValue( double val)
{
	_val = val;
}

//---------------------------------------------------------------------------------
double SwDouble::getValue() const
{
	return _val;
}

//---------------------------------------------------------------------------------
double SwDouble::getStep()
{
	return _step;
}

//---------------------------------------------------------------------------------
double SwDouble::getMaximum()
{
	return _max;
}

//---------------------------------------------------------------------------------
double SwDouble::getMinimum()
{
	return _min;
}

//---------------------------------------------------------------------------------
void SwDouble::setStep( double step)
{
	_step = step;
}

//---------------------------------------------------------------------------------
void SwDouble::setMaximum( double max)
{
	_max = max;
	_doubleValidator->setTop(max);
}

//---------------------------------------------------------------------------------
void SwDouble::setMinimum( double min)
{
	_min = min;
	_doubleValidator->setBottom(min);
}

//---------------------------------------------------------------------------------
int SwDouble::getPrecision()
{
	return _precision;
}

//---------------------------------------------------------------------------------
void SwDouble::setPrecision( double prec)
{
	_precision = prec;
	_doubleValidator->setDecimals(prec);
}



QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwDouble &myObj) 
{
    out<<myObj.getValue();
    return out;
}

QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwDouble &myObj) 
{
    double tmp;
    in>>tmp;
    myObj.setValue(tmp);
    return in;
}

//-------------------------------------------------------------------------
SwDoubleQValidatorDecorator* SwDouble::getValidator()
{
	return _doubleValidator;
}



/************************************************************************/
/*        SwDoubleQValidatorDecorator                                   */
/************************************************************************/
 
//---------------------------------------------------------------------------------
SwDoubleQValidatorDecorator::SwDoubleQValidatorDecorator(double min, double max, double step, double currentValue, QObject *parent /*= 0*/)
: QDoubleValidator(parent)
{
	_step = step;
	_currentValue = currentValue;
	setTop(max);
	setBottom(min);
}

//---------------------------------------------------------------------------------
SwDoubleQValidatorDecorator::~SwDoubleQValidatorDecorator()
{

}

//---------------------------------------------------------------------------------
void SwDoubleQValidatorDecorator::setStep( double step)
{
	_step = step;
}

//---------------------------------------------------------------------------------
void SwDoubleQValidatorDecorator::setCurrentValue( double currentValue)
{
	_currentValue = currentValue;
}

//---------------------------------------------------------------------------------
QValidator::State SwDoubleQValidatorDecorator::validate( QString & input, int & pos ) const
{
	//check du validator interne
	QValidator::State state = QValidator::Acceptable;

	state =  QDoubleValidator::validate(input, pos);

	// Si le modulo est nul, le step est ok par rapport à la valeur de départ
	if(fmod( (_currentValue - bottom()), _step) != 0 )
	{
		state = QValidator::Invalid;
	}
	return state;
}
