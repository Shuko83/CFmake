/*!
 \file _SwGuiCompHttpServer.h
 \brief Implementation of the Class _SwGuiCompHttpServer gerant un serveur HTTP simple
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwGuiCompHttpServer_H
#define __SwGuiCompHttpServer_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */
#include <QMap>
#include <QTcpServer>
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwProperties_Class.h>
#include <ISwProperty.h>
#include "ISwExecutable_Service.h"
#include "ISwHttpServer.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwExecution;

/*!
	\class _SwGuiCompHttpServer 
	\brief _SwGuiCompHttpServer gerant un serveur HTTP simple
*/
class _SwGuiCompHttpServer : public SwComponent_Class, 
                                public ISwHttpServer,
                                public StreamWork::SwExecution::ISwExecutable_Service,
                                public ISwHttpPart
{
    Q_OBJECT
protected:
    /* @brief service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
    /* @brief service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* @brief port du serveur */
    uint _portServer;
    /* @brief propriété port du serveur */
    ISwProperty * _portServer_property;
    /* @brief Serveur */
    QTcpServer * _server;
    /* @brief liste des parts */
    QSet<ISwHttpPart *> _parts;
    /* @brief isStarted */
    bool _isStarted;
public:
    /*! \brief Constructeur */
    _SwGuiCompHttpServer();
    /*! \brief Destructeur */
    virtual ~_SwGuiCompHttpServer();

    /*! \brief Initialisation des ressources
    \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
     /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //---------------------------------------------------------------------
    // Interface ISwHttpServer
    //---------------------------------------------------------------------
    /*! \brief Permet d'enregistrer une partie de serveur http */
	virtual void registerHttpPart(ISwHttpPart *part);
    /*! \brief Permet de desenregistrer une partie de serveur http */
	virtual void unregisterHttpPart(ISwHttpPart *part);
    //----------------------------------------------------
    // Interface ISwExecutable_Service
    //----------------------------------------------------
	/** @brief Initialisation */
	void Initialize(double start_time,StreamWork::SwExecution::ISwExecution_Service * executor) throw (StreamWork::SwCore::SwException);            
	/** @brief Demarrage */
	void Start(double current_time) throw (StreamWork::SwCore::SwException);            
	/** @brief Execution */
	void Execute(double current_time,bool is_first_call) throw (StreamWork::SwCore::SwException);            
	/** @brief Execution */
	void Stop(double current_time);            
	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------            
	/** @brief Est appele uniquement par le service manager aupres duquel le service est enregistr?
	lorsque ce premier se detruit ou une operation de desenregistrement du service est r?alis?e*/
	void Liberate();  
	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/** @brief acces a son composant hote */
	StreamWork::SwCore::SwComponent_Class * GetHostComponent();            
	//---------------------------------------------------------------------
	// Interface ISwHttpPart
	//---------------------------------------------------------------------            
    /*! @brief Renvoie le path de la partie  */
	virtual QString  getPath();
    /*! @brief process de la requete */
    virtual void processRequest(QHttpRequestHeader * request,
                                QMap<QString,QString> * parameters,
                                QByteArray * body,
                                QIODevice * device);
protected slots:
	//---------------------------------------------------------------------
	// Interface Reception d'une requete HTTP
	//---------------------------------------------------------------------
    void httpRequestReceived();

};
#endif 
