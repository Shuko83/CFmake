/**
@file _SwGuiVisualUpdater.h
@brief Update pour streamwork
@author F.Bighelli
 */

#ifndef __SWGUIVISUALUPDATER_H
#define __SWGUIVISUALUPDATER_H

#include <SwComponent_Class.h>
#include <SwInterfaces_Consumer_Class.h>
#include <ISwHttpServer.h>
#include <ISwHttpPart.h>
#include <QtCore>
#include <QtGui>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/**
@class _SwGuiVisualUpdater
@brief Update pour streamwork
*/
class _SwGuiVisualUpdater :public SwComponent_Class,
    public ISwInterfaces_ConsumerObserver,
    public ISwHttpPart
{
    Q_OBJECT
protected:
    
	//--------------------------------------------------------------
	//Services
	//--------------------------------------------------------------
    /** @brief service de consommation d'interface */
    SwInterfaces_Consumer_Class * _consumer_service;
	//--------------------------------------------------------------
	// Handle serveur http
	//--------------------------------------------------------------
    ISwHttpServer * _handleHttpServer;
    QByteArray _updateHtmlBody;
    QByteArray _updateLockHtmlBody;
    QByteArray _updateInProcessHtmlBody;
	//--------------------------------------------------------------
	// download & update info
	//--------------------------------------------------------------
    /** @brief repertoire de mise a jour */
    QString _updateDirectory;
    /** @brief icone d'info */
    QSystemTrayIcon * _trayIcon;
    /** @brief timer pour icone d'info */
    QTimer * _trayIconTimer;
    /** @brief compteur image tray icon */
    int _trayIconCpt;
	//--------------------------------------------------------------
	// download data
	//--------------------------------------------------------------
    /** @brief indique que le lock est actif */
    bool _isLocked;    
    /** @brief file list */
    QStringList _loadedFiles;
	//--------------------------------------------------------------
	// update data
	//--------------------------------------------------------------
    /** @brief indique que le lock pour la mise a jour est actif */
    bool _isUpdateLocked;    
    /** @brief timer pour check de mise a jour */
    QTimer * _updateTimer;

public:
	/** @brief Constructor */
	_SwGuiVisualUpdater();
    /** @brief Destructeur */
    virtual ~_SwGuiVisualUpdater();
    /** 
     * @brief Initialisation des ressources
     * @note tous les services du composants doivent être déclarés dans cette methodes
     */
    virtual void InitializeResources() throw(SwException);
    //---------------------------------------------------------------------
    // Interface ISwInterfaces_ConsumerObserver
    //---------------------------------------------------------------------
	/** @brief Avant changement de la disponibilité de l'interface */
	virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
	/** @brief Apres changement de la disponibilité de l'interface */
	virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host);
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
public slots:
    /*! @breif  sur mise a jour du tray icon */
    void onUpdateTrayIcon();
    /*! @breif  sur update a faire*/
    void onUpdate();
private:
    /*! @brief lock */
    void lockOn();
    /*! @brief unlock */
    void lockOff();
    /*! @brief decode des donnees au format multipart/form-data */
    void decodeData(QByteArray * data);
    /*! @brief decode des donnees au format form-data */
    void decodePart(QByteArray * data);
    /*! @brief decode des donnees d'entete au format form-data*/
    void extractPartHeader(QMap<QString,QString> * headerData,QString header);
    /*! @brief Sauvegarde d'un fichier recu*/
    void saveFile(QString filename,QString contentType,QByteArray & realData);
    /*! @brief build response */
    QString buildResponse();
    /*! @brief envoie du status */
    void sendStatus(QHttpRequestHeader * request,QIODevice *device);
    /*! @brief construction du status */
    QString buildStatus();
};

#endif
