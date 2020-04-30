/*!
 \file _SwEditorCompToolbox.cpp
 \brief Implementation of the Class _SwEditorCompToolbox generant un boite a outils contenant la liste des composants
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QListView>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwEditorCompToolbox.h"
#include "PluginsListModel.h"
#include "ComponentListModel.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorCompToolbox::_SwEditorCompToolbox(): SwComponent_Class()
{
    _provider_service = NULL;
    _toolbox = NULL;
}
/*! \brief Destructeur */
_SwEditorCompToolbox::~_SwEditorCompToolbox()
{
    //Desenregistrement des services
    this->UnregisterService( _provider_service->GetServiceName() );
    this->UnregisterService( _properties_service->GetServiceName() );
    //qDebug("_SwEditorCompToolbox variables destruction");
    //Destruction des services
    delete _provider_service;
    delete _properties_service;
    delete _toolbox;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwEditorCompToolbox::InitializeResources() throw( SwException )
{
    //Creation des service
    _provider_service = new SwInterfaces_Provider_Class( this ) ;
    _properties_service = new SwProperties_Class( this );
    //Creation de l'interface principale
    _toolbox = new QSplitter();
    BuildWidget();
    //Enregistrement des services
    this->RegisterService( _properties_service );
    this->RegisterService( _provider_service );
    
    //Exportation de l'interface QWidget
    _provider_service->RegisterProvidedInterface<QWidget>( "Widget", _toolbox );
    
    _properties_service->CreatePropertiesForQObject( _toolbox, "QToolBox" );
    
    if( SW_APP->IsVerbose() ) SW_APP->Logger().Log( LogLvl_Info, QString( "InitializeResources of SwEditorCompToolBox done\n" ) );
    
}
//---------------------------------------------------------------------
// Autres
//---------------------------------------------------------------------
void _SwEditorCompToolbox::BuildWidget()
{

    QListView * lviewPlugins = new QListView( _toolbox );
    lviewPlugins->setModel( new PluginsListModel( lviewPlugins ) );
    //lviewPlugins->setFixedWidth(150);
    lviewPlugins->setFrameShape( QFrame::NoFrame );
    //lviewPlugins->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
    //lviewPlugins->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    QListView * lviewComponents = new QListView( _toolbox );
    ComponentListModel * componentModel = new ComponentListModel( lviewComponents );
    lviewPlugins->connect( lviewPlugins, SIGNAL( clicked( const QModelIndex & ) ), componentModel, SLOT( onSelectedPluginChanged( const QModelIndex & ) ) );
    lviewComponents->setModel( componentModel );
    //lviewComponents->setFixedWidth(200);
    lviewComponents->setFrameShape( QFrame::NoFrame );
    lviewComponents->setDragEnabled( true );
    //lviewComponents->verticalScrollBar()->setStyleSheet(ScrollBarVerticalStyle);
    //lviewComponents->horizontalScrollBar()->setStyleSheet(ScrollBarHorizontalStyle);
    _toolbox->addWidget( lviewComponents );
    _toolbox->addWidget( lviewPlugins );
    
    
    /*    QMap<QString,SwPluginFactory_Class *> * p_map;
        QMap<QString,SwPluginFactory_Class *>::iterator it;
        QListView *view;
        //Clean
        while (_toolbox->count()>0) {
            _toolbox->removeItem(0);
        }
        //Rebuild
        p_map=SW_APP->ComponentsBank().GetAllPlugins();
        for (it=p_map->begin();it!=p_map->end();it++) {
            view=new QListView(_toolbox);
            view->setDragEnabled(true);
            view->setSelectionMode(QAbstractItemView::ExtendedSelection);
            view->setModel(it.value()->GetModelList());
            _toolbox->addItem(view,QIcon(":/SwEditor/comp_toolbox.png"),it.value()->GetPluginName());
        } */
}
