/**
@file StreamControler.h
@brief Controleur Stream <-> Scene
@author F.Bighelli
 */

#ifndef _STREAMCONTROLER_H
#define _STREAMCONTROLER_H

#include <QDomDocument>

#include "StreamScene.h"
#include "StreamView.h"
#include "ComponentGraphicItem.h"
#include "PropertiesWidget.h"
#include "IAnchorable.h"
#include "Component/Base/SwComponent_Class.h"
#include "Main/Connexion/ISwInterfaces_ServicesListener.h"
#include "Main/Connexion/ISwPins_ManagerListener.h"
#include "ISelectionObserver.h"
#include "IStreamControlerObserver.h"
/**
@class StreamControler
@brief Controleur Stream <-> Scene
*/
class StreamControler : public QObject , 
                        public StreamWork::SwCore::ISwInterfaces_ServicesListener,
                        public StreamWork::SwCore::ISwPins_ManagerListener {
    Q_OBJECT
public:
	/** @brief Constructor */
	StreamControler(PropertiesWidget * propertiesWidget);
	/** @brief Destructor */
	~StreamControler();
    /** @brief Load stream */
    void loadStream(QString streamFileName);
    /** @brief Load existing stream */
    void loadExistingStream(QString streamFileName,StreamWork::SwCore::SwComponent_Class * aStream);
	/** @brief Save stream */
	void saveStream();
	/** @brief Save stream as */
	void saveStreamAs(QString streamFileName);

	static void SaveStream(QFile& file);

    /** @brief Stream file name access */
	QString getStreamFileName();
    /** @brief Scene access */
	StreamScene *getScene();
    /** @brief definition de la vue */
    void setView(StreamView * view);
    /** @brief Scene access */
	StreamView *getView();
    /** @brief renvoie l'item racine */
    StreamWork::SwCore::SwComponent_Class * getRootItem();
    /** @brief force la selection d'un item */
    void setSelection(StreamWork::SwCore::SwComponent_Class * component);
    //------------------------------------------------------------------------------
    // Evenements de la vue 
    //------------------------------------------------------------------------------
public:
    /** @brief demande de menu contextuel a une position donnée */
    QMenu * getContextualMenu(const QPointF & p);
    /** @brief ajout d'un composant a une position donné */
    void addComponent(const QString & pluginName, const QString & name,const QPointF & p);
    //------------------------------------------------------------------------------
    // Evenements Composant
    //------------------------------------------------------------------------------
    /** @brief demande de visualisation des propriétés */
    void showProperties();

    //------------------------------------------------------------------------------
    // Evenements de la scene 
    //------------------------------------------------------------------------------
public slots:
    /** @brief changement de la selection */
    void selectionChanged();
    /** @brief changement du stream */
    void streamControlerChanged();
    /** @brief sur demande de connection */
    void onLinkConnectors(ConnectorGraphicItem * src,ConnectorGraphicItem * target);
    /** @brief sur sceneRectChanged de la scene */
    void onSceneRectChanged(const QRectF & rect);
public:
    /** @brief Renvoie le dernier composant ajouté */
    ComponentGraphicItem * getLastAddedComponent();
    //--------------------------------------------------------------------------
    // Evenements du modele 
    //--------------------------------------------------------------------------
    /** @brief evenement du stream : ajout d'un enfant */
    void childAdded(StreamWork::SwCore::SwComponent_Class * parent,
                    StreamWork::SwCore::SwComponent_Class *child);
    /** @brief evenement du stream : suppression d'un enfant */
    void childRemoved(StreamWork::SwCore::SwComponent_Class * parent,
                    StreamWork::SwCore::SwComponent_Class *child);
    /** @brief evenement du stream : renommage d'un composant */
    void componentNameChanged(StreamWork::SwCore::SwComponent_Class * component);
    /*! \brief Sur ajout d'une nouvelle interface */
    void OnAddInterface(StreamWork::SwCore::ISwInterfaces_Service * source,QString interface_name);            
    /*! \brief Sur suppression d'une  interface */
    void OnRemoveInterface(StreamWork::SwCore::ISwInterfaces_Service * source,QString interface_name);            
    /*! \brief Sur connection d'une interface */
    void OnConnectInterface(StreamWork::SwCore::ISwInterfaces_Service * source,QString interface_name,
                            StreamWork::SwCore::ISwInterfaces_Service * remote_source,QString remote_interface_name);            
    /*! \brief Sur deconnection d'une interface */
    void OnDisconnectInterface(StreamWork::SwCore::ISwInterfaces_Service * source,QString interface_name,
                               StreamWork::SwCore::ISwInterfaces_Service * remote_source,QString remote_interface_name);            
    /*! \brief Sur ajout d'un nouveau pin*/
    void OnAddPin(StreamWork::SwCore::SwPin *pin);            
    /*! \brief Sur suppression d'un pin existant*/
    void OnRemovePin(StreamWork::SwCore::SwPin *pin);            
    /*! \brief Sur connexion d'un pin*/
    void OnConnectPin(StreamWork::SwCore::SwPin * local_pin,StreamWork::SwCore::SwPin * remote_pin);            
    /*! \brief Sur deconnexion d'un pin*/
    void OnDisconnectPin(StreamWork::SwCore::SwPin * local_pin,StreamWork::SwCore::SwPin * remote_pin);  
    /*! \brief Create model from selection*/
    void createModelFromSelection(QList<StreamWork::SwCore::SwComponent_Class *> & components,QString modelName);
    //--------------------------------------------------------------------------
    //  gestion des observers de selection
    //--------------------------------------------------------------------------
    void addSelectionObserver(ISelectionObserver * observer);
    void removeSelectionObserver(ISelectionObserver * observer);
    //--------------------------------------------------------------------------
    //  gestion des observers de changement
    //--------------------------------------------------------------------------
    void addStreamControlerObserver(IStreamControlerObserver * observer);
    void removeStreamControlerObserver(IStreamControlerObserver * observer);
private:
    //--------------------------------------------------------------------------
    // Liaison modele <-> Controler
    //--------------------------------------------------------------------------
    /** @brief connexion des evenements d'un composant au controleur */ 
    void connectToControler(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief connexion des evenements d'un composant au controleur */ 
    void recursiveConnectToControler(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief deconnexion des evenements de tous les composant d'un stream au controleur */ 
    void disconnectFromControler(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief deconnexion des evenements d'un composant au controleur */ 
    void recursiveDisconnectToControler(StreamWork::SwCore::SwComponent_Class * component);

    //--------------------------------------------------------------------------
    // Gestion persistence des données
    //--------------------------------------------------------------------------
    /** @brief sauvegarde des données visuelles */
    void saveVisualData(QDomDocument & doc);
	void saveVisualData(QXmlStreamWriter & writer);
	/** @brief sauvegarde des données visuelles from selection*/
	void saveVisualDataFromSelection(QDomDocument & doc, QList<StreamWork::SwCore::SwComponent_Class *> & components);
	void saveVisualDataFromSelection(QXmlStreamWriter & writer, QList<StreamWork::SwCore::SwComponent_Class *> & components);
    /** @brief sauvegarde d'un item */
    void saveVisualItem(ComponentGraphicItem * item,QDomDocument & doc,QDomElement &parentNode);
	void saveVisualItem(ComponentGraphicItem * item, QXmlStreamWriter & writer);
	/** @brief chargement des données visuelles */
    void loadVisualData(QDomDocument & doc);
    /** @brief chargement d'un item */
    void loadVisualItem(QDomDocument & doc,QDomElement &node,StreamWork::SwCore::SwComponent_Class * parentComponent,QGraphicsItem * parentItem);
    //--------------------------------------------------------------------------
    // Tools
    //--------------------------------------------------------------------------
    /** @brief buildComponents */
    ComponentGraphicItem *  buildComponents(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief buildConnectors */
    void buildConnectors(StreamWork::SwCore::SwComponent_Class * component);
    /** @brief construction des liens */
    void buildLinks();
    /** @brief destruction des liens */
    void destroyLinks();

	//--------------------------------------------------------------------------
	// Signature
	//--------------------------------------------------------------------------
	/** @brief Récupère la signature du stream */
	static QString getStreamSignature(QString stream);

	static void writeToken(const QString & streamDesc, QXmlStreamWriter& writer, QString tokenName, bool checkIsWhiteSpace = false);
	static bool isStream(const QString & fileDesc);
private:
	/** @brief Scene */
    StreamScene * _streamScene;
    /** @brief View */
    StreamView * _streamView;
    /** @brief Stream File Name */
    QString _streamFileName;
    /** @brief composant racine */
    StreamWork::SwCore::SwComponent_Class * _rootComponent;
    /** @brief creation position */
    QPointF _creationPosition;
    /** @brief Map de composant vers GraphicsItem */
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *> _mapCompToItem;
    /** @brief Widget de propriété */
    PropertiesWidget * _propertiesWidget;
    /** @brief gestion des observers de selection */
    QList<ISelectionObserver *> _selectionObservers;
    /** @brief gestion des observers de chnagement */
    QList<IStreamControlerObserver *> _streamControlerObservers;
    /** @brief boolean d'activation de l'observation du streamControler */
    bool _enableStreamControlerObservation;
    /** @brief arret de l'execution necessaire lors de l'arret */
    bool _stopExecution;
    /** @brief dernier composant ajoute */
    ComponentGraphicItem * _lastAddedComponent;
    /** @brief is existing stream */
    bool _isExistingStream;

};

#endif
