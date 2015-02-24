/*!
 \file _SwEditorStreamView.cpp
 \brief Implementation d'une vue d'une scene de stream
 \version 1.0
 \date 03/01/07
 \author F.Bighelli
*/
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwEditorStreamView.h"
#include <QBrush>
#include <QRectF>
#include <QWheelEvent>


using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorStreamView::_SwEditorStreamView(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _view=NULL;
    _manager=NULL;
}
/*! \brief Destructeur */
_SwEditorStreamView::~_SwEditorStreamView(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _view;
    delete _default_scene;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwEditorStreamView::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    //Creation de l'interface principale
    //_view=new _SwEditorQGraphicsView(&_default_scene);
    _default_scene=new QGraphicsScene();
    _view=new _SwEditorQGraphicsView(_default_scene);
        // a blue background
    _default_scene->setBackgroundBrush(Qt::black);


    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>("Widget",(ISwWidget *)this);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);
    _consumer_service->RegisterConsumedInterface<ISwEditorStreamsManager>("StreamsManager",&_manager);

	//Enregistrement des propriétés
    _properties_service->CreatePropertiesForQObject(_view,"QGraphicsView");

    //Fin
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwEditorStreamView done\n"));

}
//---------------------------------------------------------------------
// Interface ISwWidget
//---------------------------------------------------------------------
/*! \brief Renvoie le widget
\return le widget */
QWidget & _SwEditorStreamView::GetWidget() {
    return *_view;
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwEditorStreamView::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_manager!=NULL) {
        _view->setScene(_default_scene);
        _view->setScene(NULL);
        _manager->DetachStreamManagerObserver(this);
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwEditorStreamView::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_manager!=NULL) {
        _manager->AttachStreamManagerObserver(this);
    }
}
//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwEditorStreamView::Update() {
    if (_manager->GetCurrentStreamGraphicScene()!=NULL) {
        _view->setScene(_manager->GetCurrentStreamGraphicScene()->GetRelatedScene());
        _view->sceneUpdated();
        QRectF r=_view->sceneRect();
        QBrush b=_manager->GetCurrentStreamGraphicScene()->GetRelatedScene()->backgroundBrush();
        if (!(_view->sceneRect().width()>0.0 && _view->sceneRect().height()>0.0)) {
            _view->setSceneRect((qreal)-_view->width()/2,(qreal)-_view->height()/2,_view->width(),_view->height());
        }
        _view->show();
    } else {
       _view->setScene(_default_scene);
    }
}
