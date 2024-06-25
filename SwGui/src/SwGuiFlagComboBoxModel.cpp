#include "SwGuiFlagComboBoxModel.h"

using namespace StreamWork::SwGui;

//---------------------------------------------------------------------------------
SwGuiFlagComboBoxModel::SwGuiFlagComboBoxModel(const QMetaEnum metaEnum, QObject * parent)
	: QAbstractListModel(parent), _value(0)
{
	_metaEnum = metaEnum;
}

//---------------------------------------------------------------------------------
SwGuiFlagComboBoxModel::~SwGuiFlagComboBoxModel()
{
}

//---------------------------------------------------------------------------------
int SwGuiFlagComboBoxModel::getValue() const
{
	return _value;
}

//---------------------------------------------------------------------------------
void SwGuiFlagComboBoxModel::setValue(int val)
{
	_value = val;
	emit dataChanged(index(0, 0, QModelIndex()), index(_metaEnum.keyCount() - 1, 0, QModelIndex()), { Qt::CheckStateRole });
}

//---------------------------------------------------------------------------------
int SwGuiFlagComboBoxModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : _metaEnum.keyCount();
}

//---------------------------------------------------------------------------------
QVariant SwGuiFlagComboBoxModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	switch (role)
	{
	case Qt::DisplayRole:
		return _metaEnum.key(index.row());
	case Qt::CheckStateRole:
		if (_metaEnum.isFlag())
		{
			int flag = _metaEnum.value(index.row());
			if ((_value & flag) == flag)
			{
				return Qt::Checked;
			}
			else if((_value & flag) == 0)
			{
				return Qt::Unchecked;
			}
			else
			{
				return Qt::PartiallyChecked;
			}
		}
		break;
	default:
		break;
	}

	return QVariant();
}

//---------------------------------------------------------------------------------
bool SwGuiFlagComboBoxModel::setData(const QModelIndex &i, const QVariant &value, int role)
{
	if (!i.isValid()) 
		return false;

	switch (role)
	{
	case Qt::CheckStateRole:
		if (_metaEnum.isFlag())
		{
			Qt::CheckState state = value.value<Qt::CheckState>();
			if (state == Qt::Unchecked)
			{
				_value &= ~_metaEnum.value(i.row());
			}
			else if (state == Qt::Checked)
			{
				_value |= _metaEnum.value(i.row());
			}
			emit dataChanged(index(0, 0, QModelIndex()), index(_metaEnum.keyCount() - 1, 0, QModelIndex()), { Qt::CheckStateRole });
			return true;
		}
	default:
		break;
	}
	return false;
}

//---------------------------------------------------------------------------------
Qt::ItemFlags SwGuiFlagComboBoxModel::flags(const QModelIndex &index) const
{
	if (_metaEnum.isFlag())
	{
		return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable;
	}
	else
	{
		return QAbstractItemModel::flags(index);
	}
}
