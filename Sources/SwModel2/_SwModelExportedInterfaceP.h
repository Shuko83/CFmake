/*!
 \file _SwModelExportedInterfaceP.h
 \brief Class implementant une entite exportable interface produite
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedInterfaceP_H
#define __SwModelExportedInterfaceP_H

#include <QString>
#include <SwComponent_Class.h>
#include "_SwModelExportedEntity.h"
#include <SwInterfaces_Provider_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <ISwInterfaces_ConsumerObserver.h>
#include <LibIndeSig.h>

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedInterfaceP 
	\brief  Class implementant une entite exportable interface produite
*/
class _SwModelExportedInterfaceP : public _SwModelExportedEntity, public ISwInterfaces_ServicesListener, public ISwInterfaces_ConsumerObserver{
	public:
		/*! \brief Consommateur de l'hote*/
		ISwInterfaces_Provider * _host_provider;
		/*! \brief Consommateur interne*/
		SwInterfaces_Consumer_Class * _internal_consumer;
		/*! \brief Fournisseur externe*/
		SwInterfaces_Provider_Class * _external_provider;
		/*! \brief Pointer de stockage*/
        void * _handle;
        /*! \brief Indique que l'interface est crée*/
        bool _interface_created;
        /*! brief last path */
		QString _lastPath;
        /*! last interface */
        QString _lastInterface;
	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	    /*! \brief Effectue la construction */
		virtual void SpecificBuild();
	    /*! \brief Effectue la destruction */
		virtual void SpecificDestroy();
	public:
		/*! \brief Constructeur */
		_SwModelExportedInterfaceP();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedInterfaceP();
        /*! \brief acces au path du producteur */
        QString getProviderPath();
        /*! \brief acces au nom de l'interface du producteur */
        QString getInterfaceName();
        /*! \brief connectTo a un producteur*/
        void connectInterfaceTo(QString path,QString name);
        //-------------------------------------------------------------------------
        //Interface ISwInterfaces_ServicesListener
        //-------------------------------------------------------------------------
	    /*! \brief Sur ajout d'une nouvelle interface */
	    virtual void OnAddInterface(ISwInterfaces_Service * source,QString interface_name);            
	    /*! \brief Sur suppression d'une  interface */
	    virtual void OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name);            
	    /*! \brief Sur connection d'une interface */
	    virtual void OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name);            
	    /*! \brief Sur deconnection d'une interface */
	    virtual void OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name);            
        //-------------------------------------------------------------------------
        //Interface ISwInterfaces_ConsumerObserver
        //-------------------------------------------------------------------------
	    /*! \brief Avant changement de la disponibilité de l'interface */
	    virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
	    /*! \brief Apres changement de la disponibilité de l'interface */
	    virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);            
		
};

#endif