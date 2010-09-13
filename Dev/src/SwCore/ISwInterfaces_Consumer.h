/*!
\file ISwInterfaces_Consumer.h
\date 12/04/2006
\brief service consommateur d'interfaces
\author  Big
\version 1.0
 */

#ifndef _ISwInterfaces_Consumer_H
#define _ISwInterfaces_Consumer_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
/*
  * INCLUDES LOCAUX
 */
#include "ISwInterfaces_Service.h"
#include "ISwHost.h"
#include "ISwPersistent.h"

//Nom du service
#define CG_SW_SERVICE_INTERFACES_CONSUMER "InterfacesConsumer"

namespace StreamWork 
{
    namespace SwCore
    {
        class ISwInterfaces_Provider;
        /*!
        \class ISwInterfaces_Consumer
        \brief service consommateur d'interfaces
        */
        class ISwInterfaces_Consumer: public ISwInterfaces_Service, public ISwPersistent {
        public:
	        /*! \brief Enregistre une interface a fournir */
            template<typename T> inline void RegisterConsumedInterface(QString cinterface_name,T ** handle_handle_interface) {
                T * named_handle;
				named_handle=0;
                RegisterConsumedInterfaceWithType(cinterface_name,QString(typeid(named_handle).name()),(void **)handle_handle_interface);
            }
	        /*! \brief Enregistre une interface a fournir (utliser de préférence la methode précédente)*/
            virtual void RegisterConsumedInterfaceWithType(QString cinterface_name,QString cinterface_type,void ** handle_handle_interface)=0;
	        /*! \brief Desenregistre une interface fournie */
            virtual void UnregisterConsumedInterface(QString cinterface_name)=0;
	        /*! \brief Permet de savoir si une interface est disponible */
            virtual bool IsInterfaceAvailable(QString cinterface_name)=0;
	        /*! \brief Attache un fournisseur a une interface donnée */
	        virtual void AttachProvider(ISwInterfaces_Provider * provider,QString cinterface_name,QString pinterface_name)=0;
	        /*! \brief Detache un fournisseur d'une interface donnée */
	        virtual void DetachProvider(QString cinterface_name)=0;
	        /*! \brief Fournit l'interface (est appelé par le fournisseur)*/
	        virtual void ProvideInterface(QString cinterface_name,void *)=0;
	        /*! \brief Ne fournit plus l'interface (est appelé par le fournisseur)*/
	        virtual void UnprovideInterface(QString cinterface_name)=0;
	        /*! \brief Permet d'acceder a la premier interface consommé 
            (renvoie aussi le type, le fournisseur et le nom de l'interface fournie)
            Quand la chaine renvoye Null plus d'interface)*/
	        virtual QString GetFirstInterface(QString * nametype,ISwInterfaces_Provider ** pt_provider,QString * pt_pinterface_name)=0;
	        /*! \brief Permet d'acceder a l'interface consommé suivante 
            (renvoie aussi le type, le fournisseur et le nom de l'interface fournie)
            Quand la chaine renvoye Null plus d'interface)*/
	        virtual QString GetNextInterface(QString * nametype,ISwInterfaces_Provider ** pt_provider,QString * pt_pinterface_name)=0;
	        /*! \brief Permet de recuperer le type de l'interface consomme(via typeid)*/
	        virtual QString GetInterfaceType(QString)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_INTERFACES_CONSUMER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}    
        };
    }
}

#endif
