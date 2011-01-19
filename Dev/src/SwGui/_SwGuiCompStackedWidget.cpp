/*!
 \file _SwGuiCompStackedWidget.cpp
 \brief Implementation of the Class _SwGuiCompStackedWidget generant un QWidget
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwGuiCompStackedWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;


#define CL_WIDGET_INTERFACE_NAME "Widget_%1"


/*****************************************************************************/
_SwGuiCompStackedWidget::_SwGuiCompStackedWidget(): SwComponent_Class()
{
    _provider_service=NULL;
    _consumer_service=NULL;
    _properties_service=NULL;
    _stackedWidget=NULL;
    _stackedWidgets_nb=0;
    _tmp_handle_widget=NULL;
}

/*****************************************************************************/
_SwGuiCompStackedWidget::~_SwGuiCompStackedWidget()
{
    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());

    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

	//Clear stacked_widget but do not delete widget
	while(_stackedWidget->count() > 0)
		_stackedWidget->removeWidget(_stackedWidget->widget(0));

    delete _stackedWidget;
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::InitializeResources() throw(SwException) 
{
    //Creation des service
    _consumer_service		= new SwInterfaces_Consumer_Class(this) ;
    _provider_service		= new SwInterfaces_Provider_Class(this) ;
    _properties_service		= new SwProperties_Class(this);

    //Creation de l'interface principale
    _stackedWidget			= new QStackedWidget();

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwWidget
	_provider_service->RegisterProvidedInterface<ISwWidget>("Widget",(ISwWidget *)this);
	_provider_service->RegisterProvidedInterface<ISwStackedWidget_Controler>("StackedWidget_Controler",(ISwStackedWidget_Controler *)this);


    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    _properties_service->CreatePropertiesForQObject(_stackedWidget,"QStackedWidget");

    //Gestion des widgets
    _stackedWidgets_nb_property=_properties_service->CreateProperty<uint>("nb_page");
    if (_stackedWidgets_nb_property==NULL) 
	{
        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Warning,QString("Fail to register nb_page property\n"));
    }
    _stackedWidgets_nb_property->SetDescription("Define how many ISwWidget interfaces this component accept");
    _stackedWidgets_nb_property->SetValue(QVariant(_stackedWidgets_nb));
    _stackedWidgets_nb_property->GetOnChangeSignal().iconnect(*this,&_SwGuiCompStackedWidget::OnPropertyChange);


    if (SW_APP->IsVerbose()) 
		SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwFrameWidget done\n"));

}

/*****************************************************************************/
void _SwGuiCompStackedWidget::OnPropertyChange(ISwProperty * property) 
{
    uint val;
    QString interface_name;

    if (_stackedWidgets_nb_property==property) 
	{
        val=property->GetValue().toUInt();

        if (val==_stackedWidgets_nb) 
			return;

        if (val<_stackedWidgets_nb) 
		{
            for (uint i=val;i<_stackedWidgets_nb;i++) 
			{
                interface_name=QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _consumer_service->UnregisterConsumedInterface(interface_name);

				if(_stackedWidget && _stackedWidget->widget(i))
					_stackedWidget->removeWidget(_stackedWidget->widget(i));
				notifyObserver();

            }
        } 
		else 
		{
            for (uint i=_stackedWidgets_nb;i<val;i++) 
			{
                interface_name=QString(CL_WIDGET_INTERFACE_NAME).arg(i);
                _widgets.insert(interface_name,(ISwWidget *)NULL);
                _consumer_service->RegisterConsumedInterface<ISwWidget>(interface_name,&_tmp_handle_widget);
				if(_stackedWidget)
					_stackedWidget->addWidget(new QWidget);
				notifyObserver();

            }
        }
        _stackedWidgets_nb=val;
    }
}

//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------

/*****************************************************************************/
void _SwGuiCompStackedWidget::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{
    QMap<QString,ISwWidget *>::iterator widget_it;

    //Si c'est un menu
    widget_it=_widgets.find(interface_name);

    if (widget_it!=_widgets.end()) 
	{
        if (widget_it.value()!=NULL) 
		{
            //Et qu'il etait defini, on le suprime de la stack
			int indexWidget =  _stackedWidget->indexOf(&widget_it.value()->GetWidget());
            _stackedWidget->removeWidget(&widget_it.value()->GetWidget());
			widget_it.value()->GetWidget().setParent(NULL);

			_stackedWidget->insertWidget(indexWidget,new QWidget);

			notifyObserver();
 
             //Si c'est un widget observable, on se mets en observer
             ISwWidget2 * wo=dynamic_cast<ISwWidget2 *>(widget_it.value());
             if (wo!=0) 
 			{
                 wo->UnregisterISwWidgetObserver(this);
 			} 
            //Fin
            widget_it.value()=NULL;
        }
        return;
    }


}

/*****************************************************************************/
void _SwGuiCompStackedWidget::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) 
{

    QMap<QString,ISwWidget *>::iterator widget_it;

    //Si c'est un widget
    widget_it=_widgets.find(interface_name);

    if (widget_it!=_widgets.end()) 
	{
        if (widget_it.value()==NULL && _tmp_handle_widget!=NULL) 
		{
            //Et qu'il etait non defini, on l'enregistre et l'ajoute au stack
            widget_it.value()=_tmp_handle_widget;

			 int indexWidget = interface_name.remove("Widget_").toInt();

			QWidget * temporyW = _stackedWidget->widget(indexWidget);
			_stackedWidget->removeWidget(temporyW);
			delete temporyW;
			temporyW = NULL;

            _stackedWidget->insertWidget(indexWidget,&_tmp_handle_widget->GetWidget());
			notifyObserver();

             //Si c'est un widget observable, on se mets en observer
            ISwWidget2 * wo=dynamic_cast<ISwWidget2 *>(_tmp_handle_widget);
             if (wo!=0) 
 			{
                 wo->RegisterISwWidgetObserver(this);
             } 
         }
        return;
    }
}

//---------------------------------------------------------------------
// Interface ISwWidget2
//---------------------------------------------------------------------

/*****************************************************************************/
QWidget & _SwGuiCompStackedWidget::GetWidget() 
{
    return *_stackedWidget;
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::RegisterISwWidgetObserver(ISwWidget2_Observer * o) 
{
    _wObservers.push_back(o);
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::UnregisterISwWidgetObserver(ISwWidget2_Observer * o) 
{
    _wObservers.removeOne(o);
}

//---------------------------------------------------------------------
// InterfaceISwWidget2_Observer
//---------------------------------------------------------------------

/*****************************************************************************/
void _SwGuiCompStackedWidget::OnBringToFrontRequest(ISwWidget * w) 
{
    int index=_stackedWidget->indexOf(&(w->GetWidget()));
    if (index>=0 && _stackedWidget && index <= _stackedWidget->count() )
        _stackedWidget->setCurrentIndex(index);

    for(int i=0;i<_wObservers.count();i++) 
	{
        _wObservers[i]->OnBringToFrontRequest(this);
    }
}

//---------------------------------------------------------------------
// InterfaceISwWidget2_Observer
//---------------------------------------------------------------------

/*****************************************************************************/
void _SwGuiCompStackedWidget::setCurrentIndex( int pageIndex )
{
	if(pageIndex >=0 && _stackedWidget && pageIndex < _stackedWidget->count())
	{
		_stackedWidget->setCurrentIndex(pageIndex);
	}
}

/*****************************************************************************/
int _SwGuiCompStackedWidget::getNbPage()
{
	if(_stackedWidget)
	{
		return _stackedWidget->count();
	}

	return 0;
}

/*****************************************************************************/
int _SwGuiCompStackedWidget::getCurrentIndex()
{
	if(_stackedWidget)
	{
		return _stackedWidget->currentIndex();
	}

	return 0;
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::registerStackedWidgetObserver( ISwStackedWidget_Observer *o )
{
	 _wStackedWidgetsObservers.push_back(o);
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::unRegisterStackedWidgetObserver( ISwStackedWidget_Observer *o )
{
    _wStackedWidgetsObservers.removeOne(o);
}

/*****************************************************************************/
void _SwGuiCompStackedWidget::notifyObserver()
{
	for(int i=0;i<_wStackedWidgetsObservers.count();i++) 
	{
		_wStackedWidgetsObservers[i]->notify();
	}
}

/*****************************************************************************/
QString _SwGuiCompStackedWidget::getWidgetName( int pageIndex )
{	
 	if(_stackedWidget)
 	{
 		//Try to get the widget from the stackedWidget
 		QWidget * t_w = _stackedWidget->widget(pageIndex);
 
 		if(t_w != NULL)
 		{
			if(!t_w->windowTitle().isEmpty())
 				return t_w->windowTitle();
			else
				return QString("Veuillez renseigner un windows title pour l'index %1").arg(pageIndex);
 		}
 	}

	//
	return QString("Veuillez renseigner un windows title pour l'index  :%1").arg(pageIndex);

}

