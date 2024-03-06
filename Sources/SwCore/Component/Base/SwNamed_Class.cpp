/*!
\file SwNamed_Class.cpp
\date 12/04/2006
\brief Classe dont l'instance est nommée
\author  Big
\version 1.0
\note
Project: Streamwork

\note
Company: DIGINEXT
*/
/*
  * INCLUDES LOCAUX
  */
#include <QRegularExpression>
#include <QDebug>
#include "Component/Base/SwNamed_Class.h"

using namespace StreamWork::SwCore;
using namespace LibIndeSig;


//-----------------------------------------------------------------------
SwNamed_Class::SwNamed_Class()
{
	_instance_name = "noname";
	_instance_description = "";
}

//-----------------------------------------------------------------------
SwNamed_Class::~SwNamed_Class()
{

}

//-----------------------------------------------------------------------
QString SwNamed_Class::GetDescription() const
{
	return _instance_description;
}

//-----------------------------------------------------------------------
QString SwNamed_Class::GetName() const
{
	return _instance_name;
}

//-----------------------------------------------------------------------
void SwNamed_Class::SetName(const QString & new_name) throw(SwException)
{
	if ( !CheckNameValidity(new_name) )
	{
		QString msg = QString("Invalid name %1 for instance. The name must check the following pattern \"[A-Za-z_][A-Za-z_0-9]*\"").arg(new_name);
		qDebug() << " : " << msg;
		// LAUNCH_SWEXCEPTION("SwCore",msg)
	}
	_instance_name = new_name;
	//Signal du changement
	OnChangeName();
}

//-----------------------------------------------------------------------
void SwNamed_Class::SetDescription(const QString & new_description)
{
	_instance_description = new_description;
	//Signal du changement
	OnChangeDescription();
}

//-----------------------------------------------------------------------
bool SwNamed_Class::CheckNameValidity(const QString & name)
{
	QRegularExpression rx("^[A-Za-z_][A-Za-z_0-9]*$");
	if ( !name.contains(rx) )
	{
		return false;
	}
	return true;
}

