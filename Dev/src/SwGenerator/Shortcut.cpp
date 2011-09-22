#include "Shortcut.h"

Shortcut::Shortcut(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QRegExp rx("([A-Z]|[a-z]|\\d|\\s|_)*");
	v= new QRegExpValidator(rx, 0);

	ui.Le_displayName->setValidator(v);
}

Shortcut::~Shortcut()
{

}
