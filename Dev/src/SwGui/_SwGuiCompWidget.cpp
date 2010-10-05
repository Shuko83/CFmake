/*!
 \file _SwGuiCompWidget.cpp
 \brief Implementation of the Class _SwGuiCompWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QDesktopWidget>
#include <QApplication>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"
#define SHOW_NORMAL 0
#define SHOW_CENTERED 1
#define SHOW_FULLSCREEN 2
#define SHOW_MAXIMIZED 3
#define SHOW_MINIMIZED 4


/*! \brief Constructeur */
_SwGuiCompWidget::_SwGuiCompWidget(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _widget=NULL;
    _widgets_nb=0;
    _tmp_handle_widget=NULL;
    _registered_widgets_nb=0;
    _handle_layout=NULL;
    _is_layout_mode=false;
    _show_mode.AddKey(SHOW_NORMAL,"None");
    _show_mode.AddKey(SHOW_CENTERED,"Centered");
    _show_mode.AddKey(SHOW_FULLSCREEN,"FullScreen");
    _show_mode.AddKey(SHOW_MAXIMIZED,"Maximized");
    _show_mode.AddKey(SHOW_MINIMIZED,"Minimized");
    _show_mode.FromInt(SHOW_NORMAL);

    _flags_mode.AddKey(Qt::Widget,"Widget");
    _flags_mode.AddKey(Qt::Window,"Window");
    _flags_mode.AddKey(Qt::Dialog,"Dialog");
    _flags_mode.AddKey(Qt::Sheet,"Sheet");
    _flags_mode.AddKey(Qt::Drawer,"Drawer");
    _flags_mode.AddKey(Qt::Popup,"Popup");
    _flags_mode.AddKey(Qt::Tool,"Tool");
    _flags_mode.AddKey(Qt::ToolTip,"ToolTip");
    _flags_mode.AddKey(Qt::SplashScreen,"SplashScreen");
    _flags_mode.AddKey(Qt::FramelessWindowHint,"FramelessWindowHint");
    _flags_mode.FromInt(Qt::Widget);
}
/*! \brief Destructeur */
_SwGuiCompWidget::~_SwGuiCompWidget(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    delete _widget;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompWidget::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    _widget=new QWidget();
    //_widget->setStyleSheet("QWidget {background: red}");
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

    _properties_service->CreatePropertiesForQObject(_widget,"QWidget");

    //Gestion des widgets
    _widgets_nb_property=_properties_service->CreateProperty<uint>("nb_widgets");
    if (_widgets_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_widgets property\n"));
    }
    _widgets_nb_property->SetDescription("Define how many ISwWidget interfaces this component accept");  
    _widgets_nb_property->SetValue(QVariant(_widgets_nb));
    _widgets_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompWidget::OnPropertyChange);

    _show_property=_properties_service->CreateProperty<SwEnum>("ShowMode");
    if (_show_property!=NULL) {
        QVariant variant;
        variant.setValue(_show_mode);
        _show_property->SetValue(variant);
        _show_property->SetDescription("Show Mode");  
        _show_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompWidget::OnPropertyChange);
    }


    _flags_property=_properties_service->CreateProperty<SwEnum>("FlagMode");
    if (_flags_property!=NULL) {
        QVariant variant;
        variant.setValue(_flags_mode);
        _flags_property->SetValue(variant);
        _flags_property->SetDescription("Flags Mode");  
        _flags_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompWidget::OnPropertyChange);
    }


    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiWidget done\n"));

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompWidget::OnPropertyChange(ISwProperty * property) {
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
    if (_show_property==property) {
        SwEnum showmode=_show_property->GetValue().value<SwEnum>();
        _show_mode=showmode;
        showChanged();
    }
    if (_flags_property==property) {
        SwEnum flags_mode=_flags_property->GetValue().value<SwEnum>();
        _flags_mode=flags_mode;
        _widget->setWindowFlags(Qt::WindowFlags(_flags_mode.ToInt()));
    }
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompWidget::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
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
void _SwGuiCompWidget::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwWidget *>::iterator widget_it;
    
    //Si c'est un widget
    widget_it=_widgets.find(interface_name);
    if (widget_it!=_widgets.end()) {
        if (widget_it.value()==NULL && _tmp_handle_widget!=NULL) {
            //Et qu'il etait non defini, on l'enregistre et l'attache a la widgetbar
            widget_it.value()=_tmp_handle_widget;  
            _tmp_handle_widget->GetWidget().setParent(_widget);
            _registered_widgets_nb++;
            if (_registered_widgets_nb==1) _consumer_service->UnregisterConsumedInterface("Layout");
        }
        return;
    } 
    if (_handle_layout!=NULL) {
        _widgets_nb_property->SetValue(QVariant((uint)0));
        _is_layout_mode=true;
        _properties_service->ChangePropertyVisibility("nb_widgets",false);
        _widget->setLayout(&(_handle_layout->GetLayout()));
    }
}
//---------------------------------------------------------------------
// Interface ISwMainWindow
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QWidget & _SwGuiCompWidget::GetWidget() {
    return *_widget;
}
void _SwGuiCompWidget::showChanged() {
    switch(_show_mode.ToInt()) {
        case SHOW_CENTERED: {
            _widget->showNormal();
            QDesktopWidget *desktop = QApplication::desktop();
            QRect screensize= desktop->screenGeometry(desktop->primaryScreen());
            QRect windowSize= _widget->frameGeometry();
            QPoint slefttop(screensize.left(),screensize.top());
            QPoint wlefttop;
            slefttop.setX(slefttop.x()+(screensize.width()-windowSize.width())/2);
            slefttop.setY(slefttop.y()+(screensize.height()-windowSize.height())/2);
            _widget->move(slefttop);
            
                            }
            break;
        case SHOW_FULLSCREEN:
            _widget->showFullScreen();
            break;
        case SHOW_MAXIMIZED:
            _widget->showMaximized();
            break;
        case SHOW_MINIMIZED:
            _widget->showMinimized();
            break;
        case SHOW_NORMAL:
        default:
            break;
    }
}

    void flagsChanged();
