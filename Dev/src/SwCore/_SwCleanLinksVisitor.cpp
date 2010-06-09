/**
@file _SwCleanLinksVisitor.cpp
@brief visitor supprimant les liens en modules lors de leur destruction
@author F.Bighelli
 */
#include <Qt>
#include "_SwCleanLinksVisitor.h"
#include "SwComponent_Class.h" 
#include "ISwInterfaces_Consumer.h" 
#include "ISwInterfaces_Provider.h"
#include "ISwPins_Manager.h" 

using namespace StreamWork::SwCore;

/** @brief Constructor */
_SwCleanLinksVisitor::_SwCleanLinksVisitor() {
	//A Completer
}
/*! \brief methode de visite */
void _SwCleanLinksVisitor::Visit(SwComponent_Class * component) {
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        QList<QString> list_interfaces;
        QString result=iconsumer->GetFirstInterface(0,0,0);
        while(!result.isNull() && !result.isEmpty()) {
            list_interfaces.push_back(result);
            result=iconsumer->GetNextInterface(0,0,0);
        }
        for(int i=0;i<list_interfaces.count();i++) {
            iconsumer->DetachProvider(list_interfaces.at(i));
        }

    }
    ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pinManager!=0) {
	    QList<SwPin *> list_pin=pinManager->GetPinList();
        for(int i=0;i<list_pin.count();i++) {
            SwPin * pin=list_pin.at(i);
            if (pin->GetConnected()!=0) {
                pinManager->DisconnectPin(pin->GetName());
            }
        }
    }
}
