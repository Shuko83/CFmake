#include "SwGuiEnumIntComboBox.h"

#include <QLineEdit>
#include <QDebug>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
SwGuiEnumIntComboBox::SwGuiEnumIntComboBox(SwIntegerEnum & source,QWidget *parent)
    : QComboBox(parent)
{
	isPopulating = false;
    SetEnum(source);

    setFrame(false);
    setEditable(true);

	connect(lineEdit(), SIGNAL(editingFinished ()) , this, SLOT(onEditFinished()));
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slotActivated(int)));

	this->setToolTip("This property is a <SwIntegerEnum>");
}
/*! \brief Destructeur */
SwGuiEnumIntComboBox::~SwGuiEnumIntComboBox()
{
}
/*! \brief Definition Enum */
void SwGuiEnumIntComboBox::SetEnum(SwIntegerEnum & source) 
{
    _content=source;

	clear();

	const QMap<int, QString> map = _content.getValues();
	QMap<int, QString>::const_iterator it = map.constBegin();

	isPopulating = true;
	while (it != map.constEnd())
	{
		addItem(it.value(), QVariant::fromValue(it.key()));
		it++;
	}


	if(_content.isValidEnum(_content.toInt()))
	{
		setCurrentIndex(_content.toInt());
	}
	else
	{
		QString s = _content.toString();
		setEditText(s);
	}
	isPopulating = false;   
}

/*! \brief Definition Enum */
SwIntegerEnum SwGuiEnumIntComboBox::GetEnum() 
{
    return _content;
}


//---------------------------------------------------------------------------------
void SwGuiEnumIntComboBox::onEditFinished()
{
	QString s = currentText();
	_content.fromString(s);
}

//---------------------------------------------------------------------------------
void SwGuiEnumIntComboBox::slotActivated( int index )
{
	if(index != -1 && isPopulating == false)
	{
		QVariant var = itemData(index);
		if(var.isValid())
		{
			if( _content.isValidEnum(var.toInt()) )
				_content.fromInt(var.toInt());
		}		
	}

}
