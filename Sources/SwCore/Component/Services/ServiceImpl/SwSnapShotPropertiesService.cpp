/**
@file SwSnapShotPropertiesService.cpp
@brief Service de backup de property (implementation)
@author Big
 */

#include "SwSnapShotPropertiesService.h"
#include "ISwProperty.h"

using namespace StreamWork::SwCore;

/** @brief Constructor */
SwSnapShotPropertiesService::SwSnapShotPropertiesService() {
	//TO DO creation
}
/** @brief Destructor */
SwSnapShotPropertiesService::~SwSnapShotPropertiesService() {
    QList<PropDesc *> values=descProperties.values();
	for (PropDesc * pdesc : values) {
        delete pdesc;
    }
    descProperties.clear();
}
//test if property exists
bool SwSnapShotPropertiesService::exist(QString property_name) {
    PropDesc * pdesc=descProperties.value(property_name,0);
    if (pdesc!=0) {
        return true;
    }
    return false;
}
//remove property from snapshot
void SwSnapShotPropertiesService::removeFromSnapShot(QString property_name) {
    PropDesc * pdesc=descProperties.value(property_name,0);
    if (pdesc!=0) {
        delete pdesc;
        descProperties.remove(property_name);
    }
}

/** @brief doSnapShot */
void SwSnapShotPropertiesService::doSnapShot(ISwProperties * properties){
    QList<PropDesc *> values=descProperties.values();
	for (PropDesc * pdesc : values) {
        delete pdesc;
    }
    descProperties.clear();
    QList<ISwProperty *> plist=properties->GetProperties();
	for (ISwProperty * p : plist) {
        if (p->IsEditable()) {
            PropDesc * pdesc=new PropDesc();
            pdesc->hasChanged=p->HasChanged();
            pdesc->value=p->GetValue();
            descProperties.insert(p->GetRealName(),pdesc);
        }
        
    }
}
//get value
QVariant SwSnapShotPropertiesService::getValue(QString property_name){
    PropDesc * pdesc=descProperties.value(property_name,0);
    if (pdesc!=0) {
        return pdesc->value;
    }
    return QVariant();
}
//has changed
bool SwSnapShotPropertiesService::getHasChanged(QString property_name) {
    PropDesc * pdesc=descProperties.value(property_name,0);
    if (pdesc!=0) {
        return pdesc->hasChanged;
    }
    return false;

}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void SwSnapShotPropertiesService::Liberate() {

}

SwSnapShotPropertiesVisitor::SwSnapShotPropertiesVisitor() {

}
/*! \brief methode de visite */
void SwSnapShotPropertiesVisitor::Visit(SwComponent_Class *component) {
    ISwProperties * properties=dynamic_cast<ISwProperties *>(component->QueryService(CG_SW_SERVICE_PROPERTIES));
    SwSnapShotPropertiesService * snapshotService=dynamic_cast<SwSnapShotPropertiesService *>(component->QueryService(CG_SW_SNAPSHOPPROPERTY_SERVICE));
    if (properties!=0) {
        if (snapshotService==0) {
            snapshotService=new SwSnapShotPropertiesService();
            component->RegisterService(snapshotService);
        }
        snapshotService->doSnapShot(properties);
    }
}
