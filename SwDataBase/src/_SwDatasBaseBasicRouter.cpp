/*!
 \file _SwDatasBaseBasicRouter.cpp
 \brief Implementation of the Class _SwDatasBaseBasicRouter generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QSet>
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "_SwDatasBaseBasicRouter.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwDatasBaseBasicRouter::_SwDatasBaseBasicRouter(): SwComponent_Class(){
    QSet<SwUUID> pin_types;
    QSet<SwUUID>::iterator it;

    _pins_service=NULL;
    _properties_service=NULL;
    _pins_nb=0;
    pin_types=SW_FACTORIES.GetDataList();
    int i=0;
    for (it=pin_types.begin();it!=pin_types.end();it++) {
        _pin_type.AddKey(i,SW_FACTORIES.GetDataTypeNameFromDataTypeId(*it));
        i++;
    }
    _pin_type.FromInt(0);
}
/*! \brief Destructeur */
_SwDatasBaseBasicRouter::~_SwDatasBaseBasicRouter(){
    //Desenregistrement des services
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());

    //Destruction des services
    delete _pins_service;
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwDatasBaseBasicRouter::InitializeResources() throw(SwException) {
    //Creation des service
    _pins_service=new SwPins_Manager_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_pins_service);

    //Gestion du nombre de pins
    _pins_nb_property=_properties_service->CreateProperty<uint>("nb_pins");
    if (_pins_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_pins property\n"));
    }
    _pins_nb_property->SetDescription("Define how many pins this component has");
    _pins_nb_property->SetValue(QVariant(_pins_nb));
    _pins_nb_property->GetOnChangeSignal().iconnect(*this,&_SwDatasBaseBasicRouter::OnPropertyChange);

    //Pin Type
    _pin_type_property=_properties_service->CreateProperty<SwEnum>("pin_type");
    if (_pin_type_property==NULL) {
    	        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register pin_type property\n"));
    }
    _pin_type_property->SetDescription("Define the pins type");
    QVariant _pin_type_variant;
    _pin_type_variant.setValue(_pin_type);
    _pin_type_property->SetValue(_pin_type_variant);
    _pin_type_property->GetOnChangeSignal().iconnect(*this,&_SwDatasBaseBasicRouter::OnPropertyChange);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwDatasBaseBasicRouter done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwDatasBaseBasicRouter::OnPropertyChange(ISwProperty * property) {
    if (property==_pins_nb_property) {
        //Si le nombre de pin n' pas changer on ne fait rien
        uint new_pins_nb=_pins_nb_property->GetValue().toUInt();
        if (_pins_nb==new_pins_nb) return;
        //Changement du nombre de pins
        if (_pins_list.count()<(int)new_pins_nb) {
            //Creation de nouveaux pins
            for(int i=_pins_nb;i<(int)new_pins_nb;i++) {
                SwPin * pin=new SwPin(_pins_service,QString("Pin_%1").arg(i),_pin_type.ToString());
                pin->RegisterListener(this);
                _pins_list.push_back(pin);
                _pins_service->RegisterPin(pin);
            }
        } else {
            //Suppression de pins existants
            for(int i=new_pins_nb;i<(int)_pins_nb;i++) {
                SwPin * pin=_pins_list.back();
                _pins_list.pop_back();
                _pins_service->UnregisterPin(pin);
                delete pin;
            }
        }
        //Mise a jour du nombre de pins
        _pins_nb=new_pins_nb;
    }
    if (property==_pin_type_property) {
       SwEnum _new_pin_type=_pin_type_property->GetValue().value<SwEnum>();
       //Si le type n'a pas changer, on ne fait rien
       if (_new_pin_type==_pin_type ) return;
       if (_pins_nb==0) {
            _pin_type=_new_pin_type;
            return;
       }
       //On detruit tout les pin existants
       while (_pins_list.count()>0) {
                SwPin * pin=_pins_list.back();
                _pins_list.pop_back();
                _pins_service->UnregisterPin(pin);
                delete pin;
       }
       //On reconstruit tout les pins
       _pin_type=_new_pin_type;
       for(int i=0;i<(int)_pins_nb;i++) {
            SwPin * pin=new SwPin(_pins_service,QString("Pin_%1").arg(i),_pin_type.ToString());
            pin->RegisterListener(this);
            _pins_list.push_back(pin);
            _pins_service->RegisterPin(pin);
        }
    }
}
//----------------------------------------------------
// Interface ISwPin_Listener
//----------------------------------------------------
/*! \brief Callback sur les changements de propriétés*/
/*! \brief Sur reception d'une donnée*/
void _SwDatasBaseBasicRouter::OnReceiveData(SwPin * src,SwData_Class * data) {
    for (int i=0;i<_pins_list.count();i++) {
        if (_pins_list[i]!=src && _pins_list[i]!=NULL) {
            _pins_list[i]->SendData(data);
        }
    }

}
