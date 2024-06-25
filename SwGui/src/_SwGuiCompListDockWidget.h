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
using namespace StreamWork::SwCore;

/*!
	\class _SwGuiCompListDockWidget 
	\brief _SwGuiCompListDockWidget generant une liste de SwDockWidget
*/
class _SwGuiCompListDockWidget : public SwAssistedComponent, public ISwListDockWidget
{
	Q_OBJECT
	Q_PROPERTY(uint widgetNumber READ getWidgetNumber WRITE setWidgetNumber)
	Q_PROPERTY(QString name READ getDockWidgetName WRITE setName)

public:
	_SwGuiCompListDockWidget();
	~_SwGuiCompListDockWidget();

	void setWidgetNumber(uint nb);
	uint getWidgetNumber();

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
	virtual QString getDockWidgetName();

private:
	unsigned int _widgetNumber;

	QMap<QString, SwDockWidget_DockWidget *> _dockwidgets;
	QList<ISwListDockWidgetListener*> _listListener;
	QString _name;

};

#endif
