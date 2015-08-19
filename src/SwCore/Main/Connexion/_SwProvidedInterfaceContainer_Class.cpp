/*!
\file _SwProvidedInterfaceContainer_Class.h
\date 12/04/2006
\brief conteneur de stockage des informations relatives a une interface fournie
\author  Big
\version 1.0
 */

#include "_SwProvidedInterfaceContainer_Class.h"
#include "ISwInterfaces_Consumer.h"

using namespace StreamWork::SwCore;
using namespace std;

/*! \brief Construsteur */
_SwProvidedInterfaceContainer_Class::_SwProvidedInterfaceContainer_Class(SwInterfaces_Provider_Class * parent) {
    _name=QString();
    _type=QString();
    _isAvailable=false;
    _handle=NULL;
    _parent=parent;
    _consumers.clear();
    _current_consumersIt=_consumers.end();
}
/*! \brief Destrusteur */
_SwProvidedInterfaceContainer_Class::~_SwProvidedInterfaceContainer_Class(){
    QList<__KeyConsumption>::iterator consumersIt;
    QList<__KeyConsumption>::iterator consumersItd;

    consumersIt=_consumers.begin();
    consumersItd=consumersIt;
    while (consumersIt!=_consumers.end()) {
        consumersIt++;
        (*consumersItd)._consumer->DetachProvider((*consumersItd)._cinterface);
        consumersItd=consumersIt;
    }
    _consumers.clear();
}
/*! \brief Definit les caracteristiques de l'interfaces */
void _SwProvidedInterfaceContainer_Class::Define(QString name,QString type){
    _name=name;
    _type=type;
}
/*! \brief Acces au type de l'interface */
QString _SwProvidedInterfaceContainer_Class::GetType() {
    return _type;
}
/*! \brief Definition de l'interface */
void _SwProvidedInterfaceContainer_Class::SetInterface(void * handle){
    ChangeAvailability(false);    
    _handle=handle;
    ChangeAvailability(true);
}
/*! \brief Changer la disponibilite */
void _SwProvidedInterfaceContainer_Class::ChangeAvailability(bool is_available){
    QList<__KeyConsumption>::const_iterator consumersIt;

    if (is_available==_isAvailable)
        return;
    for (consumersIt=_consumers.begin();consumersIt!=_consumers.end();consumersIt++) {
        if (is_available) {
			(*consumersIt)._consumer->ProvideInterface((*consumersIt)._cinterface,_handle);
        } else {
            (*consumersIt)._consumer->UnprovideInterface((*consumersIt)._cinterface);
        }
    }
    _isAvailable=is_available;
}
/*! \brief Revoie la disponibilite */
bool _SwProvidedInterfaceContainer_Class::IsAvailable(){
    return _isAvailable;
}
/*! \brief Enregistre une interface consommatrice */
void _SwProvidedInterfaceContainer_Class::RegisterConsumedInterface(ISwInterfaces_Consumer * consumer,QString cinterface)  throw(SwException){
    int index;

    index=_consumers.indexOf(__KeyConsumption(consumer,cinterface));
    if (index!=-1) {
        QString msg=QString("Interface %1 to consume of component %2 already registered\n for interface %3 of component %4").arg(cinterface).arg(consumer->GetHostComponent()->GetName()).arg(_name).arg(_parent->GetHostComponent()->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    //Insertion
    _consumers.push_back(__KeyConsumption(consumer,cinterface));
    //Fourniture de l'interface au consommateur si disponible
    if (_isAvailable) consumer->ProvideInterface(cinterface,_handle);
}
/*! \brief Desenregistre une interface consommatrice */
void _SwProvidedInterfaceContainer_Class::UnregisterConsumedInterface(ISwInterfaces_Consumer * consumer,QString cinterface){
    int index;

    index=_consumers.indexOf(__KeyConsumption(consumer,cinterface));
    if (index==-1) {
        QString msg=QString("Failed to unregister consumed interface %1 of component %2 because it's undefined\n for interface %3 of component %4").arg(cinterface).arg(consumer->GetHostComponent()->GetName()).arg(_name).arg(_parent->GetHostComponent()->GetName());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    if (_isAvailable) _consumers[index]._consumer->UnprovideInterface(cinterface);
    _consumers.removeAt(index);
}
/*! \brief Acces a la premier consommateur et son nom */
ISwInterfaces_Consumer * _SwProvidedInterfaceContainer_Class::GetFirstConsumer(QString * pt_cinterfacename) {
    ISwInterfaces_Consumer * consumer;

    _current_consumersIt=_consumers.begin();
    if (_current_consumersIt==_consumers.end()) {
        return NULL;
    }
    consumer=(*_current_consumersIt)._consumer;
    if (pt_cinterfacename!=NULL) *pt_cinterfacename=(*_current_consumersIt)._cinterface;
    _current_consumersIt++;
    return consumer;
}
/*! \brief Acces a la consommateur suivant et son nom */
ISwInterfaces_Consumer * _SwProvidedInterfaceContainer_Class::GetNextConsumer(QString * pt_cinterfacename){
    ISwInterfaces_Consumer * consumer;

    if (_current_consumersIt==_consumers.end()) {
        return NULL;
    }
    consumer=(*_current_consumersIt)._consumer;
    if (pt_cinterfacename!=NULL) *pt_cinterfacename=(*_current_consumersIt)._cinterface;
    _current_consumersIt++;
    return consumer;
}
