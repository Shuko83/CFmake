/*!
 \file _SwGuiCompQDockToDock.cpp
 \brief Implementation of the Class _SwGuiCompQDockToDock
 \version 1.0
 \date
 \author
*/

#include "_SwGuiCompQDockToDock.h"
#include "ISwQDockWidget.h"

#define DOCKWIDGET_INTERFACE_NAME	"DockWidget"
#define QDOCKWIDGET_INTERFACE_NAME	"QDockWidget"

//-----------------------------------------------------------------------------
_SwGuiCompQDockToDock::_SwGuiCompQDockToDock(): SwAssistedComponent(), _dock(NULL)
{
	setConsumerServiceAvaibility(true);
	setProviderServiceAvaibility(true);
}

//-----------------------------------------------------------------------------
_SwGuiCompQDockToDock::~_SwGuiCompQDockToDock()
{
	unconsumeInterface(QDOCKWIDGET_INTERFACE_NAME);
	unprovideInterface(DOCKWIDGET_INTERFACE_NAME);
}

//-----------------------------------------------------------------------------
void _SwGuiCompQDockToDock::initializeComponent() throw(SwException)
{
	consumeInterface<ISwQDockWidget>(QDOCKWIDGET_INTERFACE_NAME);
	provideInterface<ISwDockWidget>(DOCKWIDGET_INTERFACE_NAME,(ISwDockWidget*)this);
}

//-----------------------------------------------------------------------------
void _SwGuiCompQDockToDock::interfaceAvailable(QString interfaceName)
{
	if(interfaceName == QDOCKWIDGET_INTERFACE_NAME)
	{
		ISwQDockWidget * idock = getInterface<ISwQDockWidget>(interfaceName);
		if (idock)
		{
			QWidget * widget = idock->GetDockWidget().widget();
			if (widget)
			{
				_dock = new SwDockWidget_DockWidget();
				_dock->setWidget(widget);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void _SwGuiCompQDockToDock::interfaceUnavailable(QString interfaceName)
{
	if(interfaceName == QDOCKWIDGET_INTERFACE_NAME)
	{
		_dock->setParent(NULL);
		_dock = 0;
	}
}

//-----------------------------------------------------------------------------
SwDockWidget_DockWidget & _SwGuiCompQDockToDock::GetDockWidget()
{
	//Attention ca risque de crasher si on connect la sortie avant l'entrée
	if (_dock)
		return (*_dock);

	//Pour éviter le warning
	return *(new SwDockWidget_DockWidget());
}
