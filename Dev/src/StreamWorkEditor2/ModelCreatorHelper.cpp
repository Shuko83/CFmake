/**
@file ModelCreatorHelper.cpp
@brief Boite a outil pour la creation de model
@author F.Bighelli
 */

#include "ModelCreatorHelper.h"
 

/** @brief Constructor */
ModelCreatorHelper::ModelCreatorHelper() {
	//TO DO creation
}
/** @brief Destructor */
ModelCreatorHelper::~ModelCreatorHelper() {
  //TO DO destruction
}
void ModelCreatorHelper::addProviderLink(ISwInterfaces_Provider * provider,QString p_interface_name,QString m_interface_name,ISwInterfaces_Consumer * consumer,QString c_interface_name) {
    providerLinks.insert(new ProviderKey(provider,p_interface_name), new ProviderItem(provider,p_interface_name,m_interface_name,consumer,c_interface_name));
}
void ModelCreatorHelper::addConsumerLink(ISwInterfaces_Consumer * consumer,QString c_interface_name,QString m_interface_name,ISwInterfaces_Provider * provider,QString p_interface_name) {
    consumerLinks.insert(new ConsumerKey(consumer,c_interface_name), new ConsumerItem(consumer,c_interface_name,m_interface_name,provider,p_interface_name));
}
void ModelCreatorHelper::connectInternalToModelHost(SwComponent_Class * model_host) {
    //Pour les providers : deconnection
    QMultiMap<ProviderKey *,ProviderItem *>::iterator itp=providerLinks.begin();
    while (itp!=providerLinks.end()) {
        ProviderItem * item=itp.value();
        item->consumer->DetachProvider(item->c_interface_name);
        itp++;
    }
    //Pour les consumers : deconnexion
    QMap<ConsumerKey *,ConsumerItem *>::iterator itc=consumerLinks.begin();
    while (itc!=consumerLinks.end()) {
        ConsumerItem * item=itc.value();
        item->consumer->DetachProvider(item->c_interface_name);
        itc++;
    }

    //Pour les providers : connexion au model host
    ISwInterfaces_Consumer *consumer_handle=dynamic_cast<ISwInterfaces_Consumer *>(model_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (consumer_handle!=0) {
        itp=providerLinks.begin();
        while (itp!=providerLinks.end()) {
            ProviderItem * item=itp.value();
            consumer_handle->AttachProvider(item->provider,item->m_interface_name,item->p_interface_name);
            int nb=providerLinks.count(itp.key());
            while (nb>0) {
                itp++;
                nb--;
            }
        }
    }


    //Pour les consumers : connexion au model host
    ISwInterfaces_Provider *provider_handle=dynamic_cast<ISwInterfaces_Provider *>(model_host->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (provider_handle!=0) {
        itc=consumerLinks.begin();
        while (itc!=consumerLinks.end()) {
            ConsumerItem * item=itc.value();
            item->consumer->AttachProvider(provider_handle,item->c_interface_name,item->m_interface_name+"_");
            itc++;
        }
    }
    
}
void ModelCreatorHelper::connectModelToExternal(SwComponent_Class * model) {
    //Pour les providers : connexion du model a l'exterieur
    ISwInterfaces_Provider *provider_handle=dynamic_cast<ISwInterfaces_Provider *>(model->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (provider_handle!=0) {
        QMultiMap<ProviderKey *,ProviderItem *>::iterator itp=providerLinks.begin();
        while (itp!=providerLinks.end()) {
            ProviderItem * item=itp.value();
            item->consumer->AttachProvider(provider_handle,item->c_interface_name,item->m_interface_name+"_");
            itp++;
        }

    }

    //Pour les consumers : connexion du model a l'exterieur
    ISwInterfaces_Consumer *consumer_handle=dynamic_cast<ISwInterfaces_Consumer *>(model->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (consumer_handle!=0) {
        QMap<ConsumerKey *,ConsumerItem *>::iterator itc=consumerLinks.begin();
        while (itc!=consumerLinks.end()) {
            ConsumerItem * item=itc.value();
            consumer_handle->AttachProvider(item->provider,item->m_interface_name,item->p_interface_name);
            itc++;
        }
    }
}
