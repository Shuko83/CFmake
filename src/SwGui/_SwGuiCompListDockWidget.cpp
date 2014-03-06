/*!
 \file _SwGuiCompListDockWidget.cpp
 \brief Implementation of the Class _SwGuiCompListDockWidget generant une list de SwDockWidget
 \version 1.0
 \date
 \author
*/

//#include <SwApplication.h>
//#include <SwMacros.h>
#include "_SwGuiCompListDockWidget.h"
#include "ISwDockWidget.h"

//using namespace StreamWork::SwCore;
//using namespace StreamWork::SwGui;

#define DOCK_INTERFACE_NAME "DockWidget_%1"

/*! \brief Constructeur */
_SwGuiCompListDockWidget::_SwGuiCompListDockWidget() :
	SwAssistedComponent(),
	_dockNumber(0)
{
	setPropertyServiceAvaibility(true);
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);
}

/*! \brief Destructeur */
_SwGuiCompListDockWidget::~_SwGuiCompListDockWidget()
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	unprovideInterface("ISwListDockWidget");
}

void _SwGuiCompListDockWidget::initializeComponent() throw(SwException)
{
	//---------------------------------------------------
	//				Gestion des Interfaces
	//---------------------------------------------------
	provideInterface<ISwListDockWidget>("ISwListDockWidget", (ISwListDockWidget*)this);

	//---------------------------------------------------
	//				Gestion des proprietes
	//---------------------------------------------------
	createPropertiesForThisObject("DockWidget");
}

void _SwGuiCompListDockWidget::interfaceAvailable(QString interfaceName)
{
	//DockWidget
	ISwDockWidget * widget = getInterface<ISwDockWidget>(interfaceName);
	if (widget)
	{
		//Ajout du dock
		_listDockWidget.push_back(&(widget->GetDockWidget()));
		//Notification
		foreach(ISwListDockWidgetListener * listener, _listListener)
			listener->addDockWidget(widget);
	}
	return;
}


void _SwGuiCompListDockWidget::interfaceUnavailable(QString interfaceName)
{
	//DockWidget
	ISwDockWidget * widget = getInterface<ISwDockWidget>(interfaceName);
	if (widget)
	{
		//Suppression du dock
		_listDockWidget.removeOne(&(widget->GetDockWidget()));
		//Notification
		foreach(ISwListDockWidgetListener * listener, _listListener)
			listener->removeDockWidget(widget);
	}
	return;
}

void _SwGuiCompListDockWidget::setDockNumber(uint nb)
{
	if (_dockNumber != nb)
	{
		//S'il faut supprimer des interfaces
		if (nb < _dockNumber)
		{
			for (uint i = nb; i < _dockNumber; i++)
                unconsummeInterface(QString(DOCK_INTERFACE_NAME).arg(i));
		}

		//S'il faut ajouter des interfaces
		else if (nb > _dockNumber)
		{
			for (uint i = _dockNumber; i < nb; i++)
				consummeInterface<ISwDockWidget>(QString(DOCK_INTERFACE_NAME).arg(i));
		}

		_dockNumber = nb;
	}
}

uint _SwGuiCompListDockWidget::getDockNumber()
{
	return _dockNumber;
}

void _SwGuiCompListDockWidget::setName(QString name)
{
	if (name != _name)
		_name = name;
}

//----------------------------------------------------------------------------
//Interface ISwListDockWidget
//----------------------------------------------------------------------------
QList<SwDockWidget_DockWidget*> _SwGuiCompListDockWidget::GetListDockWidget()
{
	return _listDockWidget;
}

void _SwGuiCompListDockWidget::addDockWidgetListener(ISwListDockWidgetListener * observer)
{
	if (observer && !_listListener.contains(observer))
		_listListener.push_back(observer);
}

void _SwGuiCompListDockWidget::removeDockWidgetListener(ISwListDockWidgetListener * observer)
{
	if (observer && _listListener.contains(observer))
		_listListener.removeOne(observer);
}

QString _SwGuiCompListDockWidget::getName()
{
	return _name;
}