/*!
 \file _SwGuiCompQDockToDock.h
 \brief Implementation of the Class _SwGuiCompQDockToDock
 \version 1.0
 \date
 \author
*/

#ifndef __SwGuiCompQDockToDock_H
#define __SwGuiCompQDockToDock_H

#include "SwAssistedComponent.h"
#include "ISwDockWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwFoundation;

/*!
	\class _SwGuiCompQDockToDock 
	\brief _SwGuiCompQDockToDock
*/
class _SwGuiCompQDockToDock : public SwAssistedComponent, public ISwDockWidget
{
	Q_OBJECT

public:
	_SwGuiCompQDockToDock();
	~_SwGuiCompQDockToDock();

	void initializeComponent() throw(SwException);
	void interfaceAvailable(QString interfaceName);
	void interfaceUnavailable(QString interfaceName);

	//-------------------------------------------------------------------------
    // Interface ISwDockWidget
    //-------------------------------------------------------------------------
    /*! \brief Renvoie le dockwidget
    \return le dockwidget */
	virtual SwDockWidget_DockWidget & GetDockWidget();

private:
	SwDockWidget_DockWidget * _dock;

};

#endif