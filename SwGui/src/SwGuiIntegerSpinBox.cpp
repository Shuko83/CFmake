#include "SwGuiIntegerSpinBox.h"
#include <QItemDelegate>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//---------------------------------------------------------------------------------
SwGuiIntegerSpinBox::SwGuiIntegerSpinBox(SwInteger & source,QWidget *parent) : QSpinBox(parent)
{
    SetInteger(source);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));

	this->setToolTip("This property is a <SwInteger>");
}

//---------------------------------------------------------------------------------
SwGuiIntegerSpinBox::~SwGuiIntegerSpinBox()
{
}

//---------------------------------------------------------------------------------
void SwGuiIntegerSpinBox::SetInteger(SwInteger & source) 
{
    _content=source;
	setMinimum(source.getMinimum());
	setMaximum(source.getMaximum());
	setValue(source.getValue());
	setSingleStep(source.getStep());   
}

//---------------------------------------------------------------------------------
SwInteger SwGuiIntegerSpinBox::GetInteger() 
{
    return _content;
}

//---------------------------------------------------------------------------------
void SwGuiIntegerSpinBox::onValueChanged(int val)
{
    _content.setValue(val);
}
