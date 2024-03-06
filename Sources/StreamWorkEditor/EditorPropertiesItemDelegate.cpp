/**
@file EditorPropertiesItemDelegate.cpp
@brief
@author Big
*/

#include "EditorPropertiesItemDelegate.h"
#include "Properties/SwPropertiesModelImpl.h"
#include "Properties/ISwProperty.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//-----------------------------------------------------------------------
EditorPropertiesItemDelegate::EditorPropertiesItemDelegate(QObject *parent) :SwGuiDefaultItemDelegate(parent)
{
	//TO DO creation
}

//-----------------------------------------------------------------------
EditorPropertiesItemDelegate::~EditorPropertiesItemDelegate()
{
	//TO DO destruction
}

//-----------------------------------------------------------------------
void EditorPropertiesItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	SwGuiDefaultItemDelegate::paint(painter, option, index);

	if ( !index.isValid() || index.column() != 0 )
		return;
	QVariant value = index.model()->data(index, Qt::UserRole);
	void * ptr = value.value<void*>();
	if ( ptr == 0 )
	{
		return;
	}
	ISwProperty * p = static_cast<ISwProperty*>(ptr);
	if ( !p->HasChanged() || !p->isResettable() )
	{
		return;
	}
	if ( (option.state & QStyle::State_MouseOver) == QStyle::State_MouseOver )
	{
		QIcon ico(":/StreamWorkEditor/reset.png");
		ico.paint(painter, QRect(option.rect.right() - 17, option.rect.top(), 17, 17));
	}

}
