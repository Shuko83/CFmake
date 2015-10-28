/**
@file ModelCreatorHelper.cpp
@brief Boite a outil pour la creation de model
@author F.Bighelli
 */

#include "ModelCreatorHelper.h"
#include "ISwPins_Manager.h" 

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

ModelCreatorHelper::ConsumerKey* ModelCreatorHelper::addConsumerLink(ISwInterfaces_Consumer * consumer, QString c_interface_name, QString m_interface_name, ISwInterfaces_Provider * provider, QString p_interface_name)
{
	ConsumerKey * test = new ConsumerKey(consumer, c_interface_name);
	consumerLinks.insert(test, new ConsumerItem(consumer, c_interface_name, m_interface_name, provider, p_interface_name));
	return test;
}

//-----------------------------------------------------------------------
void ModelCreatorHelper::removeConsumerLink(ModelCreatorHelper::ConsumerKey* test)
{
	consumerLinks.remove(test);
}

void ModelCreatorHelper::addConnectorLink(SwPin * source,SwPin * rtarget,QString m_connecteur) {
    connectorLinks.push_back(new ConnectorItem(source,rtarget,m_connecteur));
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

    //Pour les connectors: deconnexion
    foreach(ConnectorItem * item,connectorLinks) {
        item->source->GetManager()->DisconnectPin(item->source->GetName());
    }

    //Pour les providers : connexion au model host
	//Si même provider avec même interface on le fait qu'une fois
	QMap<ISwInterfaces_Provider *, QStringList> allreadyDone;
	ISwInterfaces_Consumer *consumer_handle = dynamic_cast<ISwInterfaces_Consumer *>(model_host->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
	if ( consumer_handle != 0 )
	{
		itp = providerLinks.begin();
		while ( itp != providerLinks.end() )
		{
			ProviderItem * item = itp.value();
			if ( !allreadyDone.contains(item->provider) || !allreadyDone[item->provider].contains(item->m_interface_name) )
			{
				allreadyDone[item->provider].append(item->m_interface_name);
				consumer_handle->AttachProvider(item->provider, item->m_interface_name, item->p_interface_name);
			}

			int nb = providerLinks.count(itp.key());
			while ( nb > 0 )
			{
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
    //Pour les connectors: connexion au model host
    ISwPins_Manager *pins_manager_handle=dynamic_cast<ISwPins_Manager *>(model_host->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pins_manager_handle!=0) {
        foreach(ConnectorItem * item,connectorLinks) {
            item->source->GetManager()->ConnectRemotePinToLocalPin(item->source->GetName(),item->m_connecteur,pins_manager_handle);
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

    //Pour les connectors: connexion au model host
    ISwPins_Manager *pins_manager_handle=dynamic_cast<ISwPins_Manager *>(model->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pins_manager_handle!=0) {
        foreach(ConnectorItem * item,connectorLinks) {
            item->rtarget->GetManager()->ConnectRemotePinToLocalPin(item->rtarget->GetName(),item->m_connecteur,pins_manager_handle);
        }
    }

}
