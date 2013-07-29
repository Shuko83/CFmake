#include "SwGuiIntegerSpinBox.h"
#include <QItemDelegate>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
SwGuiIntegerSpinBox::SwGuiIntegerSpinBox(SwInteger & source,QWidget *parent) : QSpinBox(parent)
{
    SetInteger(source);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));

	this->setToolTip("This property is a <SwInteger>");
}
/*! \brief Destructeur */
SwGuiIntegerSpinBox::~SwGuiIntegerSpinBox()
{
}
/*! \brief Definition Enum */
void SwGuiIntegerSpinBox::SetInteger(SwInteger & source) 
{
    _content=source;
	setMinimum(source.getMinimum());
	setMaximum(source.getMaximum());
	setValue(source.getValue());
	setSingleStep(source.getStep());   
}
/*! \brief Definition Enum */
SwInteger SwGuiIntegerSpinBox::GetInteger() {
    return _content;
}
/*! \brief Activation d'un item */
void SwGuiIntegerSpinBox::onValueChanged(int val)
{
    _content.setValue(val);
}
