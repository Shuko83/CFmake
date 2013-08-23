#include "SwIntegerEnum.h"

#include <limits>

using namespace StreamWork::SwCore;

//---------------------------------------------------------------------------------
SwIntegerEnum::SwIntegerEnum()
{
	_max = std::numeric_limits<int>::max();
	_min = std::numeric_limits<int>::min();
	_currentValue = 0;
	_validator = 0;
}

//---------------------------------------------------------------------------------
SwIntegerEnum::SwIntegerEnum( const SwIntegerEnum & source )
{
	_max = source._max;
	_min = source._min;
	_currentValue = source._currentValue;
	_validator = source._validator;
	_enum = source._enum;
}

//---------------------------------------------------------------------------------
SwIntegerEnum::~SwIntegerEnum()
{
}	

//---------------------------------------------------------------------------------
QValidator::State SwIntegerEnum::validate( QString & input, int & pos ) const
{
	//check du validator interne
	QValidator::State state = QValidator::Acceptable;
	if(_validator)
	{
		state = _validator->validate(input, pos);
	}

	//puis check des bornes de l'entier
	if(state == QValidator::Acceptable)
	{
		int val = input.toInt();
		if( val > _max || val < _min )
		{
			state = QValidator::Invalid;
		}
	}

	return state;
}

//---------------------------------------------------------------------------------
int SwIntegerEnum::toInt() const
{
	return _currentValue;
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::fromInt( int val )
{
	if(val <= _max && val >= _min)
		_currentValue = val;
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::addKey( int val, QString & name )
{
	_enum.insert(val, name);
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::removeKey( int val )
{
	_enum.remove(val);
}

//---------------------------------------------------------------------------------
QMap<int, QString> & SwIntegerEnum::getValues()
{
	return _enum;
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::fromString( QString & string)
{
	//on recherche dabord dans la liste des valeurs d'enum
	QList<QString> values = _enum.values();
	for (int i = 0; i < values.size(); ++i) 
	{
		if (values.at(i) == string)
		{
			_currentValue = i;
			return ;
		}
	}	

	int pos = 0;
	if(validate(string, pos) == QValidator::Acceptable)
	{
		_currentValue = string.toInt();
	}
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::setValidator( QValidator * validator)
{
	_validator = validator;
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::setMin( int min )
{
	_min = min;
}

//---------------------------------------------------------------------------------
void SwIntegerEnum::setMax( int max )
{
	_max = max;
}

//---------------------------------------------------------------------------------
int SwIntegerEnum::getMax()
{
	return _max;
}

//---------------------------------------------------------------------------------
int SwIntegerEnum::getMin()
{
	return _min;
}

//---------------------------------------------------------------------------------
QString SwIntegerEnum::toString()
{
	if(isValidEnum(_currentValue))
		return _enum.find(_currentValue).value();
	else 
		return QString::number(_currentValue);
}

//---------------------------------------------------------------------------------
bool SwIntegerEnum::isValidEnum( int val )
{
	if(_enum.contains(val))
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwIntegerEnum &myObj) {
	out<<myObj.toInt();
	return out;
}

//---------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwIntegerEnum &myObj) {
	int tmp;
	in>>tmp;
	myObj.fromInt(tmp);
	return in;
}


