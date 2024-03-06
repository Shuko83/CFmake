/*!
 \file _SwModelExportedInterfaceC.h
 \brief Class implementant une entite exportable interface consommée
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedInterfaceC_H
#define __SwModelExportedInterfaceC_H

#include <QString>
#include <QStringList>
#include "Component/Base/SwComponent_Class.h"
#include "_SwModelExportedEntity.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Interfaces/ISwInterfaces_ConsumerObserver.h"
#include "Tools/Signal/LibIndeSig.h"

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedInterfaceC 
	\brief  Class implementant une entite exportable interface consommée
*/
class _SwModelExportedInterfaceC : public _SwModelExportedEntity, public ISwInterfaces_ServicesListener, public ISwInterfaces_ConsumerObserver{
	public:
		/*! \brief Consommateur de l'hote*/
		ISwInterfaces_Consumer * _host_consumer;
		/*! \brief Fournisseur interne*/
		SwInterfaces_Provider_Class * _internal_provider;
		/*! \brief Consommateur externe*/
		SwInterfaces_Consumer_Class * _external_consumer;
		/*! \brief Pointer de stockage*/
        void * _handle;
        /*! \brief Indique que l'interface est crée*/
        bool _interface_created;
        /*! brief last path */
        QStringList _lastPaths;
        /*! last interface */
        QStringList _lastInterfaces;
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
		_SwModelExportedInterfaceC();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedInterfaceC();
        /*! \brief acces au path des consommateurs */
        QStringList getConsumerPaths();
        /*! \brief acces au nom des l'interfaces des consommateurs */
        QStringList getInterfaceNames();
        /*! \brief connectTo a des consommateurs*/
        void connectInterfaceTo(QStringList & paths,QStringList & names);
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
