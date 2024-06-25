/*!
 \file _SwGuiCompDockToQDock.cpp
 \brief Implementation of the Class _SwGuiCompDockToQDock
 \version 1.0
 \date
 \author
*/

#include "_SwGuiCompDockToQDock.h"
#include "ISwQDockWidget.h"
#include "_SwGuiCompDockWidget.h"

#define DOCKWIDGET_INTERFACE_NAME	"DockWidget"
#define QDOCKWIDGET_INTERFACE_NAME	"QDockWidget"

//-----------------------------------------------------------------------------
_SwGuiCompDockToQDock::_SwGuiCompDockToQDock(): SwAssistedComponent(), _dock(NULL)
{
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);
}

//-----------------------------------------------------------------------------
_SwGuiCompDockToQDock::~_SwGuiCompDockToQDock()
{
	unconsumeInterface(DOCKWIDGET_INTERFACE_NAME);
	unprovideInterface(QDOCKWIDGET_INTERFACE_NAME);
}

//-----------------------------------------------------------------------------
void _SwGuiCompDockToQDock::initializeComponent() throw(SwException)
{
	consumeInterface<ISwDockWidget>(DOCKWIDGET_INTERFACE_NAME);
	provideInterface<ISwQDockWidget>(QDOCKWIDGET_INTERFACE_NAME,(ISwQDockWidget*)this);
}

//-----------------------------------------------------------------------------
void _SwGuiCompDockToQDock::interfaceAvailable(QString interfaceName)
{
	if(interfaceName == DOCKWIDGET_INTERFACE_NAME)
	{
		ISwDockWidget * dockwidget = getInterface<ISwDockWidget>(interfaceName);
		if (dockwidget)
		{
			QWidget * widget = dockwidget->GetDockWidget().getWidget();
			if (widget)
			{
				_dock = new QDockWidget();
				_dock->setWidget(widget);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void _SwGuiCompDockToQDock::interfaceUnavailable(QString interfaceName)
{
	if(interfaceName == DOCKWIDGET_INTERFACE_NAME)
	{
		_dock->setParent(NULL);
		_dock = 0;
	}
}

//-----------------------------------------------------------------------------
QDockWidget & _SwGuiCompDockToQDock::GetDockWidget()
{
	return (*_dock);
}
