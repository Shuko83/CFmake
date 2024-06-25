#include "SwGuiStringLineEdit.h"

#include <QDebug>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

//---------------------------------------------------------------------------------
SwGuiStringLineEdit::SwGuiStringLineEdit(SwString & source,QWidget *parent)
    : QLineEdit(parent), _validator(source)
{
     SetString(source);
 
     setFrame(false);
 
 	connect(this, SIGNAL(editingFinished ()) , this, SLOT(onEditFinished()));

	this->setToolTip("This property is a <SwString>");

}

//---------------------------------------------------------------------------------
SwGuiStringLineEdit::~SwGuiStringLineEdit()
{
}

//---------------------------------------------------------------------------------
void SwGuiStringLineEdit::SetString(SwString & source) 
{
     _content=source;
 	_validator.setValidator(source);
 	setValidator(&_validator);
 	setText(_content.toString());
}

//---------------------------------------------------------------------------------
SwString SwGuiStringLineEdit::GetString() 
{
   return _content;
}

//---------------------------------------------------------------------------------
void SwGuiStringLineEdit::onEditFinished()
{
	_content.fromString(text());
}

/************************************************************************/
/*        SwStringQValidatorDecorator                                   */
/************************************************************************/

//---------------------------------------------------------------------------------
SwStringQValidatorDecorator::SwStringQValidatorDecorator( SwString & source,QWidget *parent /*= 0*/ ) : QValidator(parent)
{
	_content = source;
}

//---------------------------------------------------------------------------------
SwStringQValidatorDecorator::~SwStringQValidatorDecorator()
{

}

//---------------------------------------------------------------------------------
void SwStringQValidatorDecorator::setValidator( SwString & source )
{
	_content = source;
}

//---------------------------------------------------------------------------------
QValidator::State SwStringQValidatorDecorator::validate( QString & input, int & pos ) const
{
	return _content.validate(input, pos);
}
