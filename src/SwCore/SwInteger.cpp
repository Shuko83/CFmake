#include "SwInteger.h"

#include <limits>

using namespace StreamWork::SwCore;

//-------------------------------------------------------------------------
SwInteger::SwInteger() 
{
	_max = std::numeric_limits<int>::max();
	_min = std::numeric_limits<int>::min();
   _step = 1;
   _val = 0;
   _intValidator = new SwIntegerQValidatorDecorator(_min, _max, _step, _val);
}

//-------------------------------------------------------------------------
SwInteger::SwInteger(const SwInteger & source)
{
    if (&source!=this) 
	{
        _max = source._max;
		_min = source._min;
		_step = source._step;
		_val = source._val;
		_intValidator = new SwIntegerQValidatorDecorator(_min, _max, _step, _val);
    }
}

//-------------------------------------------------------------------------
SwInteger::~SwInteger() 
{
    //Nothing to do
}

//-------------------------------------------------------------------------
bool SwInteger::operator==( const SwInteger& source ) const
{
	if (&source==this) return true;
	if (//   _max == source._max 
		//&& _min == source._min
		//&& _step == source._step
		_val == source._val) return true;
	return false;
}

//---------------------------------------------------------------------------------
void SwInteger::setValue( int val)
{
	_val = val;
	_intValidator->setCurrentValue(val);
}

//---------------------------------------------------------------------------------
int SwInteger::getValue() const
{
	return _val;
}

//---------------------------------------------------------------------------------
int SwInteger::getStep()
{
	return _step;
}

//---------------------------------------------------------------------------------
int SwInteger::getMaximum()
{
	return _max;
}

//---------------------------------------------------------------------------------
int SwInteger::getMinimum()
{
	return _min;
}

//---------------------------------------------------------------------------------
void SwInteger::setStep( int step)
{
	_step = step;
	_intValidator->setStep(step);
}

//---------------------------------------------------------------------------------
void SwInteger::setMaximum( int max)
{
	_max = max;
	_intValidator->setTop(max);
}

//---------------------------------------------------------------------------------
void SwInteger::setMinimum( int min)
{
	_min = min;
	_intValidator->setBottom(min);
}

//---------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwInteger &myObj) 
{
    out<<myObj.getValue();
    return out;
}

//---------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwInteger &myObj) 
{
    int tmp;
    in>>tmp;
    myObj.setValue(tmp);
    return in;
}

//-------------------------------------------------------------------------
SwIntegerQValidatorDecorator* SwInteger::getValidator()
{
	return _intValidator;
}


/************************************************************************/
/*        SwIntegerQValidatorDecorator                                   */
/************************************************************************/

//---------------------------------------------------------------------------------
SwIntegerQValidatorDecorator::SwIntegerQValidatorDecorator(int min, int max, int step, int currentValue, QObject *parent /*= 0*/)
: QIntValidator(parent)
{
	_step = step;
	_currentValue = currentValue;
	setTop(max);
	setBottom(min);
}


//---------------------------------------------------------------------------------
SwIntegerQValidatorDecorator::~SwIntegerQValidatorDecorator()
{

}

//-------------------------------------------------------------------------
void SwIntegerQValidatorDecorator::setStep( int step )
{
	_step = step;
}

//-------------------------------------------------------------------------
void SwIntegerQValidatorDecorator::setCurrentValue( int currentValue )
{
	_currentValue = currentValue;
}


//---------------------------------------------------------------------------------
QValidator::State SwIntegerQValidatorDecorator::validate( QString & input, int & pos ) const
{
	//check du validator interne
	QValidator::State state = QValidator::Acceptable;

	state =  QIntValidator::validate(input, pos);

	// Si le modulo est nul, le step est ok par rapport à la valeur de départ
	if( (_currentValue - bottom()) % _step != 0 )
	{
		state = QValidator::Invalid;
	}

	return state;
}


