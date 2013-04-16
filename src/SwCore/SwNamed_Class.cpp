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
#include <QRegExp>

#include "SwNamed_Class.h"

using namespace StreamWork::SwCore;
using namespace LibIndeSig;

/*! \brief Constructeur*/
SwNamed_Class::SwNamed_Class() {
	_instance_name="noname";
	_instance_description="";
}
/*! \brief Destructeur*/
SwNamed_Class::~SwNamed_Class(){

}
/*! \brief Permet de recuperer une description de l'instance nommé
\return description de l'instance nommé*/
QString SwNamed_Class::GetDescription() const{
	return _instance_description;
}
/*! \brief Permet de recuperer le nom de l'instance nommé
\return nom de l'instance nommé*/
QString SwNamed_Class::GetName() const{
	return _instance_name;
}
/*! \brief Permet de definir le nom de l'instance nommé
\param[in] new_name nouveau nom
\exception SwException invalid name for instance
*/
void SwNamed_Class::SetName(const QString & new_name) throw(SwException){
    if (!CheckNameValidity(new_name)) {
        QString msg=QString("Invalid name %1 for instance. The name must check the following pattern \"[A-Za-z_][A-Za-z_0-9]*\"").arg(new_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    _instance_name=new_name;
	//Signal du changement
	OnChangeName();
}
/*! \brief Permet de definir la description de l'instance nommé
\param[in] new_description nouvelle description
*/
void SwNamed_Class::SetDescription(const QString & new_description){
	_instance_description=new_description;
	//Signal du changement
    OnChangeDescription();
}
/*! \brief Permet de tester la validite du nom 
\param[in] nom a tester
\return true si nom valide et false sinon
*/
bool SwNamed_Class::CheckNameValidity(const QString & name) {
    QRegExp rx("^[A-Za-z_][A-Za-z_0-9]*$");
    if (!rx.exactMatch(name)) {
        return false;
    }
    return true;
}

