#include "SearchView.h"
#include "ui_SearchView.h"

//-------------------------------------------------------------------------
SearchView::SearchView(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint);
	setMouseTracking(true);

	_ui = new Ui::SearchView();
	_ui->setupUi(this);
	_ui->preValidationZone2->setFrame(true);
	_ui->preValidationZone2->addAction(QIcon(":apply"), QLineEdit::LeadingPosition);
	_ui->preValidationZone2->installEventFilter(this);

	_customListWidget = 0;
}


//-------------------------------------------------------------------------
SearchView::~SearchView()
{
	if (_customListWidget)
		delete _customListWidget;
	_customListWidget = 0;
}

//-------------------------------------------------------------------------
BaseCustomListWidget * SearchView::getCustomListWidget()
{
	return _customListWidget;
}


//-------------------------------------------------------------------------
void SearchView::setCustomListWidget(BaseCustomListWidget *customListWidget)
{
	_customListWidget = customListWidget;

	// Pour style CSS
	_customListWidget->setObjectName("name_customListWidget");

	if (_customListWidget)
	{
		_ui->verticalLayout->addWidget(_customListWidget);
		connect(_customListWidget, &BaseCustomListWidget::itemDoubleClicked, this, &SearchView::onItemDoubleClicked);
		connect(_customListWidget, &BaseCustomListWidget::currentItemChanged, this, &SearchView::onItemChanged);
		connect(_customListWidget, &BaseCustomListWidget::itemEntered, this, &SearchView::onItemHovered);
	}
}

//-------------------------------------------------------------------------
void SearchView::displayRefValue(QString refValue)
{
	_ui->preValidationZone2->setText(refValue);
}

//-------------------------------------------------------------------------
void SearchView::keyPressEvent(QKeyEvent *event)
{
	// lorsque "Enter" est tapé on valide la séléction comme un double clic souris
	if (event->key() == Qt::Key_Enter
		|| event->key() == Qt::Key_Return)
	{
		validateSelection(_ui->preValidationZone2->text());
	}
	if (event->key() == Qt::Key_Down)
	{
		if (_customListWidget->currentRow() < _customListWidget->count() - 1)
		{
			_customListWidget->setCurrentRow(_customListWidget->currentRow() + 1);
			if (getCurrentSelection().contains("---") && _customListWidget->currentRow() < _customListWidget->count() - 1)
			{ 
				_customListWidget->setCurrentRow(_customListWidget->currentRow() + 1);
			}
		}
	}
	if (event->key() == Qt::Key_Up)
	{
		if (_customListWidget->currentRow() > 0)
		{
			_customListWidget->setCurrentRow(_customListWidget->currentRow() - 1);
			if (getCurrentSelection().contains("---") && _customListWidget->currentRow() > 0)
			{
				_customListWidget->setCurrentRow(_customListWidget->currentRow() - 1);
			}
		}
	}
	if (event->key() == Qt::Key_Escape)
	{
		emit cancelSelection();
	}
	QWidget::keyPressEvent(event);
}



//-------------------------------------------------------------------------
bool SearchView::eventFilter(QObject * object, QEvent * event)
{
	if (object == _ui->preValidationZone2)
	{
		// Double clic sur la zone de validation = comme double click sur un des items
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			validateSelection(_ui->preValidationZone2->text());
			return true;
		}
		// Gestion des focus pour hide de la fenêtre
		if (event->type() == QEvent::FocusIn)
		{
			emit cancelCloseWindow();
		}
		if (event->type() == QEvent::FocusOut)
		{
			emit closeWindow();
		}
	}
	return false;
}

//-------------------------------------------------------------------------
//							SLOTS
//-------------------------------------------------------------------------
void SearchView::onItemDoubleClicked()
{
	validateSelection(getCurrentSelection());
}


//-------------------------------------------------------------------------
void SearchView::onItemChanged()
{
	_ui->preValidationZone2->setText(getCurrentSelection());

	// Comportement similaire au hover souris pour preview éventuel de la valeur 
	emit itemHovered(getCurrentSelection());

	// Focus sur la LineEdit à nouveau
	emit focusLineEdit();
}

//-------------------------------------------------------------------------
void SearchView::onItemHovered(QListWidgetItem *item)
{
	if (item)
	{
		QString textHovered = item->text();
		emit itemHovered(textHovered);
	}
}

//-------------------------------------------------------------------------
//							PRIVATE
//-------------------------------------------------------------------------
QString SearchView::getCurrentSelection()
{
	QString currentSelection = _customListWidget->getCurrentSelection();
	currentSelection.remove("<u>");
	currentSelection.remove("</u>");
	return currentSelection;
}

//-------------------------------------------------------------------------
void SearchView::validateSelection(QString selection)
{
	emit selectionItemChanged(selection);
}