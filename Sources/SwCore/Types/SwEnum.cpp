/*!
 \file SwEnum.cpp
 \brief Classe pour la gestion des enumeres
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

 */

#include "SwEnum.h"
#include "SwException.h"
#include "SwMacros.h"
#include "_SwEnumKeys_Class.h"
#include "SwEnumModel.h"
#include <string>

using namespace StreamWork::SwCore;



//-----------------------------------------------------------------------
SwEnum::SwEnum()
{
	_keys_values_pt = new _SwEnumKeys_Class;
	_is_flag = false;
	_i = 0;
}

//-----------------------------------------------------------------------
SwEnum::SwEnum(const SwEnum & source)
{
	if (&source != this)
	{
		_keys_values_pt = source._keys_values_pt;
		_is_flag = source._is_flag;
		_i = source._i;
	}
}

//-----------------------------------------------------------------------
SwEnum::~SwEnum()
{
	//Nothing to do
}

//-----------------------------------------------------------------------
SwEnum & SwEnum::operator=(const SwEnum& source)
{
	if (&source != this)
	{
		_keys_values_pt = source._keys_values_pt;
		_is_flag = source._is_flag;
		_i = source._i;
	}
	return *this;
}

//-----------------------------------------------------------------------
bool SwEnum::operator==(const SwEnum& val) const
{
	if (&val == this) return true;
	if (_i == val._i) return true;
	return false;
}

//-----------------------------------------------------------------------
void SwEnum::FromInt(const int source)
{
	_i = source;
}

//-----------------------------------------------------------------------
int SwEnum::ToInt() const
{
	return _i;
}

//-----------------------------------------------------------------------
QString SwEnum::ToString() const
{
	if (_is_flag)
	{
		QString s;
		QMap<int, QString>::const_iterator it;
		for (it = _keys_values_pt->_enum_values.begin(); it != _keys_values_pt->_enum_values.end(); it++)
		{
			if ((it.key() != 0 && (_i & it.key()) == it.key()) || (it.key() == 0 && _i == 0))
			{
				if (s.length() != 0)
					s += "|";
				s += it.value();
			}
		}
		return s;
	}
	return _keys_values_pt->_enum_values[_i];
}


//-----------------------------------------------------------------------
bool SwEnum::IsValid() const
{
	QMap<int, QString>::const_iterator it;
	int tmp;

	if (_is_flag)
	{
		//Cas flag
		tmp = 0;
		for (it = _keys_values_pt->_enum_values.begin(); it != _keys_values_pt->_enum_values.end(); it++)
		{
			tmp |= it.key();
		}
		//not
		tmp ^= 0xFFFFFFFF;
		tmp = _i & tmp;
		if (tmp != 0)
			return false;
		return true;
	}
	//cas enum
	it = _keys_values_pt->_enum_values.find(_i);
	if (it == _keys_values_pt->_enum_values.end())
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------
const QMap<int, QString> * SwEnum::GetValues() const
{
	return &(_keys_values_pt->_enum_values);
}

//-----------------------------------------------------------------------
bool SwEnum::IsFlag() const
{
	return _is_flag;
}


//-----------------------------------------------------------------------
void SwEnum::ChangeFlagStatus(bool is_flag)
{
	_is_flag = is_flag;
}

//-----------------------------------------------------------------------
void SwEnum::AddKey(int idx, QString enum_string)
{
	_SwEnumKeys_Class *enum_list;
	QMap<int, QString>::iterator it;

	if (_keys_values_pt->_getReferencesNb() > 1)
	{
		enum_list = new _SwEnumKeys_Class;
		enum_list->_enum_values = _keys_values_pt->_enum_values;
		_keys_values_pt = enum_list;
	}
	it = _keys_values_pt->_enum_values.find(idx);
	if (it != _keys_values_pt->_enum_values.end())
	{
		//Si la clef existe deja/on ajoute la chaine
		QString s = enum_string;
		s += "/"; s += it.value();
		it.value() = s;
		return;
	}
	_keys_values_pt->_enum_values.insert(idx, enum_string);
}

//-----------------------------------------------------------------------
void SwEnum::RemoveKey(int idx)
{
	_SwEnumKeys_Class *enum_list;
	QMap<int, QString>::iterator it;

	if (_keys_values_pt->_getReferencesNb() > 1)
	{
		enum_list = new _SwEnumKeys_Class;
		enum_list->_enum_values = _keys_values_pt->_enum_values;
		_keys_values_pt = enum_list;
	}
	it = _keys_values_pt->_enum_values.find(idx);
	if (it != _keys_values_pt->_enum_values.end())
	{
		_keys_values_pt->_enum_values.erase(it);
	}
}

//-----------------------------------------------------------------------
SwEnumModel * SwEnum::QueryModel(QObject * parent)
{
	return new SwEnumModel(this, parent);
}

//-----------------------------------------------------------------------
void SwEnum::AffectModel(SwEnumModel * model, int index)
{
	SwEnum model_content;

	//On verifie que l'enumere content est du meme type que this
	model_content = model->GetEnum();
	if (model_content._keys_values_pt != _keys_values_pt || model_content._is_flag != _is_flag)
	{
		QString msg = QString("Model content has a bad type");
		LAUNCH_SWEXCEPTION("SwCore", msg);
	}
	//Ok affectation
	//si flag
	if (_is_flag)
	{
		_i = model_content._i;
	}
	else
	{
		_i = model->GetVal(index);
	}
}

//-----------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwEnum &myObj)
{
	out << myObj.ToInt();
	return out;
}
//-----------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwEnum &myObj)
{
	int tmp;
	in >> tmp;
	myObj.FromInt(tmp);
	return in;
}


//-----------------------------------------------------------------------
void StreamWork::SwCore::SwEnum::FromString(const QString val, Qt::CaseSensitivity caseS /*= Qt::CaseSensitive*/)
{
	for (auto it = _keys_values_pt->_enum_values.begin(); it != _keys_values_pt->_enum_values.end(); it++)
	{
		if (QString::compare(it.value(), val, caseS) == 0)
		{
			_i = it.key();
			return;
		}
	}
}
