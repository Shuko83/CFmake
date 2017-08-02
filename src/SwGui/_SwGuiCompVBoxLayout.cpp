/*!
 \file _SwGuiCompVBoxLayout.cpp
 \brief Implementation of the Class _SwGuiCompVBoxLayout generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompVBoxLayout.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"
#define CL_LAYOUT_INTERFACE_NAME "Layout_%1"


/*! \brief Constructeur */
_SwGuiCompVBoxLayout::_SwGuiCompVBoxLayout(): SwComponent_Class(){
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _layout=NULL;
    _widgets_nb=0;
    _widgets_nb_property=NULL;
    _layouts_nb=0;
    _layouts_nb_property=NULL;
    _nb_childs=0;
    _margin=0;
    _spacing=6;
    _enableSpacer=false;
    _spacer=new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Expanding);
}
/*! \brief Destructeur */
_SwGuiCompVBoxLayout::~_SwGuiCompVBoxLayout(){
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    if (_layout!=NULL) 
        delete _layout;
    if (_spacer!=NULL) 
        delete _spacer;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwGuiCompVBoxLayout::InitializeResources() throw(SwException) {
    //Creation des service
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    //Creation de l'interface principale
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwWidget
    _provider_service->RegisterProvidedInterface<ISwLayout>("Layout",(ISwLayout *)this);
     
    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //Enregistrement des layouts
    _margin_property=_properties_service->CreateProperty<int>("QVBoxLayout.margin");
    _margin_property->SetDescription("Define margin for layout");  
    _margin_property->SetValue(QVariant(_margin));
    _margin_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompVBoxLayout::OnPropertyChange);

    _spacing_property=_properties_service->CreateProperty<int>("QVBoxLayout.spacing");
    _spacing_property->SetDescription("Define spacing for layout");  
    _spacing_property->SetValue(QVariant(_spacing));
    _spacing_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompVBoxLayout::OnPropertyChange);

    //Gestion des widgets
    _widgets_nb_property=_properties_service->CreateProperty<uint>("nb_widgets");
    if (_widgets_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_widgets property\n"));
    }
    _widgets_nb_property->SetDescription("Define how many ISwWidget interfaces this component accept");  
    _widgets_nb_property->SetValue(QVariant(_widgets_nb));
    _widgets_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompVBoxLayout::OnPropertyChange);

    //Gestion des layouts
    _layouts_nb_property=_properties_service->CreateProperty<uint>("nb_layouts");
    if (_layouts_nb_property==NULL) {
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_widgets property\n"));
    }
    _layouts_nb_property->SetDescription("Define how many ISwLayout interfaces this component accept");  
    _layouts_nb_property->SetValue(QVariant(_layouts_nb));
    _layouts_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompVBoxLayout::OnPropertyChange);


    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwGuiVBoxLayout done\n"));

    //Spacer
    _properties_service->CreatePropertiesForQObject(this,QString(),true);

}
/*! \brief Callback sur les changements de propriétés*/
void _SwGuiCompVBoxLayout::OnPropertyChange(ISwProperty * property) {
    uint val;
    QString interface_name;

    if (_widgets_nb_property==property) {
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
                _consumer_service->RegisterConsumedInterface<ISwWidget>(interface_name,&_tmp_handle_widget);
            }
        }
        _widgets_nb=val;
    }

    if (_layouts_nb_property==property) {
        val=property->GetValue().toUInt();
        if (val==_layouts_nb) return;
        if (val<_layouts_nb) {
            for (uint i=val;i<_layouts_nb;i++) {
                interface_name=QString(CL_LAYOUT_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);
            }
        } else {
            for (uint i=_layouts_nb;i<val;i++) {
                interface_name=QString(CL_LAYOUT_INTERFACE_NAME).arg(i);
                _consumer_service->RegisterConsumedInterface<ISwLayout>(interface_name,&_tmp_handle_layout);
            }
        }
        _layouts_nb=val;
    }
    if (_margin_property==property) {
        _margin=property->GetValue().toInt(); 
        if (_layout!=NULL) _layout->setMargin(_margin);
    }
    if (_spacing_property==property) {
        _spacing=property->GetValue().toInt(); 
        if (_layout!=NULL) _layout->setSpacing(_spacing);
    }

}
/*! \brief getter*/
bool _SwGuiCompVBoxLayout::getEnableSpacer() {
    return _enableSpacer;
}
/*! \brief setter*/
void _SwGuiCompVBoxLayout::setEnableSpacer(bool enable) {
    if (_enableSpacer==enable)
        return;
    _enableSpacer=enable; 
    if (_layout!=NULL) {
        if (_enableSpacer) {
            _layout->addSpacerItem(_spacer);
        } else {
            _layout->removeItem(_spacer); 
        }
    }
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/*! \brief Avant changement de la disponibilité de l'interface */
void _SwGuiCompVBoxLayout::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    
    QMap<QString,ISwWidget *>::iterator widget_it;
    QMap<QString,ISwLayout *>::iterator layout_it;
    
    //Si c'est un menu
    widget_it=_widgets.find(interface_name);
    if (widget_it!=_widgets.end()) {
        if (widget_it.value()!=NULL) {
            //Et qu'il etait defini, on le detache de la widgetbar
            if (_layout!=NULL) widget_it.value()->GetWidget().setParent(NULL); 
            widget_it.value()=NULL;
            _widgets.erase(widget_it);
            _nb_childs--;
            int index=_ordered_childrens.indexOf(interface_name);
            if (index!=-1) _ordered_childrens.removeAt(index);
        }
    } 
    //Si c'est un layout
    layout_it=_layouts.find(interface_name);
    if (layout_it!=_layouts.end()) {
        if (layout_it.value()!=NULL) {
            //Et qu'il etait defini, on le detache de la widgetbar
            if (_layout!=NULL) layout_it.value()->LiberateLayout(); 
            layout_it.value()=NULL;
            _layouts.erase(layout_it);
            _nb_childs--;
            int index=_ordered_childrens.indexOf(interface_name);
            if (index!=-1) _ordered_childrens.removeAt(index);
        }
    } 
}
/*! \brief Apres changement de la disponibilité de l'interface */
void _SwGuiCompVBoxLayout::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
    QMap<QString,ISwWidget *>::iterator widget_it;
    QMap<QString,ISwLayout *>::iterator layout_it;
    QString interface_header=interface_name;
    
    interface_header.truncate(1);
    //Si c'est un widget
    widget_it=_widgets.find(interface_name);
    if (interface_header==QString("W") && widget_it==_widgets.end()  && _tmp_handle_widget!=NULL) {
        _widgets.insert(interface_name,_tmp_handle_widget);
        if (_layout!=NULL) {
            if (_enableSpacer) {
                _layout->insertWidget(_layout->count()-1,&_tmp_handle_widget->GetWidget());
            } else {
                _layout->addWidget(&_tmp_handle_widget->GetWidget());
            }
        }
        _nb_childs++;
        _ordered_childrens.push_back(interface_name);
        return;
    } 
    //Si c'est un layout
    layout_it=_layouts.find(interface_name);
    if (interface_header==QString("L") && layout_it==_layouts.end() && _tmp_handle_layout!=NULL) {
        _layouts.insert(interface_name,_tmp_handle_layout);
        if (_layout!=NULL) {
            if (_enableSpacer) {
                _layout->insertLayout(_layout->count()-1,&_tmp_handle_layout->GetLayout());
            } else {
                _layout->addLayout(&_tmp_handle_layout->GetLayout());
            }
        }
        _nb_childs++;
        _ordered_childrens.push_back(interface_name);
        return;
    } 
}
//---------------------------------------------------------------------
// Interface ISwLayout
//---------------------------------------------------------------------
/*! \brief Renvoie le menu
\return le menu */
QLayout & _SwGuiCompVBoxLayout::GetLayout(){
    QString interface_header;
    QMap<QString,ISwWidget *>::iterator widget_it;
    QMap<QString,ISwLayout *>::iterator layout_it;

    if (_layout==NULL) {
        //Reconstruction du layout
        _layout=new QVBoxLayout();
        _layout->setMargin(_margin);
        _layout->setSpacing(_spacing);
        //Mise en place des enfants
        for(int i=0;i<_ordered_childrens.count();i++) {
            interface_header=_ordered_childrens[i];
            interface_header.truncate(1);
            widget_it=_widgets.find(_ordered_childrens[i]);
            if (interface_header==QString("W") && widget_it!=_widgets.end()) {
                _layout->addWidget(&widget_it.value()->GetWidget());
            } 
            //Si c'est un layout
            layout_it=_layouts.find(_ordered_childrens[i]);
            if (interface_header==QString("L") && layout_it==_layouts.end()) {
                _layout->addLayout(&layout_it.value()->GetLayout());
            } 
        }
        //Spacer
        if (_enableSpacer) {
            _layout->addSpacerItem(_spacer);
        } else {
            _layout->removeItem(_spacer); 
        }
    }
    return *_layout;

}
/*! \brief Liberation du layout */
void _SwGuiCompVBoxLayout::LiberateLayout(){
    QMap<QString,ISwWidget *>::iterator widget_it;
    QMap<QString,ISwLayout *>::iterator layout_it;

    for(widget_it=_widgets.begin();widget_it!=_widgets.end();widget_it++) {
        widget_it.value()->GetWidget().setParent(NULL);     
    }
    for(layout_it=_layouts.begin();layout_it!=_layouts.end();layout_it++) {
        layout_it.value()->LiberateLayout();   
    }
    if (_enableSpacer) {
        _layout->removeItem(_spacer); 
    }
    delete _layout;
    _layout=NULL;
}

