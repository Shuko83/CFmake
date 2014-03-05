/*!
 \file _SwGuiCompTreeView.cpp
 \brief Implementation of the Class _SwGuiCompTreeView generant un QTreeView pour un modele fournit
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QObject>
#include <SwApplication.h>
#include <SwMacros.h>
#include <QStandardItem>
#include "_SwGuiCompTreeView.h"
#include "SwGuiDefaultItemDelegate.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"


/*! \brief Constructeur */
_SwGuiCompTreeView::_SwGuiCompTreeView(): SwComponent_Class(){
	QStandardItem *item;

    _provider_service=NULL;
	_consumer_service=NULL;
    _properties_service=NULL;
    _treeview=NULL;
	_handle=NULL;
	_default_model=new QStandardItemModel(1,1);
    _default_model->setHorizontalHeaderLabels(QStringList(QString("Informations")));
	item= new QStandardItem(QString("No alone component selected"));
    _default_model->setItem(0,0, item);
    _slots_handles=NULL;
}
/*! \brief Destructeur */
_SwGuiCompTreeView::~_SwGuiCompTreeView(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _treeview;
	delete _default_model;
    delete _signals_catcher;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompTreeView::InitializeResources() throw(SwException) {
    //Creation des service
    _properties_service=new SwProperties_Class(this);
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    //Creation de l'interface principale
    _treeview=new QTreeView();
    _signals_catcher=new _SwQAbstractItemViewSlots_Catcher(_treeview);
    _treeview->viewport()->installEventFilter(_signals_catcher);
    _treeview->installEventFilter(_signals_catcher);
    _treeview->setItemDelegate(new SwGuiDefaultItemDelegate(_treeview));
	_treeview->setModel(_default_model);
    _treeview->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(_treeview,SIGNAL(activated(const QModelIndex&)),_signals_catcher,SLOT(onActivated(const QModelIndex&)));
    QObject::connect(_treeview,SIGNAL(clicked(const QModelIndex&)),_signals_catcher,SLOT(onClicked(const QModelIndex&)));
    QObject::connect(_treeview,SIGNAL(doubleClicked(const QModelIndex&)),_signals_catcher,SLOT(onDoubleClicked(const QModelIndex&)));
    QObject::connect(_treeview,SIGNAL(entered(const QModelIndex&)),_signals_catcher,SLOT(onEntered(const QModelIndex&)));
    QObject::connect(_treeview,SIGNAL(pressed(const QModelIndex&)),_signals_catcher,SLOT(onPressed(const QModelIndex&)));
    QObject::connect(_treeview,SIGNAL(viewportEntered()),_signals_catcher,SLOT(onViewportEntered()));
    QObject::connect(_treeview,SIGNAL(customContextMenuRequested(const QPoint &)),_signals_catcher,SLOT(OnMenuRequested(const QPoint &)));

	connect(_treeview,SIGNAL(collapsed ( const QModelIndex &  )),_signals_catcher,SLOT(resizeColumn(const QModelIndex&)));
	connect(_treeview,SIGNAL(expanded ( const QModelIndex &  )),_signals_catcher,SLOT(resizeColumn(const QModelIndex&)));

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>("Widget",(ISwWidget *)this);

    //Importation de l'interface QAbstractItemModel
    _consumer_service->RegisterConsumedInterface<QAbstractItemModel>("Model",&_handle);
    //Importation de l'interface QAbstractItemModel
    _consumer_service->RegisterConsumedInterface<ISwQAbstractItemViewSlots>("ViewSlots",&_slots_handles);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

	    //Enregistrement des propriétés
    _properties_service->CreatePropertiesForQObject(_treeview,"QTreeView");

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiTreeView done\n"));

}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompTreeView::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QAbstractItemView::SelectionMode sb;
    if (_handle!=NULL && interface_name==QString("Model")) {
		_treeview->setModel(_default_model);
        _treeview->expandAll();
        sb=_treeview->selectionMode();
    }
    if (_slots_handles!=NULL && interface_name==QString("ViewSlots")) {
        _signals_catcher->DefineInterfaceSlots(NULL);
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompTreeView::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QAbstractItemView::SelectionMode sb;
    if (_handle!=NULL  && interface_name==QString("Model")) {
        _treeview->setModel(_handle);
        _treeview->expandAll();
		_treeview->resizeColumnToContents(0);
        sb=_treeview->selectionMode();
    }
    if (_slots_handles!=NULL && interface_name==QString("ViewSlots")) {
        _signals_catcher->DefineInterfaceSlots(_slots_handles);
    }
}
//---------------------------------------------------------------------
// Interface ISwQMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget & _SwGuiCompTreeView::GetWidget() {
    return *_treeview;
}

