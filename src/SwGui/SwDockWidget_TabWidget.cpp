#include "SwDockWidget_TabWidget.h"

#include <QStackedWidget>

//-----------------------------------------------------------------------------
SwDockWidget_TabWidget::SwDockWidget_TabWidget(QWidget * parent)
: QTabWidget(parent)
{
	setObjectName("TabWidget");
	setMovable(true);
	//Les onglets sont positionnes en bas
	QTabWidget::setTabPosition(QTabWidget::North);
}

//-----------------------------------------------------------------------------
SwDockWidget_TabWidget::~SwDockWidget_TabWidget()
{

}

//-----------------------------------------------------------------------------
//Ajout d'un onglet
int SwDockWidget_TabWidget::addTab(QWidget * widget, int pos)
{
	if (widget)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		if (dock)
		{
			dock->setInTab(true);

			if (pos == -1)
				pos = QTabWidget::addTab(dock, dock->windowIcon(), dock->getTitle());
			else
				QTabWidget::insertTab(pos, dock, dock->windowIcon(), dock->getTitle());
			setSize();

			return pos;
		}
	}

	return -1;
}

//-----------------------------------------------------------------------------
//Mise a jour de la taille du TabWidget
void SwDockWidget_TabWidget::setSize()
{
	int num = this->count();
	int width = 0;
	int height = 0;

	//Redimensionnement si 2 elements (creation du tabwidget)
	if (num == 2)
	{
		//Recuperation de la taille du plus grand element
		for (int i = 0; i < num; i++)
		{
			SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(this->widget(i));
			if (dock)
			{
				width = qMax(width, dock->getRawSize().width());
				height = qMax(height, dock->getRawSize().height());
			}
		}

		if (width && height)
		{
			//Affectation de la plus grande taille au widget
			QTabWidget::resize(width, height);
		}
	}
}