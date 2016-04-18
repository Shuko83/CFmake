/**
@file ModelCreatorHelper.h
@brief Boite a outil pour la creation de model
@author F.Bighelli
 */

#include <QtCore>
#include <SwComponent_Class.h>
#include <SwPin.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>

#ifndef _MODELCREATORHELPER_H
#define _MODELCREATORHELPER_H


using namespace StreamWork::SwCore;
/**
@class ModelCreatorHelper
@brief Boite a outil pour la creation de model
*/
class ModelCreatorHelper {
public:
    /** @brief Constructor */
    ModelCreatorHelper();
    /** @brief Destructor */
    virtual ~ModelCreatorHelper();

    void addProviderLink(ISwInterfaces_Provider * provider,QString p_interface_name,QString m_interface_name,ISwInterfaces_Consumer * consumer,QString c_interface_name);
	ConsumerKey* addConsumerLink(ISwInterfaces_Consumer * consumer, QString c_interface_name, QString m_interface_name, ISwInterfaces_Provider * provider, QString p_interface_name);
	
	void removeConsumerLink(ConsumerKey*);
    void addConnectorLink(SwPin * source,SwPin * rtarget,QString m_connecteur);
    void connectInternalToModelHost(SwComponent_Class * model_host);
    void connectModelToExternal(SwComponent_Class * model);

private:
    class ProviderKey {
    public:
        ISwInterfaces_Provider * provider;
        QString p_interface_name;
        ProviderKey(ISwInterfaces_Provider * provider,QString p_interface_name) {
            this->provider=provider;
            this->p_interface_name=p_interface_name;
        }
        bool operator<(ProviderKey * other)  {
            if (this->provider!=other->provider) {
                return this->provider<other->provider;
            }
            return this->p_interface_name<other->p_interface_name;
        }
    };

    class ProviderItem : public ProviderKey{
    public:
        QString m_interface_name;
        ISwInterfaces_Consumer * consumer;
        QString c_interface_name;

        ProviderItem(ISwInterfaces_Provider * provider,QString p_interface_name,QString m_interface_name,ISwInterfaces_Consumer * consumer,QString c_interface_name):
        ProviderKey(provider,p_interface_name){
            this->m_interface_name=m_interface_name;
            this->consumer=consumer;
            this->c_interface_name=c_interface_name;
        }
    };
public:
    class ConsumerKey {
    public:
        ISwInterfaces_Consumer * consumer;
        QString c_interface_name;
        ConsumerKey(ISwInterfaces_Consumer * consumer,QString c_interface_name) {
            this->consumer=consumer;
            this->c_interface_name=c_interface_name;
        }
        bool operator<(ConsumerKey * other)  {
            if (this->consumer!=other->consumer) {
                return this->consumer<other->consumer;
            }
            return this->c_interface_name<other->c_interface_name;
        }
    };

    class ConsumerItem : public ConsumerKey{
    public:
        QString m_interface_name;
        ISwInterfaces_Provider * provider;
        QString p_interface_name;

        ConsumerItem(ISwInterfaces_Consumer * consumer,QString c_interface_name,QString m_interface_name,ISwInterfaces_Provider * provider,QString p_interface_name):
        ConsumerKey(consumer,c_interface_name){
            this->m_interface_name=m_interface_name;
            this->provider=provider;
            this->p_interface_name=p_interface_name;
        }
    };

    class ConnectorItem {
    public:
        SwPin * source;
        SwPin * rtarget;
        QString m_connecteur;
        ConnectorItem(SwPin * source,SwPin * rtarget,QString m_connecteur) {
            this->source=source;
            this->rtarget=rtarget;
            this->m_connecteur=m_connecteur;
        }

    };

    QMultiMap<ProviderKey *,ProviderItem *> providerLinks; 
    QMap<ConsumerKey *,ConsumerItem *> consumerLinks; 
    QList<ConnectorItem *> connectorLinks;

};

#endif
