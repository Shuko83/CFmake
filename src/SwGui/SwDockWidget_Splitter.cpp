#include "SwDockWidget_Splitter.h"
#include <QList>
#include <QResizeEvent>

//-----------------------------------------------------------------------------
SwDockWidget_Splitter::SwDockWidget_Splitter(Qt::Orientation orientation, QWidget *parent)
	: QSplitter(parent), _widget1(NULL), _widget2(NULL)
{
	setObjectName("SwDockWidget_Splitter");
	setChildrenCollapsible(false); //Les widgets ne peuvent pas etre masques completement
	setOrientation(orientation);
	connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(childrenResized()));
}

//-----------------------------------------------------------------------------
SwDockWidget_Splitter::SwDockWidget_Splitter(QWidget *parent)
	: QSplitter(parent), _widget1(NULL), _widget2(NULL)
{
	setObjectName("SwDockWidget_Splitter");
	setChildrenCollapsible(false); //Les widgets ne peuvent pas etre masques completement
	connect(this, SIGNAL(splitterMoved(int, int)), this, SLOT(childrenResized()));
}

//-----------------------------------------------------------------------------
//Gestion des evenements
bool SwDockWidget_Splitter::event( QEvent * event )
{
	switch(event->type())
	{
		case QEvent::Close:
			if (_widget1)
				_widget1->close();
			if (_widget2)
				_widget2->close();
			break;
	}

	return QSplitter::event(event);
}

//-----------------------------------------------------------------------------
SwDockWidget_Splitter::~SwDockWidget_Splitter()
{

}

//-----------------------------------------------------------------------------
//Ajout des deux widgets
void SwDockWidget_Splitter::fill(QWidget * widget1, QWidget * widget2, Qt::DockWidgetArea area, Qt::DockWidgetArea parentArea)
{
	if (widget1 && widget2)
	{
		//Premier widget
		_widget1 = widget1;
		SwDockWidget_DockWidget * dock1 = qobject_cast<SwDockWidget_DockWidget*>(widget1);
		QSize size1 = widget1->size();
		if (dock1)
		{
			size1 = dock1->getRawSize();
			if (parentArea != Qt::NoDockWidgetArea)
				dock1->setArea(parentArea);
			else
				dock1->setArea(area);
		}

		//Deuxieme widget
		_widget2 = widget2;
		SwDockWidget_DockWidget * dock2 = qobject_cast<SwDockWidget_DockWidget*>(widget2);
		QSize size2 = widget2->size();
		if (dock2)
		{
			size2 = dock2->getRawSize();
			if (parentArea != Qt::NoDockWidgetArea)
				dock2->setArea(parentArea);
			else
				dock2->setArea(area);
		}

		QSplitter::addWidget(widget1);
		QSplitter::addWidget(widget2);

		//Mise a jour des dimensions
		switch(area)
		{
			case Qt::LeftDockWidgetArea:
				size2.setWidth(qMax(size2.width() - size1.width(),10));
				break;

			case Qt::RightDockWidgetArea:
				size1.setWidth(qMax(size1.width() - size2.width(),10));
				break;

			case Qt::TopDockWidgetArea:
				size2.setHeight(qMax(size2.height() - size1.height(),10));
				break;

			case Qt::BottomDockWidgetArea:
				size1.setHeight(qMax(size1.height() - size2.height(),10));
				break;
		}

		_widget1_Size = size1;
		_widget2_Size = size2;
		updateSizes(_widget1_Size, _widget2_Size);
	}
}

//-----------------------------------------------------------------------------
//Ajout d'un widget
void SwDockWidget_Splitter::addWidget(QWidget * widget, int pos, Qt::DockWidgetArea area)
{
	if (widget)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		QSize size = widget->size();

		if (dock)
		{
			size = dock->getRawSize();
			if (area != Qt::NoDockWidgetArea)
				dock->setArea(area);
		//	setStretchFactor(pos, 0);
		}
		//else
		//	setStretchFactor(pos, 1);

		if (pos == 0)
		{
			_widget1 = widget;
			_widget1_Size = size;
		}
		else
		{
			_widget2 = widget;
			_widget2_Size = size;
		}
	}
	
	QSplitter::insertWidget(pos, widget);
	
	updateSizes(_widget1_Size, _widget2_Size);
}

//-----------------------------------------------------------------------------
//Remplacement d'un widget
void SwDockWidget_Splitter::replaceWidget(QWidget * widget, int pos)
{
	QSize size = widget->size();
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
	if (dock)
		size = dock->getRawSize();

	if (pos == 0)
	{
		_widget1 = widget;
		_widget1_Size = size;
	}
	else
	{
		_widget2 = widget;
		_widget2_Size = size;
	}
	QSplitter::insertWidget(pos, widget);
	updateSizes(_widget1_Size, _widget2_Size);
}

//-----------------------------------------------------------------------------
//Redimensionnement apres ajout d'un widget en fonction de la taille des widgets
void SwDockWidget_Splitter::updateSizes(QSize size1, QSize size2)
{
	if (QSplitter::count() == 2)
	{
		//Mise a jour de l'affichage
		//this->show();

		//Set widgets sizes
		QList<int> sizes;
		sizes << 1 << 1;
		int pos1 = QSplitter::indexOf(_widget1);
		int pos2 = QSplitter::indexOf(_widget2);

		//Mise a jour de la taille des elements
		if (pos1 >= 0 && pos2 >= 0)
		{
			if (QSplitter::orientation() == Qt::Horizontal)
			{
				sizes.replace(pos1, size1.width());
				sizes.replace(pos2, size2.width());
			}
			else
			{
				sizes.replace(pos1, size1.height());
				sizes.replace(pos2, size2.height());
			}
			QSplitter::setSizes(sizes);
		}

		//Mise a jour de la taille du splitter
		if (QSplitter::orientation() == Qt::Horizontal)
		{
			QSize size;
			size.setWidth(size1.width() + size2.width());
			size.setHeight(qMax(size1.height(), size2.height()));
			QSplitter::resize(size);
		}
		else
		{
			QSize size;
			size.setWidth(qMax(size1.width(), size2.width()));
			size.setHeight(size1.height() + size2.height());
			QSplitter::resize(size);
		}
	}
}

//-----------------------------------------------------------------------------
//Si le splitter est deplace, on enregistre la nouvelle taille des elements
void SwDockWidget_Splitter::resizeEvent( QResizeEvent * r )
{
	QSplitter::resizeEvent(r);
	childrenResized();
}

//-----------------------------------------------------------------------------
//Enregistrement de la nouvelle taille des elements
void SwDockWidget_Splitter::childrenResized()
{
	if (_widget1)
		_widget1_Size = _widget1->size();
	if (_widget2)
		_widget2_Size = _widget2->size();
}

//-----------------------------------------------------------------------------
void SwDockWidget_Splitter::moveSplitter(int pos)
{
	QSplitter::moveSplitter(pos, 1);
}