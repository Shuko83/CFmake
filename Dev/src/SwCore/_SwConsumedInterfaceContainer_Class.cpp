/*!
\file _SwConsumedInterfaceContainer_Class.cpp
\date 12/04/2006
\brief conteneur de stockage des informations relatives a une interface consommée
\author  Big
\version 1.0
 */
#include <QtGlobal>
#include "SwApplication.h"
#include "_SwConsumedInterfaceContainer_Class.h"

using namespace StreamWork::SwCore;


/*! \brief Construsteur */
_SwConsumedInterfaceContainer_Class::_SwConsumedInterfaceContainer_Class(SwInterfaces_Consumer_Class * parent) {
    _parent=parent;
    _name=QString();
    QString _provider_name=QString();
    QString _type=QString();
    _isAvailable=false;
    _hhandle=NULL;
    _provider_parent=NULL;
    _historical_index=Q_UINT64_C(0);
    _potential_provider_name=QString();
    _potential_provider_path=QString();
}
/*! \brief Destrusteur */
_SwConsumedInterfaceContainer_Class::~_SwConsumedInterfaceContainer_Class(){

}
/*! \brief Definit les caracteristiques de l'interfaces */
void _SwConsumedInterfaceContainer_Class::Define(QString name,QString type){
    _name=name;
    _type=type;
}
/*! \brief Acces au type de l'interface */
QString _SwConsumedInterfaceContainer_Class::GetType(){
    return _type;
}
/*! \brief Acces au nom de l'interface */
QString _SwConsumedInterfaceContainer_Class::GetName() {
    return _name;    
}
/*! \brief Definition du handle de l'interface */
void _SwConsumedInterfaceContainer_Class::SetInterfaceHandle(void ** handle){
    _hhandle=handle;
}
/*! \brief Definition de l'interface */
void _SwConsumedInterfaceContainer_Class::SetHandle(void * handle){
    *_hhandle=handle;
    if (handle==NULL) _isAvailable=false;
    else _isAvailable=true;
}
/*! \brief Revoie la disponibilite */
bool _SwConsumedInterfaceContainer_Class::IsAvailable(){
    return _isAvailable;
}
/*! \brief Renvoie l'index d'historique*/
quint64 _SwConsumedInterfaceContainer_Class::GetHistoricalIndex(){
    return _historical_index;
}
/*! \brief Renvoie le producteur ou NULL si non definit*/
ISwInterfaces_Provider * _SwConsumedInterfaceContainer_Class::GetProvider(){
    return _provider_parent;
}
/*! \brief Renvoie le nom de l'interface produite ou chaine vide si non definit*/
QString _SwConsumedInterfaceContainer_Class::GetProvidedInterfaceName(){
    return _provider_name;
}
/*! \brief Enregistre une interface consommatrice */
void _SwConsumedInterfaceContainer_Class::RegisterProviderInterface(ISwInterfaces_Provider * provider,QString pinterface) throw(SwException){
    try {
        _provider_parent=provider;
        _provider_name=pinterface;
        //Enregistrement du temps de l'action de regsitration
        _historical_index=SW_APP->GetHistoricCpt();
        provider->AddConsumer(_parent,_name,pinterface);
    } catch (SwException & se) {
        _provider_parent=NULL;
        _provider_name=QString();
        throw(se);
    } 
}
/*! \brief Desenregistre une interface consommatrice */
void _SwConsumedInterfaceContainer_Class::UnregisterProviderInterface(){
    _provider_parent->RemoveConsumer(_parent,_name,_provider_name);
    _provider_parent=NULL;
    _provider_name=QString();
}
/*! \brief Definition de l'interface potentielle et du fournisseur */
void _SwConsumedInterfaceContainer_Class::DefinePotentialProviderInterfaceDescription(QString ppotential_interface,QString ppotential_provider_path) {
    _potential_provider_name=ppotential_interface;
    _potential_provider_path=ppotential_provider_path;
}
/*! \brief Acces au nom de l'interface potentielle */
QString _SwConsumedInterfaceContainer_Class::GetPotentialProviderInterfaceName() {
    return _potential_provider_name;
}
/*! \brief Acces au path du fournisseur potentielle */
QString _SwConsumedInterfaceContainer_Class::GetPotentialProviderPath() {
    return _potential_provider_path;
}

