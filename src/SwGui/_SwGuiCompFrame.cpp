/*!
 \file _SwGuiCompFrame.cpp
 \brief Implementation of the Class _SwGuiCompFrame generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompFrame.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"


/*! \brief Constructeur */
_SwGuiCompFrame::_SwGuiCompFrame(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _frame=NULL;
    _widgets_nb=0;
    _tmp_handle_widget=NULL;
    _registered_widgets_nb=0;
    _handle_layout=NULL;
    _is_layout_mode=false;
}
/*! \brief Destructeur */
_SwGuiCompFrame::~_SwGuiCompFrame(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _frame;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompFrame::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _frame=new QFrame();
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwWidget>("Widget",(ISwWidget *)this);

    //Importation de l'interface ISwWidget (RAF puisque par defaut pas de widget) et ISwLayout (exclusif)
    _consumer_service->RegisterConsumedInterface<ISwLayout>("Layout",&_handle_layout);

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    _properties_service->CreatePropertiesForQObject(_frame,"QFrame");

    //Gestion des widgets
    _widgets_nb_property=_properties_service->CreateProperty<uint>("nb_widgets");
    if (_widgets_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_widgets property\n"));
    }
    _widgets_nb_property->SetDescription("Define how many ISwWidget interfaces this component accept");
    _widgets_nb_property->SetValue(QVariant(_widgets_nb));
    _widgets_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompFrame::OnPropertyChange);


    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwFrameWidget done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompFrame::OnPropertyChange(ISwProperty * property) {
    uint val;
    QString interface_name;

    if (_widgets_nb_property==property && _is_layout_mode==false) {
        val=property->GetValue().toUInt();
        if (val==_widgets_nb) return;
        if (val<_widgets_nb) {
            for (uint i=val;i<_widgets_nb;i++) {
                interface_name=QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_widgets_nb;i<val;i++) {
                interface_name=QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _widgets.insert(interface_name,(ISwWidget *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwWidget>(interface_name,&_tmp_handle_widget);
            }
        }
        _widgets_nb=val;
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompFrame::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwWidget *>::iterator widget_it;
    //Si c'est un menu
    widget_it=_widgets.find(interface_name);
    if (widget_it!=_widgets.end()) {
        if (widget_it.value()!=NULL) {
            //Et qu'il etait defini, on le detache de la widgetbar
            widget_it.value()->GetWidget().setParent(NULL);
            widget_it.value()=NULL;
            _registered_widgets_nb--;
            if (_registered_widgets_nb==0) _consumer_service->RegisterConsumedInterface<ISwLayout>("Layout",&_handle_layout);
        }
        return;
    }
    //Si c'est un layout
    if (_handle_layout!=NULL) {
        _handle_layout->LiberateLayout();
        _is_layout_mode=false;
        OnPropertyChange(_widgets_nb_property);
        _properties_service->ChangePropertyVisibility("nb_widgets",true);
    }
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompFrame::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwWidget *>::iterator widget_it;

    //Si c'est un widget
    widget_it=_widgets.find(interface_name);
    if (widget_it!=_widgets.end()) {
        if (widget_it.value()==NULL && _tmp_handle_widget!=NULL) {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la widgetbar
            widget_it.value()=_tmp_handle_widget;
            _tmp_handle_widget->GetWidget().setParent(_frame);
            _registered_widgets_nb++;
            if (_registered_widgets_nb==1) _consumer_service->UnregisterConsumedInterface("Layout");
        }
        return;
    }
    if (_handle_layout!=NULL) {
        _widgets_nb_property->SetValue(QVariant((uint)0));
        _is_layout_mode=true;
        _properties_service->ChangePropertyVisibility("nb_widgets",false);
        _frame->setLayout(&(_handle_layout->GetLayout()));
    }
}
//---------------------------------------------------------------------
// Interface ISwQMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget & _SwGuiCompFrame::GetWidget() {
    return *_frame;
}

