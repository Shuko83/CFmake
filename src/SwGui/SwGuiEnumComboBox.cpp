/*!
 \file SwGuiEnumComboBox.h
 \brief ComboBox pour l'edition d'un SwEnum
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#include "SwGuiEnumComboBox.h"
#include <QItemDelegate>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


//-------------------------------------------------------------------------
SwGuiEnumComboBox::SwGuiEnumComboBox(SwEnum & source,QWidget *parent)
    : QComboBox(parent)
{
    _content=source;
    SwEnumModel * emodel=_content.QueryModel(this);
    setModel(emodel);
    setFrame(false);
    setEditable(false);
    
    if (!source.IsFlag()) 
        setCurrentIndex(emodel->GetIndex(_content.ToInt()));
    
    connect(this, SIGNAL(activated(int)), this, SLOT(slotActivated(int)));

	this->setToolTip("This property is a <SwEnum>");
}

//-------------------------------------------------------------------------
SwGuiEnumComboBox::~SwGuiEnumComboBox()
{
}

//-------------------------------------------------------------------------
void SwGuiEnumComboBox::SetEnum(SwEnum & source) 
{

    _content=source;
    SwEnumModel * emodel=_content.QueryModel(this);
    setModel(emodel);
    if (!source.IsFlag())
        setCurrentIndex(emodel->GetIndex(_content.ToInt()));
}

//-------------------------------------------------------------------------
SwEnum SwGuiEnumComboBox::GetEnum() 
{
    SwEnumModel * emodel=dynamic_cast<SwEnumModel *>(model());
    _content.AffectModel(emodel,currentIndex());
    return _content;
}

//-------------------------------------------------------------------------
void SwGuiEnumComboBox::slotActivated(int index)
{
    SwEnumModel * emodel=dynamic_cast<SwEnumModel *>(model());

    QVariant value = itemData(index, Qt::CheckStateRole);
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    setItemData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
    _content.AffectModel(emodel,currentIndex());

    if (_content.IsFlag()) 
	{
        showPopup();
        setEditText(_content.ToString());
    } 
	else 
	{
        currentIndexChanged(index);
    }
}
