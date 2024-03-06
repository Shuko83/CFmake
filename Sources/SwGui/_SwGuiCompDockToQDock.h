/*!
 \file _SwGuiCompDockToQDock.h
 \brief Implementation of the Class _SwGuiCompDockToQDock
 \version 1.0
 \date
 \author
*/

#ifndef __SwGuiCompDockToQDock_H
#define __SwGuiCompDockToQDock_H

#include "SwAssistedComponent.h"
#include "ISwQDockWidget.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwFoundation;

/*!
	\class _SwGuiDockToQDock 
	\brief _SwGuiDockToQDock
*/
class _SwGuiCompDockToQDock : public SwAssistedComponent, public ISwQDockWidget
{
	Q_OBJECT

public:
	_SwGuiCompDockToQDock();
	~_SwGuiCompDockToQDock();

	void initializeComponent() throw(SwException);
	void interfaceAvailable(QString interfaceName);
	void interfaceUnavailable(QString interfaceName);

	//-------------------------------------------------------------------------
    // Interface ISwQDockWidget
    //-------------------------------------------------------------------------
    /*! \brief Renvoie le dockwidget
    \return le dockwidget */
	virtual QDockWidget & GetDockWidget();
	virtual bool GetForceFloating() const {return false;}

private:
	QDockWidget * _dock;

};

#endif
