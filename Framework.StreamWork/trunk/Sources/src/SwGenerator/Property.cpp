#include "Property.h"

Property::Property(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QRegExp rxString("([A-Z]|[a-z]|\\d|\\s|_)*");
	QRegExp rxQString("(\\w|\\s|\\d|_|\\.)*");
	QRegExp rxInt("-*\\d*");
	QRegExp rxUnsignedInt("\\d*");
	QRegExp rxDouble("-*\\d*\\.*\\d*");
	QRegExp rxBool("(true|false)");
	_vString= new QRegExpValidator(rxString, 0);
	_vQString= new QRegExpValidator(rxQString, 0);
	_vInt= new QRegExpValidator(rxInt, 0);
	_vUnsignedInt= new QRegExpValidator(rxUnsignedInt, 0);
	_vDouble= new QRegExpValidator(rxDouble, 0);
	_vBool= new QRegExpValidator(rxBool, 0);

	ui.Le_name->setValidator(_vString);
	ui.le_defaultValue->setValidator(_vInt);

	connect(ui.Cb_type,SIGNAL(currentIndexChanged ( const QString & )),this,SLOT(changeValidator(const QString & )));

}

Property::~Property()
{

}

//-------------------------------------------------------------------------
void Property::changeValidator( const QString & val )
{
	ui.le_defaultValue->setText("");
	if(val == "int")
	{
		ui.le_defaultValue->setValidator(_vInt);
	}

	if(val == "unsigned int")
	{
		ui.le_defaultValue->setValidator(_vUnsignedInt);
	}

	if(val == "bool")
	{
		ui.le_defaultValue->setValidator(_vBool);
	}

	if(val == "double")
	{
		ui.le_defaultValue->setValidator(_vDouble);
	}

	if(val == "QString")
	{
		ui.le_defaultValue->setValidator(_vQString);
	}
}
