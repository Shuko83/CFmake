/**
@file ComponentTableWidget.cpp
@date 02/2014
@brief TableWidget view pour la vue des composants
@author  EPO
@version 1.0
*/

// Includes globaux
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QHeaderView>
#include <QMimeData>
#include <QMessageBox>

// Includes locaux
#include "ComponentTools/ComponentTableWidget.h"

using namespace StreamWork::SwGui;

// ------------------------------------------------------------------------
ComponentTableWidget::ComponentTableWidget(QWidget * parent)
	: QTableWidget(parent)
{
	// Construction du QTableWidget
	if ( this->columnCount() < 1 )
		this->setColumnCount(1);

	QTableWidgetItem *_qtableheader1 = new QTableWidgetItem("Name");
	this->setHorizontalHeaderItem(0, _qtableheader1);

	this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->horizontalHeader()->setDefaultSectionSize(150);
	this->horizontalHeader()->setStretchLastSection(true);
	this->horizontalHeader()->setHighlightSections(true);

	this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->verticalHeader()->setVisible(false);
	this->verticalHeader()->setHighlightSections(true);

	this->setSelectionBehavior(SelectRows);
}

//-------------------------------------------------------------------------
ComponentTableWidget::~ComponentTableWidget()
{
}

// ------------------------------------------------------------------------
void ComponentTableWidget::dragEnterEvent(QDragEnterEvent * event)
{
	if ( event->mimeData()->hasFormat("application/component") )
	{
		event->accept();
	}
	else
		event->ignore();
}

// ------------------------------------------------------------------------
void ComponentTableWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
	event->accept();
}

// ------------------------------------------------------------------------
void ComponentTableWidget::dragMoveEvent(QDragMoveEvent * event)
{
	if ( event->mimeData()->hasFormat("application/component") )
	{
		event->accept();
	}
	else
		event->ignore();
}

// ------------------------------------------------------------------------
void ComponentTableWidget::keyPressEvent(QKeyEvent * event)
{
	this->setSortingEnabled(false);

	if ( event->matches(QKeySequence::Delete) )
	{
		int rowToDelete = this->currentRow();

		// Suppression de la property dans la liste
		if ( rowToDelete <= this->rowCount() && rowToDelete >= 0 &&
			removeComponentToList(this->item(rowToDelete, 0)->text()) )
		{
			// Suppression de la ligne dans la QTableWidget
			this->removeRow(rowToDelete);
		}
		event->accept();
	}
	QTableWidget::keyPressEvent(event);
	this->setSortingEnabled(true);
}

// ------------------------------------------------------------------------
void ComponentTableWidget::addComponentInQTable(QString compo)
{
	this->setSortingEnabled(false);

	// Insertion d'une nouvelle ligne à la fin de la liste
	int rowNumber = this->rowCount();
	this->insertRow(rowNumber);

	this->setItem(rowNumber, 0, new QTableWidgetItem(compo));
	this->item(rowNumber, 0)->setFlags(this->item(rowNumber, 0)->flags() ^ Qt::ItemIsEditable);

	this->setSortingEnabled(true);
}

//-----------------------------------------------------------------------------------------
bool ComponentTableWidget::dropMimeData(int /*row*/, int /*column*/, const QMimeData *data, Qt::DropAction /*action*/)
{
	this->setSortingEnabled(false);
	if ( data->hasFormat("application/component") )
	{
		//	Déserialisation de la classe 
		QString dataCompo = data->data("application/component").data();

		if ( !dataCompo.isEmpty() )
		{
			// Ajout de la property dans la QList
			if ( addComponentToList(dataCompo) )
			{
				// insertion d'une nouvelle ligne dans la QTableWidget
				addComponentInQTable(dataCompo);
			}
		}
	}
	this->setSortingEnabled(true);
	return false;
}

//-------------------------------------------------------------------------
void ComponentTableWidget::setComponentsSaved(QList<QString> listCompo)
{
	_components = listCompo;

	if ( _components.size() != 0 )
	{
		for ( int nbCompo = 0; nbCompo < _components.size(); nbCompo++ )
			addComponentInQTable(_components.at(nbCompo));
	}

	this->setSortingEnabled(true);
	this->sortByColumn(0, Qt::AscendingOrder);
}

//-------------------------------------------------------------------------
bool ComponentTableWidget::addComponentToList(QString compo)
{
	this->setSortingEnabled(false);

	bool ret = false;

	if ( _components.indexOf(compo) != -1 )
	{
		QMessageBox::warning(0,
							 "Cannot add component ",
							 QString("The component \"%1\" can't be added because it is already in the list").arg(compo));
	}
	else
	{
		_components.append(compo);
		ret = true;
	}

	this->setSortingEnabled(true);
	return ret;
}

//-------------------------------------------------------------------------
bool ComponentTableWidget::removeComponentToList(QString compo)
{
	this->setSortingEnabled(false);

	bool ret = false;
	int indexCompo = 0;

	if ( (indexCompo = _components.indexOf(compo)) == -1 )
	{
		QMessageBox::warning(0,"Cannot delete component ",QString("The component \"%1\" can't be deleted because its not find").arg(compo));
	}
	else
	{
		_components.removeAt(indexCompo);
		ret = true;
	}

	this->setSortingEnabled(true);
	return ret;
}
