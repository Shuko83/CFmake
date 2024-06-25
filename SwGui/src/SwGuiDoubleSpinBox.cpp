#include "SwGuiDoubleSpinBox.h"
#include <QItemDelegate>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//-------------------------------------------------------------------------
SwGuiDoubleSpinBox::SwGuiDoubleSpinBox(SwDouble & source,QWidget *parent) : QDoubleSpinBox(parent)
{
	SetDouble(source);
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));

	this->setToolTip("This property is a <SwDouble>");
}

//-------------------------------------------------------------------------
SwGuiDoubleSpinBox::~SwGuiDoubleSpinBox()
{

}

//-------------------------------------------------------------------------
void SwGuiDoubleSpinBox::SetDouble(SwDouble & source) 
{
	_content=source;
	setMinimum(source.getMinimum());
	setMaximum(source.getMaximum());
	setValue(source.getValue());
	setSingleStep(source.getStep());   
	setDecimals(source.getPrecision());  
}

//-------------------------------------------------------------------------
SwDouble SwGuiDoubleSpinBox::GetDouble() 
{
	return _content;
}

//-------------------------------------------------------------------------
void SwGuiDoubleSpinBox::onValueChanged(double val)
{
	_content.setValue(val);
}
