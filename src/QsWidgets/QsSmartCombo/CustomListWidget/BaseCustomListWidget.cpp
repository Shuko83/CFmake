#include "BaseCustomListWidget.h"
#include "CustomDelegate.h"

#include <QMessageBox>

#include <QDebug>
//-------------------------------------------------------------------------
BaseCustomListWidget::BaseCustomListWidget(QWidget *parent) : QListWidget(parent)
{
	setFrameStyle(QFrame::StyledPanel);
	CustomDelegate *customdelegate = new CustomDelegate();
	setItemDelegate(customdelegate);
}


//-------------------------------------------------------------------------
BaseCustomListWidget::~BaseCustomListWidget()
{
}


//-------------------------------------------------------------------------
void BaseCustomListWidget::setItemList(QStringList val)
{
	_itemRawList = val;
	_itemConvertedList.clear();
	// On converti les QString plain en HTML pour la gestion des caractères spéciaux < > & "
	for each (auto rawItem in val)
	{
		QString formatedString = rawItem.toHtmlEscaped();
		_itemConvertedList.append(formatedString);
	}
	clearAll();
	buildItemList();
}


//-------------------------------------------------------------------------
void BaseCustomListWidget::clearAll()
{
	// Blocage des signaux car le clear déclenche le signal "onItemChanged" sinon...
	selectionModel()->blockSignals(true);
	_itemConvertedSearchList.clear();
	clear();
	selectionModel()->blockSignals(false);
}


//-------------------------------------------------------------------------
QString BaseCustomListWidget::getCurrentSelection()
{
	QString rawCurrentSelection = "";

	// Si pas de séléction on se re-positionne sur la valeur courante
	if (!currentItem())
	{
		blockSignals(true);
		setCurrentIndex(model()->index(0, 0));	
		blockSignals(false);
	}

	QListWidgetItem *it = currentItem();
	if (it)
	{
		QString current = it->text();
		rawCurrentSelection = it->data(Qt::UserRole).toString();
	}
	return rawCurrentSelection;
}

//-------------------------------------------------------------------------
void BaseCustomListWidget::setCurrentSelection(QString text)
{
	QString formatedText = text.toHtmlEscaped();
	QList<QListWidgetItem *> items = findItems(formatedText, Qt::MatchCaseSensitive);
	if (items.size() > 0)
	{
		QListWidgetItem* item = items.first();
		if (item)
		{
			blockSignals(true);			// Bloquer le signal sinon une notif que l'on ne veut pas est déclenchée 
			setCurrentItem(item);
			scrollToItem(item, QAbstractItemView::PositionAtTop);
			blockSignals(false);
		}
	}
}

//-------------------------------------------------------------------------
int BaseCustomListWidget::getSizeMax()
{
	return _sizeMax;
}
