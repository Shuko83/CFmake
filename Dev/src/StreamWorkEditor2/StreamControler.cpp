/**
@file StreamControler.cpp
@brief Controleur Stream <-> Scene
@author F.Bighelli
 */

#include <QDomElement>

#include <SwApplication.h>
#include <SwMacros.h>
#include <SwSaver_Class.h>
#include <SwLoader_Class.h>
#include <ISwInterfaces_Provider.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwPins_Manager.h>
#include <SwPin.h>

#include "StreamControler.h"
#include "MenuManager.h"
#include "WidgetFactory.h"
#include "GraphicsLoaderV1.h"
#include "Arranger.h"
#include "InterestArea.h"

using namespace StreamWork::SwCore;

#define CL_SCENE_NODE "Scene"
#define CL_SCENE_ATT_X "x"
#define CL_SCENE_ATT_Y "y"
#define CL_SCENE_ATT_WIDTH "width"
#define CL_SCENE_ATT_HEIGHT "height"

#define CL_IA_NODE "IA"
#define CL_IA_ATT_X "x"
#define CL_IA_ATT_Y "y"
#define CL_IA_ATT_W "W"
#define CL_IA_ATT_H "H"
#define CL_IA_ATT_COLOR "color"
#define CL_IA_ATT_TEXT "text"

#define CL_CGITEM_NODE "CGItem"
#define CL_CGITEM_ATT_NAME "name"
#define CL_CGITEM_ATT_X "x"
#define CL_CGITEM_ATT_Y "y"
#define CL_CGITEM_ATT_COLOR "color"
#define CL_CGITEM_ATT_TEXT_COLOR "tcolor"
#define CL_CGCNTITEM_NODE "Cnt"
#define CL_CGCNTITEM_ATT_NAME "name"
#define CL_CGCNTITEM_ATT_POS "pos"

/** @brief Constructor */
StreamControler::StreamControler():QObject() {
    _enableStreamControlerObservation=false;
    _streamView=0;
    _creationPosition=QPointF(0.0f,0.0f);
    _rootComponent=SW_APP->CreateNewStream("NoNamed");
    recursiveConnectToControler(_rootComponent);
	/** @brief Scene */
    _streamScene=new StreamScene();
    _streamScene->setSceneRect(0.0,0.0,100.0,100.0); 
    _streamFileName="";
    MenuManager::getInstance()->setControler(this);
    //Ajout widget
    //_streamScene->addApplicativeWidget(WidgetFactory::getInstance()->buildPluginsBankView(true));

    _propertiesWidget=WidgetFactory::getInstance()->buildPropertiesWidget();
    //Connections signaux
    connect(_streamScene,SIGNAL(sceneRectChanged ( const QRectF &)),this,SLOT(onSceneRectChanged ( const QRectF &)));
    connect(_streamScene,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
    connect(_streamScene,SIGNAL(onLinkConnectors(ConnectorGraphicItem * ,ConnectorGraphicItem *)),
            this,SLOT(onLinkConnectors(ConnectorGraphicItem * ,ConnectorGraphicItem *)));
    _enableStreamControlerObservation=true;
}
/** @brief Destructor */
StreamControler::~StreamControler() {
    MenuManager::getInstance()->setControler(0);
    _propertiesWidget->setSelectedGraphicComponent(0);
    if (_streamView!=0) {
        //Deconnexion a la vue
        _streamView->setScene(0);
        _streamView->setControler(0);
    }
    disconnect(_streamScene,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
    disconnect(_streamScene,SIGNAL(sceneRectChanged ( const QRectF &)),this,SLOT(onSceneRectChanged ( const QRectF &)));
    disconnect(_streamScene,SIGNAL(onLinkConnectors(ConnectorGraphicItem * ,ConnectorGraphicItem *)),
               this,SLOT(onLinkConnectors(ConnectorGraphicItem * ,ConnectorGraphicItem *)));
    //Deconnection au modele
    recursiveDisconnectToControler(_rootComponent);
    //Destruction des liens
    destroyLinks();
    //Destruction du streams
    SW_APP->DestroyStream(_rootComponent);
    //Destruction de la scene
    delete _streamScene;
    //Destruction du widget properties
    delete _propertiesWidget;
}
	/** @brief Scene access */
StreamScene * StreamControler::getScene() {
    return _streamScene;
}
/** @brief definition de la vue */
void StreamControler::setView(StreamView * view) {
    _streamView=view;
    _streamView->setScene(_streamScene);
    _streamView->setControler(this);
}
/** @brief Scene access */
StreamView *StreamControler::getView() {
    return _streamView;
}
/** @brief Load stream */
void StreamControler::loadStream(QString streamFileName){
    _streamFileName=streamFileName;
    _enableStreamControlerObservation=false;
    QFile file;
    QDomDocument doc;
    QString xml_error;
    int error_line,error_column;
    SwComponent_ClassPtr root_component;

    try {
        QString stream_desc;
        file.setFileName(_streamFileName);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to open file stream %1").arg(_streamFileName));
            return;
        }
        stream_desc=QString(file.readAll());
        //parsing xml du document
        if (!doc.setContent(stream_desc,&xml_error,&error_line,&error_column)) {
            QString msg=QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
            QMessageBox::warning(NULL,"StreamWorkEditor alert",msg);
            return;
        }
        SwLoader_Class loader;
        root_component=loader.Load(doc);
        if (root_component==NULL) {
            QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Unable to load %1").arg(_streamFileName));
            return;
        }
        //Supression de l'ancien
        disconnectFromControler(_rootComponent);
        SW_APP->DestroyStream(_rootComponent);
        //Enregistrement du composant
        SW_APP->AddNewStream(root_component);
        _rootComponent=root_component;
        //Chargement donnees visuelles
        loadVisualData(doc);
        buildLinks();
        //Connection au modele
        recursiveConnectToControler(_rootComponent);
    } catch(SwException & e) {
        _enableStreamControlerObservation=true;
        QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Exception when load %1\n%2").arg(_streamFileName).arg(e.what()));
        return;
    }
    _enableStreamControlerObservation=true;
    streamControlerChanged();
}

/** @brief Save stream */
void StreamControler::saveStream(){
    SwSaver_Class saver;
    QDomDocument doc;
    QByteArray stream_desc;
    QFile file;

    //serialisation du stream
    saver.Save(_rootComponent,doc);
    //Ajout donnees visuelles
    saveVisualData(doc);
    //Recuperation du stream
    stream_desc=doc.toByteArray(4); //Indentation de quatre espace
    //Ouverture d'un fichier en ecriture
    file.setFileName(_streamFileName);
    if (file.open(QIODevice::WriteOnly  | QIODevice::Truncate)==false) {
        QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Fail to save stream in file %1").arg(_streamFileName));
        return;
    }
    //Ecriture du fichier
    file.write(stream_desc);
    //Fermeture du fichier
    file.close();

}

/** @brief Save stream as */
void StreamControler::saveStreamAs(QString streamFileName){
    _streamFileName=streamFileName;
    saveStream();
}


/** @brief Stream file name access */
QString StreamControler::getStreamFileName() {
    return _streamFileName;
}
/** @brief renvoie l'item racine */
SwComponent_Class * StreamControler::getRootItem() {
    return _rootComponent;
}
/** @brief force la selection d'un item */
void StreamControler::setSelection(StreamWork::SwCore::SwComponent_Class * component) {
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(component); 
    if (it!=_mapCompToItem.end()) {
        _streamScene->clearSelection();
        (*it)->setSelected(true);
        _streamView->centerOn((*it));
    }
}
//------------------------------------------------------------------------------
// Evenements de la vue 
//------------------------------------------------------------------------------
/** @brief demande de menu contextuel a une position donnée */
QMenu * StreamControler::getContextualMenu(const QPointF & p) {
    return MenuManager::getInstance()->buildContextMenu(p);
}
/** @brief drop d'un composant a une position donné */
void StreamControler::addComponent(QString & name,const QPointF & p) {
    _creationPosition=p;
    SwComponent_ClassPtr component=SW_FACTORIES.CreateComponent(name);
    QList<QGraphicsItem *> list=_streamScene->items(p);
    QList<ComponentGraphicItem *> listc;
    for(int i=0;i<list.count();i++) {
        ComponentGraphicItem * gcomponent=dynamic_cast<ComponentGraphicItem *>(list[i]);
        if (gcomponent!=0)
            listc.push_back(gcomponent);
    }
    if (listc.count()==0) {
        component->SetName(_rootComponent->GetSuggestedNameForChild(name));
        _rootComponent->AddChild(component);
    } else {
        component->SetName(listc.front()->getComponent()->GetSuggestedNameForChild(name));
        listc.front()->getComponent()->AddChild(component);
    }
}
//------------------------------------------------------------------------------
// Evenements Composant
//------------------------------------------------------------------------------
/** @brief demande de visualisation des propriétés */
void StreamControler::showProperties() {
    _propertiesWidget->showProperties(_streamView);
}
//------------------------------------------------------------------------------
// Evenements de la scene 
//------------------------------------------------------------------------------
/** @brief changement de la selection */
void StreamControler::selectionChanged() {
    MenuManager::getInstance()->setControler(this);
    MenuManager::getInstance()->selectionChanged();
    QList<QGraphicsItem *> list=_streamScene->selectedItems();
    QList<SwComponent_Class *> sel;
    ComponentGraphicItem * first_gcomponent=0;
    for(int i=0;i<list.count();i++) {
        ComponentGraphicItem * gcomponent=dynamic_cast<ComponentGraphicItem *>(list[i]);
        if (gcomponent!=0) {
            if (first_gcomponent==0) 
                first_gcomponent=gcomponent;
            sel.push_back(gcomponent->getComponent());
        }
    }
    for(int i=0;i<_selectionObservers.count();i++) {
        _selectionObservers[i]->setSelection(sel);
    }
    if (first_gcomponent!=0) {
        _propertiesWidget->setSelectedGraphicComponent(first_gcomponent);
        return;
    }
    _propertiesWidget->setSelectedGraphicComponent(0);
}
/** @brief changement du stream */
void StreamControler::streamControlerChanged() {
    for(int i=0;i<_streamControlerObservers.count();i++) {
        _streamControlerObservers[i]->controlerHasChanged();
    }
}
/** @brief sur demande de connection */
void StreamControler::onLinkConnectors(ConnectorGraphicItem * src,ConnectorGraphicItem * target) {
    if (src->getConnectorType()==CONSUMER) {
        //Cas interface
        SwComponent_Class * cconsumer=((ComponentGraphicItem *)src->parentItem())->getComponent();
        ISwInterfaces_Consumer *iconsumer=
            dynamic_cast<ISwInterfaces_Consumer *>(cconsumer->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
        SwComponent_Class * cprovider=((ComponentGraphicItem *)target->parentItem())->getComponent();
        ISwInterfaces_Provider *iprovider=
            dynamic_cast<ISwInterfaces_Provider *>(cprovider->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
        iconsumer->AttachProvider(iprovider,src->getName(),target->getName());
    } 
    if (src->getConnectorType()==PIN) {
        SwComponent_Class * csource=((ComponentGraphicItem *)src->parentItem())->getComponent();
        SwComponent_Class * ctarget=((ComponentGraphicItem *)target->parentItem())->getComponent();
        ISwPins_Manager *lpinManager=dynamic_cast<ISwPins_Manager *>(csource->QueryService(CG_SW_SERVICE_PINS_MANAGER));
        ISwPins_Manager *rpinManager=dynamic_cast<ISwPins_Manager *>(ctarget->QueryService(CG_SW_SERVICE_PINS_MANAGER));
        if (lpinManager!=0 && rpinManager!=0 && rpinManager!=lpinManager) {
            lpinManager->ConnectRemotePinToLocalPin(src->getName(),target->getName(),rpinManager);
        }
    }
}
/** @brief sur sceneRectChanged de la scene */
void StreamControler::onSceneRectChanged(const QRectF & rect) {
    // A Faire ->scrollcontentBy de la view
    if (_streamView!=0) {
        _streamView->externalUpdateSceneRect();
    }
}

//------------------------------------------------------------------------------
// Evenements du modele 
//------------------------------------------------------------------------------
/** @brief evenement du stream : ajout d'un enfant */
void StreamControler::childAdded(SwComponent_Class * parent,SwComponent_Class *child) {
    ComponentGraphicItem * c=new ComponentGraphicItem(child,this);
    if (parent!=_rootComponent) {
        QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
        it=_mapCompToItem.find(parent);
        if (it!=_mapCompToItem.end()) {
            c->setParentItem((*it));
        }
        c->setPos((*it)->mapFromScene(_creationPosition));
    } else {
        c->setPos(_creationPosition);
    }
    _mapCompToItem.insert(child,c);
    connectToControler(child);
    if (parent==_rootComponent) {
        _streamScene->addItem(c);  
    }
    buildConnectors(child);
    c->updateAttributs();
    _streamView->update();
    streamControlerChanged();
}
/** @brief evenement du stream : suppression d'un enfant */
void StreamControler::childRemoved(StreamWork::SwCore::SwComponent_Class * parent,
                                   StreamWork::SwCore::SwComponent_Class *child) {
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(child); 
    if (it!=_mapCompToItem.end()) {
        (*it)->setParentItem(0);
        _streamScene->removeItem((*it));
        recursiveDisconnectToControler(child);
    }
    streamControlerChanged();
}
/** @brief evenement du stream : renommage d'un composant */
void StreamControler::componentNameChanged(StreamWork::SwCore::SwComponent_Class * component) {
    if (component==_rootComponent)
        return;
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(component);
    if (it!=_mapCompToItem.end()) {
        (*it)->updateAttributs();
    }
    streamControlerChanged();
}
/*! \brief Sur ajout d'une nouvelle interface */
void StreamControler::OnAddInterface(ISwInterfaces_Service * source,QString interface_name) {
    ISwHost * ihost=dynamic_cast<ISwHost *>(source);
    SwComponent_Class * component=ihost->GetHostComponent();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(component); 
    if (it==_mapCompToItem.end()) {
        return;
    }
    ComponentGraphicItem * gitem=(*it);
    QList<ConnectorGraphicItem *> * connectors=gitem->getConnectors();

    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(source);
    if (iprovider!=0) {
        QString type=iprovider->GetInterfaceType(interface_name);
        ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,interface_name,type,PROVIDER);
        connectors->push_back(gcitem);
        gcitem->setParentItem(gitem);
        gitem->updateAttributs();
        return;
    }
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(source);
    if (iconsumer!=0) {
        QString type=iconsumer->GetInterfaceType(interface_name);
        ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,interface_name,type,CONSUMER);
        connectors->push_back(gcitem);
        gcitem->setParentItem(gitem);
        gitem->updateAttributs();
        return;
    }
    return;
}
/*! \brief Sur suppression d'une  interface */
void StreamControler::OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name){
    ISwHost * ihost=dynamic_cast<ISwHost *>(source);
    SwComponent_Class * component=ihost->GetHostComponent();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(component); 
    if (it==_mapCompToItem.end()) {
        return;
    }
    ComponentGraphicItem * gitem=(*it);
    QList<ConnectorGraphicItem *> * connectors=gitem->getConnectors();
    for(int i=0;i<connectors->count();i++) {
        if ((*connectors)[i]->getName()==interface_name) {
            delete (*connectors)[i];
            connectors->removeAt(i);
            gitem->updateAttributs();
            return;
        }
    }
}            
/*! \brief Sur connection d'une interface */
void StreamControler::OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,
                        ISwInterfaces_Service * remote_source,QString remote_interface_name){
    if (dynamic_cast<ISwInterfaces_Provider *>(source)!=0)
        return;             
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator its;
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator itt;
    its=_mapCompToItem.find(source->GetHostComponent()); 
    itt=_mapCompToItem.find(remote_source->GetHostComponent()); 
    if (its==_mapCompToItem.end() || itt==_mapCompToItem.end()) {
        return;
    }    
    ConnectorGraphicItem *cgis=its.value()->getConnector(interface_name);
    ConnectorGraphicItem *cgit=itt.value()->getConnector(remote_interface_name);
    LinkGraphicItem *lgi=new LinkGraphicItem(cgis,cgit); 
    _streamScene->addItem(lgi);  
    lgi->setZValue(-100.0);
}            
/*! \brief Sur deconnection d'une interface */
void StreamControler::OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,
                           ISwInterfaces_Service * remote_source,QString remote_interface_name){
    ISwInterfaces_Service * lsource=source;
    QString iname=interface_name;
    if (dynamic_cast<ISwInterfaces_Provider *>(source)!=0) {
        return;
        //lsource=remote_source;  
        //iname=remote_interface_name;
    }
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator its;
    its=_mapCompToItem.find(lsource->GetHostComponent()); 
    if (its==_mapCompToItem.end() ) {
        return;
    }    
    ConnectorGraphicItem *cgis=its.value()->getConnector(iname);
    if (!cgis->getLinks()->isEmpty()) {
        LinkGraphicItem *lgi=cgis->getLinks()->front();
        cgis->getLinks()->pop_front();
        lgi->setParentItem(0);
        _streamScene->removeItem(lgi);
        delete lgi;
    }
}           
/*! \brief Sur ajout d'un nouveau pin*/
void StreamControler::OnAddPin(StreamWork::SwCore::SwPin * pin) {
    ISwPins_Manager *pinManager=pin->GetManager();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(pinManager->GetHostComponent()); 
    if (it==_mapCompToItem.end()) {
        return;
    }
    ComponentGraphicItem * gitem=(*it);
    QList<ConnectorGraphicItem *> * connectors=gitem->getConnectors();
    ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,pin->GetName(),pin->GetType(),PIN);
    connectors->push_back(gcitem);
    gcitem->setParentItem(gitem);
    gitem->updateAttributs();

}
/*! \brief Sur suppression d'un pin existant*/
void StreamControler::OnRemovePin(StreamWork::SwCore::SwPin * pin){
    ISwPins_Manager *pinManager=pin->GetManager();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(pinManager->GetHostComponent()); 
    if (it==_mapCompToItem.end()) {
        return;
    }
    ConnectorGraphicItem * cgis=it.value()->getConnector(pin->GetName());
    //if (!cgis->getLinks()->isEmpty()) {
    //    LinkGraphicItem *lgi=cgis->getLinks()->front();
    //    cgis->getLinks()->pop_front();
    //    lgi->setParentItem(0);
    //    _streamScene->removeItem(lgi);
    //    delete lgi;
    //}
    it.value()->getConnectors()->removeOne(cgis);
    it.value()->updateAttributs();
    delete cgis;

}            
/*! \brief Sur connexion d'un pin*/
void StreamControler::OnConnectPin(StreamWork::SwCore::SwPin * local_pin,StreamWork::SwCore::SwPin * remote_pin){
    SwComponent_Class * lcomponent=local_pin->GetManager()->GetHostComponent();
    SwComponent_Class * rcomponent=remote_pin->GetManager()->GetHostComponent();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator itl;
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator itr;
    itl=_mapCompToItem.find(lcomponent); 
    itr=_mapCompToItem.find(rcomponent); 
    if (itl!=_mapCompToItem.end() && itr!=_mapCompToItem.end()) {
        ConnectorGraphicItem * ritem=(*itr)->getConnector(remote_pin->GetName());
        ConnectorGraphicItem * litem=(*itl)->getConnector(local_pin->GetName());
        if (ritem!=0 && litem!=0 && ritem->getLinks()->isEmpty()) {
            //Construction du lien
            LinkGraphicItem *lgi=new LinkGraphicItem(litem,ritem); 
            _streamScene->addItem(lgi);  
            lgi->setZValue(-100.0);
        }
    }
}            
/*! \brief Sur deconnexion d'un pin*/
void StreamControler::OnDisconnectPin(StreamWork::SwCore::SwPin * local_pin,StreamWork::SwCore::SwPin * remote_pin){
    SwComponent_Class * lcomponent=local_pin->GetManager()->GetHostComponent();
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator itl;
    itl=_mapCompToItem.find(lcomponent); 
    if (itl!=_mapCompToItem.end()) {
        ConnectorGraphicItem * litem=(*itl)->getConnector(local_pin->GetName());
        if (litem!=0 && !litem->getLinks()->isEmpty()) {
            LinkGraphicItem *lgi=litem->getLinks()->front();
            litem->getLinks()->pop_front();
            lgi->setParentItem(0);
            _streamScene->removeItem(lgi);
            delete lgi;
        }
    }
}            
//------------------------------------------------------------------------------
// Liaison modele <-> Controler
//------------------------------------------------------------------------------
/** @brief connexion des evenements d'un composant au controleur */ 
void StreamControler::connectToControler(SwComponent_Class * component) {
    component->OnAddChild.iconnect(*this,&StreamControler::childAdded);
    component->OnRemoveChild.iconnect(*this,&StreamControler::childRemoved);
    component->OnChangeComponentName.iconnect(*this,&StreamControler::componentNameChanged);
    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (iprovider!=0) {
        iprovider->AttachInterfacesServices_Listener(this);
    }
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        iconsumer->AttachInterfacesServices_Listener(this);
    }
    ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pinManager!=0) {
        pinManager->RegisterListener(this);
    }
}
/** @brief connexion des evenements d'un composant au controleur */ 
void StreamControler::recursiveConnectToControler(SwComponent_Class * component){
    connectToControler(component);
    unsigned long count=component->GetNbChildren();
    for(unsigned long i=0;i<count;i++) {
        recursiveConnectToControler(component->GetChild(i));
    }
}
/** @brief deconnexion des evenements de tous les composant d'un stream au controleur */ 
void StreamControler::disconnectFromControler(SwComponent_Class * component){
    component->OnAddChild.idisconnect(*this,&StreamControler::childAdded);
    component->OnRemoveChild.idisconnect(*this,&StreamControler::childRemoved);
    component->OnChangeComponentName.idisconnect(*this,&StreamControler::componentNameChanged);
    //La partie suivante est mise en commentaire pour ctacher les events en cours de destructions
    //ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    //if (iprovider!=0) {
    //    iprovider->DetachInterfacesServices_Listener(this);
    //}
    //ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    //if (iconsumer!=0) {
    //    iconsumer->DetachInterfacesServices_Listener(this);
    //}
    //ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    //if (pinManager!=0) {
    //    pinManager->UnregisterListener(this);
    //}
}
/** @brief deconnexion des evenements d'un composant au controleur */ 
void StreamControler::recursiveDisconnectToControler(SwComponent_Class * component){
    disconnectFromControler(component);
    unsigned long count=component->GetNbChildren();
    for(unsigned long i=0;i<count;i++) {
        recursiveDisconnectToControler(component->GetChild(i));
    }
}
//--------------------------------------------------------------------------
// Gestion persistence des données
//--------------------------------------------------------------------------
#define CL_IA_NODE "IA"
#define CL_IA_ATT_X "x"
#define CL_IA_ATT_Y "y"
#define CL_IA_ATT_W "W"
#define CL_IA_ATT_H "H"
#define CL_IA_ATT_COLOR "color"
#define CL_IA_ATT_TEXT "text"

/** @brief sauvegarde des données visuelles */
void StreamControler::saveVisualData(QDomDocument & doc){
    QDomElement sceneNode=doc.createElement(CL_SCENE_NODE);
    QRectF srect=_streamScene->sceneRect();
    sceneNode.setAttribute(CL_SCENE_ATT_X,srect.topLeft().x());
    sceneNode.setAttribute(CL_SCENE_ATT_Y,srect.topLeft().y());
    sceneNode.setAttribute(CL_SCENE_ATT_WIDTH,srect.width());
    sceneNode.setAttribute(CL_SCENE_ATT_HEIGHT,srect.height());
    doc.documentElement().appendChild(sceneNode);

    QList<QGraphicsItem *> items=_streamScene->items();
    int count=items.count();
    for(int i=0;i<count;i++) {
        QGraphicsItem * item=items[i];
        if (item->parentItem()==0) {
            ComponentGraphicItem * cgitem=dynamic_cast<ComponentGraphicItem *>(item);
            if (cgitem!=0) {
                saveVisualItem(cgitem,doc,sceneNode);
            }
            InterestArea * ia=dynamic_cast<InterestArea *>(item);
            if (ia!=0) {
                QDomElement itemNode=doc.createElement(CL_IA_NODE);
                itemNode.setAttribute(CL_IA_ATT_X,ia->pos().x());
                itemNode.setAttribute(CL_IA_ATT_Y,ia->pos().y());
                QRectF r=ia->boundingRect();
                itemNode.setAttribute(CL_IA_ATT_W,r.width());
                itemNode.setAttribute(CL_IA_ATT_H,r.height());
                itemNode.setAttribute(CL_IA_ATT_COLOR,ia->getColor().name());
                itemNode.setAttribute(CL_IA_ATT_TEXT,ia->getText());
                sceneNode.appendChild(itemNode);
            }
        }
    }
}
/** @brief sauvegarde d'un item */
void StreamControler::saveVisualItem(ComponentGraphicItem * item,QDomDocument & doc,QDomElement &parentNode){
    QDomElement itemNode=doc.createElement(CL_CGITEM_NODE);
    itemNode.setAttribute(CL_CGITEM_ATT_NAME,item->getComponent()->GetName());
    itemNode.setAttribute(CL_CGITEM_ATT_X,item->pos().x());
    itemNode.setAttribute(CL_CGITEM_ATT_Y,item->pos().y());
    itemNode.setAttribute(CL_CGITEM_ATT_COLOR,item->getColor().name());
    itemNode.setAttribute(CL_CGITEM_ATT_TEXT_COLOR,item->getTextColor().name());
    parentNode.appendChild(itemNode);
    //Connector
    QList<ConnectorGraphicItem *> * clist=item->getConnectors();
    for(int i=0;i<clist->count();i++) {
        QDomElement cntNode=doc.createElement(CL_CGCNTITEM_NODE); 
        cntNode.setAttribute(CL_CGCNTITEM_ATT_NAME,(*clist)[i]->getName());
        cntNode.setAttribute(CL_CGCNTITEM_ATT_POS,(*clist)[i]->getParentPosition()==LEFT?"left":"right");
        itemNode.appendChild(cntNode);
    }

    //Enfant
    QList<QGraphicsItem *> items=item->childItems();
    int count=items.count();
    for(int i=0;i<count;i++) {
        ComponentGraphicItem * cgitem=dynamic_cast<ComponentGraphicItem *>(items[i]);
        if (cgitem!=0) {
            saveVisualItem(cgitem,doc,itemNode);
        }
    }
}
/** @brief chargement des données visuelles */
void StreamControler::loadVisualData(QDomDocument & doc){
    QDomElement sceneNode=doc.documentElement().firstChildElement(QString(CL_SCENE_NODE));
    if (sceneNode.isNull()) {
        buildComponents(_rootComponent);
        QList<ComponentGraphicItem *> mainItems=_streamScene->getAllComponentGraphicItems();
        GraphicsLoaderV1::loadGraphicsInfoFromDom(doc,mainItems,_streamScene);
        Arranger arranger;
        arranger.arrangeConnectors(&mainItems);
        return;
    }
    QRectF srect;
    srect.setLeft(sceneNode.attribute(CL_SCENE_ATT_X).toDouble());
    srect.setTop(sceneNode.attribute(CL_SCENE_ATT_Y).toDouble());
    srect.setWidth(sceneNode.attribute(CL_SCENE_ATT_WIDTH).toDouble());
    srect.setHeight(sceneNode.attribute(CL_SCENE_ATT_HEIGHT).toDouble());
    _streamScene->setSceneRect(srect);

    for(QDomElement node = sceneNode.firstChildElement(QString(CL_CGITEM_NODE));
        !node.isNull();
        node = node.nextSiblingElement(QString(CL_CGITEM_NODE)))
    {
        loadVisualItem(doc,node,_rootComponent,0);
    }

    for(QDomElement node = sceneNode.firstChildElement(QString(CL_IA_NODE));
        !node.isNull();
        node = node.nextSiblingElement(QString(CL_IA_NODE)))
    {
        InterestArea * ia=new InterestArea(this);
        QPointF p(node.attribute(CL_IA_ATT_X).toDouble(),node.attribute(CL_IA_ATT_Y).toDouble());
        ia->setPos(p);
        QRectF bbox(0,0,node.attribute(CL_IA_ATT_W).toDouble(),node.attribute(CL_IA_ATT_H).toDouble());
        ia->setBoundingRect(bbox);
        ia->setColor(QColor(node.attribute(CL_IA_ATT_COLOR)));
        ia->setText(node.attribute(CL_IA_ATT_TEXT));
        _streamScene->addItem(ia);
    }
}
/** @brief chargement d'un item */
void StreamControler::loadVisualItem(QDomDocument & doc,QDomElement &node,SwComponent_Class * parentComponent,QGraphicsItem * parentItem){
    //Chargement item courant
    QString cname=node.attribute(CL_CGITEM_ATT_NAME);
    SwComponent_Class * component=parentComponent->GetChild(cname);
    if (component==0) {
        QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Fail to find component %1").arg(cname));
        return;
    }
    ComponentGraphicItem * cgitem=new ComponentGraphicItem(component,this);
    QPointF pos;
    pos.setX(node.attribute(CL_CGITEM_ATT_X).toDouble());
    pos.setY(node.attribute(CL_CGITEM_ATT_Y).toDouble());
    cgitem->setPos(pos);
    cgitem->setColor(QColor(node.attribute(CL_CGITEM_ATT_COLOR)));
    cgitem->setTextColor(QColor(node.attribute(CL_CGITEM_ATT_TEXT_COLOR)));
   
    if (parentComponent!=_rootComponent) {
        QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
        it=_mapCompToItem.find(parentComponent);
        if (it!=_mapCompToItem.end()) {
            cgitem->setParentItem((*it));
        }
    } 
    _mapCompToItem.insert(component,cgitem);
    if (parentComponent==_rootComponent) {
        _streamScene->addItem(cgitem);  
    }
    buildConnectors(component);
    //Chargement de l'ordre des connectors
    QList<QString> labelConnectors;
    QList<TConnectorPosition> labelPositions;
    for(QDomElement cntnode = node.firstChildElement(QString(CL_CGCNTITEM_NODE));
        !cntnode.isNull();
        cntnode = cntnode.nextSiblingElement(QString(CL_CGCNTITEM_NODE)))
    {
        labelConnectors.push_back(cntnode.attribute(CL_CGCNTITEM_ATT_NAME));
        labelPositions.push_back(cntnode.attribute(CL_CGCNTITEM_ATT_POS)=="right"?RIGHT:LEFT);
    }
    QList<ConnectorGraphicItem *> * clist=cgitem->getConnectors();
    if (labelConnectors.count()==clist->count()) {
        QList<ConnectorGraphicItem *> original_list=(* clist);
        for(int i=0;i<original_list.count();i++) {
            int j=labelConnectors.indexOf(original_list[i]->getName());
            (*clist)[j]=original_list[i];
            (*clist)[j]->setParentPosition(labelPositions[j]);
        }
    }
    cgitem->updateAttributs();
    //Chargement item enfant
    for(QDomElement cnode = node.firstChildElement(QString(CL_CGITEM_NODE));
        !cnode.isNull();
        cnode = cnode.nextSiblingElement(QString(CL_CGITEM_NODE)))
    {
        loadVisualItem(doc,cnode,component,cgitem);
    }



}
//--------------------------------------------------------------------------
// Tools
//--------------------------------------------------------------------------
/** @brief buildComponents */
ComponentGraphicItem * StreamControler::buildComponents(StreamWork::SwCore::SwComponent_Class * component) {
   ComponentGraphicItem * cgitem=0;

    if (component!=_rootComponent) {
        cgitem=new ComponentGraphicItem(component,this);
        QPointF pos;
        pos.setX(0.0);
        pos.setY(0.0);
        cgitem->setPos(pos);
        cgitem->setColor(QColor(0,0,128));
        cgitem->setTextColor(QColor(255,255,255));

        StreamWork::SwCore::SwComponent_Class * parentComponent=component->GetParent();
        QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
        it=_mapCompToItem.find(parentComponent);
        if (it!=_mapCompToItem.end()) {
            cgitem->setParentItem((*it));
        }
        _mapCompToItem.insert(component,cgitem);
        if (parentComponent==_rootComponent) {
            _streamScene->addItem(cgitem);  
        }
        buildConnectors(component);
    }
    SwComponent_Class * scomponent=component->GetFirstChild();
    int nbChilds=component->GetNbChildren();
    int index=0;
    ComponentGraphicItem * scgitem;
    while (scomponent!=0) {
        scgitem=buildComponents(scomponent);
        if (nbChilds>1) {
            QPointF pos;
            pos.setX(300.0*cos(3.14*(double)(index)/(double)nbChilds));
            pos.setY(300.0*sin(3.14*(double)(index)/(double)nbChilds));
            scgitem->setPos(pos);
        }
        scomponent=component->GetNextChild();
        index++;
    }
    return cgitem;
}
/** @brief buildConnectors */
void StreamControler::buildConnectors(StreamWork::SwCore::SwComponent_Class * component) {
    //return;
    QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
    it=_mapCompToItem.find(component); 
    if (it==_mapCompToItem.end()) {
        return;
    }
    ComponentGraphicItem * gitem=(*it);
    QList<ConnectorGraphicItem *> * connectors=gitem->getConnectors();

    ISwInterfaces_Provider *iprovider=dynamic_cast<ISwInterfaces_Provider *>(component->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    if (iprovider!=0) {
        QString name=iprovider->GetFirstInterface();
        while (!name.isNull()) {
            QString type=iprovider->GetInterfaceType(name);
            ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,name,type,PROVIDER);
            connectors->push_back(gcitem);
            gcitem->setParentItem(gitem);
            name=iprovider->GetNextInterface();
        }  
    }
    ISwInterfaces_Consumer *iconsumer=dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (iconsumer!=0) {
        QString type;
        QString name=iconsumer->GetFirstInterface(&type,0,0);
        while (!name.isNull()) {
            ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,name,type,CONSUMER);
            connectors->push_back(gcitem);
            gcitem->setParentItem(gitem);
            name=iconsumer->GetNextInterface(&type,0,0);
        }  
    }
    ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (pinManager!=0) {
        QList<SwPin *> plist=pinManager->GetPinList();
        QList<SwPin *>::iterator it=plist.begin();
        while(it!=plist.end()) {
            SwPin * pin=(*it);
            ConnectorGraphicItem * gcitem=new ConnectorGraphicItem(gitem,pin->GetName(),pin->GetType(),PIN);
            connectors->push_back(gcitem);
            gcitem->setParentItem(gitem);
            it++;
        }
    }
    gitem->updateAttributs();
}
/** @brief construction des liens */
void StreamControler::buildLinks() {
    QList<QGraphicsItem *> litems=_streamScene->items();
    QList<QGraphicsItem *>::iterator it=litems.begin();
    while(it!=litems.end()) {
        ConnectorGraphicItem * citem=dynamic_cast<ConnectorGraphicItem *>(*it);
        if (citem!=0) {
            //Cas connecteur consommateur
            if (citem->getConnectorType()==CONSUMER) {
                //Recherche de l'interface produite si elle existe
                SwComponent_Class * component=((ComponentGraphicItem *)citem->parentItem())->getComponent();
                ISwInterfaces_Consumer *iconsumer=
                    dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
                if (iconsumer!=0) {
                    ISwInterfaces_Provider * pt_provider=0;
                    QString interface_provider;
                    QString model_type;
                    QString interface_consumer=iconsumer->GetFirstInterface(&model_type,&pt_provider,&interface_provider);
                    while (!interface_consumer.isNull() && interface_consumer!=citem->getName()) {
                        pt_provider=0;
                        interface_consumer=iconsumer->GetNextInterface(&model_type,&pt_provider,&interface_provider);
                    }
                    if (!interface_consumer.isNull() && pt_provider!=0 && citem->getModelType()==model_type) {
                        //Interface produite trouvé -> recuperation du connecteur associé
                        SwComponent_Class * pcomponent=pt_provider->GetHostComponent();
                        QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
                        it=_mapCompToItem.find(pcomponent); 
                        if (it!=_mapCompToItem.end()) {
                            ComponentGraphicItem * gitem=(*it);
                            ConnectorGraphicItem * pitem=gitem->getConnector(interface_provider);
                            if (pitem!=0) {
                                //Construction du lien
                                LinkGraphicItem *lgi=new LinkGraphicItem(citem,pitem); 
                                _streamScene->addItem(lgi);  
                                lgi->setZValue(-100.0);
                            }
                        }
                    }
                }
            }
            if (citem->getConnectorType()==PIN && citem->getLinks()->isEmpty()) {
                SwComponent_Class * component=((ComponentGraphicItem *)citem->parentItem())->getComponent();
                ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
                if (pinManager!=0) {
                    SwPin * pin=pinManager->GetPinByName(citem->getName());
                    SwPin * rpin=pin->GetConnected();
                    if(rpin!=0) {
                        SwComponent_Class * rcomponent=rpin->GetManager()->GetHostComponent();
                        QMap<StreamWork::SwCore::SwComponent_Class *,ComponentGraphicItem *>::iterator it;
                        it=_mapCompToItem.find(rcomponent); 
                        if (it!=_mapCompToItem.end()) {
                            ConnectorGraphicItem * pitem=(*it)->getConnector(rpin->GetName());
                            if (pitem!=0) {
                                //Construction du lien
                                LinkGraphicItem *lgi=new LinkGraphicItem(citem,pitem); 
                                _streamScene->addItem(lgi);  
                                lgi->setZValue(-100.0);
                            }
                        }
                    }
                }
            }
        }
        it++;
    }
}
/** @brief destruction des liens */
void StreamControler::destroyLinks() {
    QList<QGraphicsItem *> litems=_streamScene->items();
    QList<QGraphicsItem *>::iterator it=litems.begin();
    while(it!=litems.end()) {
        LinkGraphicItem * lkitem=dynamic_cast<LinkGraphicItem *>(*it);
        if (lkitem!=0) {
            delete lkitem;
        }
        it++;
    }
}
//--------------------------------------------------------------------------
//  gestion des observers de selection
//--------------------------------------------------------------------------
void StreamControler::addSelectionObserver(ISelectionObserver * observer) {
    _selectionObservers.push_back(observer);
}
void StreamControler::removeSelectionObserver(ISelectionObserver * observer) {
    _selectionObservers.removeOne(observer);
}
//--------------------------------------------------------------------------
//  gestion des observers de changement
//--------------------------------------------------------------------------
void StreamControler::addStreamControlerObserver(IStreamControlerObserver * observer){
    _streamControlerObservers.push_back(observer);
}
void StreamControler::removeStreamControlerObserver(IStreamControlerObserver * observer){
    _streamControlerObservers.removeOne(observer);
}
