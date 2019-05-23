#include "SwString.h"

using namespace StreamWork::SwCore;

//---------------------------------------------------------------------------------
SwString::SwString()
{
	_validator = 0;
}

//---------------------------------------------------------------------------------
SwString::SwString( const SwString & source )
{
	_string = source._string;
	_validator = source._validator;
}

//---------------------------------------------------------------------------------
SwString::~SwString()
{

}	

//-------------------------------------------------------------------------
bool SwString::operator==( const SwString& source ) const
{
	if (&source==this) return true;
	if (_string == source._string) return true;
	return false;
}

//---------------------------------------------------------------------------------
QValidator::State SwString::validate( QString & input, int & pos ) const
{
	//check du validator interne
	QValidator::State state = QValidator::Acceptable;
	if(_validator)
	{
		state = _validator->validate(input, pos);
	}
	return state;
}

//---------------------------------------------------------------------------------
void SwString::fromString(const QString & string)
{
	QString copy = string;
	int pos = 0;
	if(validate(copy, pos) == QValidator::Acceptable)
	{
		_string = copy;
	}
}

//---------------------------------------------------------------------------------
void SwString::setValidator( QValidator * validator )
{
	_validator = validator;
}

//-------------------------------------------------------------------------
QValidator * SwString::getValidator()
{
	return _validator;
}

//---------------------------------------------------------------------------------
const QString& SwString::toString() const
{
	return _string;
}

//---------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwString &myObj) {
	out<<myObj.toString();
	return out;
}

//---------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwString &myObj) {
	QString tmp;
	in>>tmp;
	myObj.fromString(tmp);
	return in;
}



