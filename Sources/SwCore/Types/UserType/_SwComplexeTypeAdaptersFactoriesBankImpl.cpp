/*!
\file _SwComplexeTypeAdaptersFactoriesBankImpl.cpp
\date 12/04/2006
\brief implementation banque des fabriques de complexe types adaptateur
\author  Big
\version 1.0
 */

#include "Types/UserType/_SwComplexeTypeAdaptersFactoriesBankImpl.h"
#include "Types/UserType/ISwComplexeTypeAdaptersFactory.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwComplexeTypeAdaptersFactoriesBankImpl::_SwComplexeTypeAdaptersFactoriesBankImpl() {

}
/*! \brief Destructeur */
_SwComplexeTypeAdaptersFactoriesBankImpl::~_SwComplexeTypeAdaptersFactoriesBankImpl() {
    QMap<int,ISwComplexeTypeAdaptersFactory *>::iterator it;

    for (it=_ctfactories.begin();it!=_ctfactories.end();it++) {
        it.value()->Liberate();
    }

}
/*! \brief Creation d'un adapteur */
void _SwComplexeTypeAdaptersFactoriesBankImpl::AddCTFactory(ISwComplexeTypeAdaptersFactory * factory) {
    QMap<int,ISwComplexeTypeAdaptersFactory *>::iterator it;

    it=_ctfactories.find(factory->GetUserType());
    if (it!=_ctfactories.end())
        return; //Deja existant
    _ctfactories.insert(factory->GetUserType(),factory);
}
/*! \brief Acces a une factory pour un user type (cd QVariant userType)*/
ISwComplexeTypeAdaptersFactory * _SwComplexeTypeAdaptersFactoriesBankImpl::GetFactoryForUserType(int user_type) {
    QMap<int,ISwComplexeTypeAdaptersFactory *>::iterator it;

    it=_ctfactories.find(user_type);
    if (it!=_ctfactories.end())
        return it.value(); //Ok Trouve
    return NULL;
}
