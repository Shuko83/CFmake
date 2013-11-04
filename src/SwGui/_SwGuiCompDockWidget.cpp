/*!
 \file _SwGuiCompDockWidget.cpp
 \brief Implementation of the Class _SwGuiCompDockWidget generant un QDockWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompDockWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;

/*! \brief Constructeur */
_SwGuiCompDockWidget::_SwGuiCompDockWidget(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _dockwidget=NULL;
    _handle_widget=NULL;
	_forceFloating = false;
	_showTitleBar = true;
}
/*! \brief Destructeur */
_SwGuiCompDockWidget::~_SwGuiCompDockWidget(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _dockwidget;

}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompDockWidget::InitializeResources() throw(SwException) {
    //Creation des service
    _properties_service=new SwProperties_Class(this);
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    //Creation de l'interface principale
    _dockwidget=new QDockWidget();	
	_titleBar = GetDockWidget().titleBarWidget();

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwDockWidget
    _provider_service->RegisterProvidedInterface<ISwDockWidget>("DockWidget",(ISwDockWidget *)this);

    //Importation des interfaces ISwWidget et ISwLayout (exclusif)
    _consumer_service->RegisterConsumedInterface<ISwWidget>("Widget",&_handle_widget);
     
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

	//Enregistrement des propriétés
	_properties_service->CreatePropertiesForQObject(_dockwidget,"QDockWidget");
	_properties_service->CreatePropertiesForQObject(this,"",true);	

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiDockWidget done\n"));	
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompDockWidget::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_handle_widget!=NULL) {
        _handle_widget->GetWidget().setParent(NULL);
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompDockWidget::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    if (_handle_widget!=NULL) {
        _dockwidget->setWidget(&(_handle_widget->GetWidget()));
    }
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QDockWidget & _SwGuiCompDockWidget::GetDockWidget() {
    return *_dockwidget;
}

//---------------------------------------------------------------------
bool _SwGuiCompDockWidget::getShowTitleBar()
{
	return _showTitleBar;
}

//---------------------------------------------------------------------
void _SwGuiCompDockWidget::setShowTitleBar( bool val )
{
	_showTitleBar = val;

	if (!_showTitleBar)
	{				
		QWidget* lEmptyWidget = new QWidget();
		GetDockWidget().setTitleBarWidget(lEmptyWidget);
	}
	else
		GetDockWidget().setTitleBarWidget(_titleBar);

}

