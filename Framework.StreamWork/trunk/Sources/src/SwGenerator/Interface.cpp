#include "Interface.h"

Interface::Interface(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QRegExp rx("([A-Z]|[a-z]|\\d|\\s|_)*");
	v= new QRegExpValidator(rx, 0);

	ui.Le_name->setValidator(v);
	ui.Le_type->setValidator(v);
}

Interface::~Interface()
{


}


