/*!
 \file _SwGuiCompListDockWidget.h
 \brief Implementation of the Class _SwGuiCompListDockWidget generant une liste de SwDockWidget
 \version 1.0
 \date
 \author
*/

#ifndef __SwGuiCompListDockWidget_H
#define __SwGuiCompListDockWidget_H

/*
  * INCLUDES GLOBAUX
  */
/*
  * INCLUDES LOCAUX
  */

#include "SwAssistedComponent.h"
#include "ISwListDockWidget.h"

using namespace StreamWork::SwFoundation;
using namespace StreamWork::SwGui;

/*!
	\class _SwGuiCompListDockWidget 
	\brief _SwGuiCompListDockWidget generant une liste de SwDockWidget
*/
class _SwGuiCompListDockWidget : public SwAssistedComponent, public ISwListDockWidget
{
	Q_OBJECT
	Q_PROPERTY(uint _dockNumber READ getDockNumber WRITE setDockNumber)
	Q_PROPERTY(QString _name READ getName WRITE setName)

public:
	_SwGuiCompListDockWidget();
	~_SwGuiCompListDockWidget();

	void setDockNumber(uint nb);
	uint getDockNumber();

	void setName(QString name);

	//----------------------------------------------------------------------------
	virtual void initializeComponent() throw(SwException);

	virtual void interfaceAvailable(QString interfaceName);
	virtual void interfaceUnavailable(QString interfaceName);

	//----------------------------------------------------------------------------
	//Interface ISwListDockWidget
	//----------------------------------------------------------------------------
	virtual QList<SwDockWidget_DockWidget*> GetListDockWidget();
	virtual void addDockWidgetListener(ISwListDockWidgetListener * observer);
	virtual void removeDockWidgetListener(ISwListDockWidgetListener * observer);
	virtual QString getName();

private:
	unsigned int _dockNumber;
	QList<SwDockWidget_DockWidget*> _listDockWidget;
	QList<ISwListDockWidgetListener*> _listListener;
	QString _name;

};

#endif