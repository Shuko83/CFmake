
#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>
#include <QTimer>

#include <QDebug>
#include <QMessageBox>

#include "CustomListWidget/SearchView.h"
#include "CustomListWidget/BaseCustomListWidget.h"
#include "QsBaseComboBox.h"
#include "CustomDoubleValidator.h"
#include "ui_QsBaseComboBox.h"


QsBaseComboBox::QsBaseComboBox(QWidget *parent)	: QWidget(parent)
{
	_ui = new Ui::BaseComboClass();
	_ui->setupUi(this);
	

 	_searchView = new SearchView(this);
 	_customListWidget = nullptr;

	connect(_ui->pushButton, &QPushButton::toggled, this, &QsBaseComboBox::displayFullComboList);
	connect(_ui->lineEdit, &QLineEdit::textEdited, this, &QsBaseComboBox::checkEnteredValue);
	connect(_searchView, &SearchView::selectionItemChanged, this, &QsBaseComboBox::onItemSelected);
	connect(_searchView, &SearchView::cancelSelection, this, &QsBaseComboBox::onCancelSelection);
	connect(_searchView, &SearchView::cancelCloseWindow, this, &QsBaseComboBox::onCancelCloseWindow);
	connect(_searchView, &SearchView::closeWindow, this, &QsBaseComboBox::onCloseWindow);
	connect(_searchView, &SearchView::focusLineEdit, this, &QsBaseComboBox::onFocusLineEdit);
	connect(_searchView, &SearchView::itemHovered, this, &QsBaseComboBox::onItemHovered);

	if (parent)
		parent->installEventFilter(this);
	_ui->lineEdit->installEventFilter(this);
	_ui->pushButton->installEventFilter(this);
	
	
	_lineEditValidator = 0;
	_hideWindowConfirmed = false;
	_placeHolderText = "Search or press Enter to valid";
	_lastSelectedValue = "";
}

QsBaseComboBox::~QsBaseComboBox()
{
	_ui->lineEdit->setValidator(0);
 	if (_lineEditValidator != 0)
 		delete _lineEditValidator;

 	delete _searchView;
}



//-------------------------------------------------------------------------
void QsBaseComboBox::setComboValues(QStringList val)
{
	_itemList = val;

	if (_itemList.size() > 0)
	{
		// Première valeur de référence
		_lastSelectedValue = _itemList.at(0);

		// Construction de la liste d'items		
		_searchView->getCustomListWidget()->setItemList(_itemList);
		_searchView->displayRefValue(_lastSelectedValue);
		_ui->lineEdit->setText(_lastSelectedValue);

		// Set de la taille mini de la combo / LineEdit en fonction de l'item de plus long
		int sizeMax = _searchView->getCustomListWidget()->getSizeMax();

		_ui->lineEdit->setMinimumWidth(sizeMax);

		emit comboValuesChanged();
	}
}


//-------------------------------------------------------------------------
QStringList QsBaseComboBox::comboValues()
{
	return _itemList;
}

//-------------------------------------------------------------------------
QString QsBaseComboBox::currentValue()
{
	return _lastSelectedValue;
}

//-------------------------------------------------------------------------
void QsBaseComboBox::forceCurrentValue(QString value)
{
	_lastSelectedValue = value;
	
	// Affichage de l'item sélectionné dans la LineEdit et dans la zone de preview
	_searchView->displayRefValue(_lastSelectedValue);
	_ui->lineEdit->setText(_lastSelectedValue);
}

//-------------------------------------------------------------------------
bool QsBaseComboBox::setCurrentValue(QString value, int index )
{
	bool ret = false;
	// on garde en mémoire la valeur courante de la combo pour voir si elle change réellement
	QString OldTmpValue = _lastSelectedValue;			
	
	// Si la recherche dans la liste des items est successful
	QStringList searchResult = _searchView->getCustomListWidget()->search(value);
	if (searchResult.size() >= 1 && index == -1)
	{
		_lastSelectedValue = searchResult.first();
		ret = true;
	}
	// Si index renseigné, on discrimine avec l'index pour être sûrs de la valeur cherchée
	else if (searchResult.size() >= 1)
	{
		for (auto result : searchResult)
		{
			int index1 = result.lastIndexOf("(");
			int index2 = result.lastIndexOf(")");
			int stanagEnumSize = (index2 - index1 - 1);
			QString stanagEnum = result.mid(index1 + 1, stanagEnumSize);

			if (stanagEnum.toInt() == index)
			{
				_lastSelectedValue = result;
				ret = true;
				break;
			}
		}
	}
	
	// Affichage de l'item sélectionné dans la LineEdit et dans la zone de preview
	_searchView->displayRefValue(_lastSelectedValue);
	_ui->lineEdit->setText(_lastSelectedValue);

	// Emit le signal selectionItemChanged(); pour notifier du changement de valeur de la combo que lorsqu'un changement de valeur a réellement eu lieu
	if (OldTmpValue != _lastSelectedValue)
		emit selectionItemChanged();

	return ret;
}

//-------------------------------------------------------------------------
//								SLOTS
//-------------------------------------------------------------------------
void QsBaseComboBox::displayFullComboList(bool buttonValue)
{
	if (!buttonValue)
	{
		onCancelSelection();
	}
	else if (buttonValue && _itemList.size() > 0)
	{
		// Liste entière des items possibles
		displayAllItemList();

		// Placement / rezise / Affichage
		placeAndShowWidget(120);
		
		// Set focus sur LineEdit et suppression texte
		_ui->lineEdit->setText("");
		_ui->lineEdit->setPlaceholderText(_placeHolderText);
		onFocusLineEdit();

		// On installe l'event filter sur toute la qApp pour catcher les clics 
		// partout ailleurs dans la fenêtre pour fermer la combo
		qApp->installEventFilter(this);
	}
}

//-------------------------------------------------------------------------
void QsBaseComboBox::onItemSelected(QString item)
{
	// On remove l'event filter sur toute la qApp pour ne pas catcher tous les clics 
	// du monde alors que la combo n'est pas displayed
	qApp->removeEventFilter(this);

	_searchView->hide();
	checkPushButton(false);		// Pour etre sûr

	// On garde en mémoire le dernier item sélectionné
	if (item != "")
	{
		_lastSelectedValue = item;
		emit selectionItemChanged();		// On notifie d'un changement réel de valeur de la combo
	}

	// Affichage de l'item sélectionné dans la LineEdit
	_ui->lineEdit->setText(_lastSelectedValue);
	_ui->lineEdit->selectAll();
}

//-------------------------------------------------------------------------
void QsBaseComboBox::onItemHovered(QString item)
{
	emit highlighted(item);
}

//-------------------------------------------------------------------------
void QsBaseComboBox::onCancelSelection()
{
	onItemSelected("");
}

//-------------------------------------------------------------------------
void QsBaseComboBox::processHideWindow()
{
	if (_hideWindowConfirmed)
	{
		onCancelSelection();

		// Déselection de l'item dans la LineEdit
		_ui->lineEdit->deselect();
	}
}

//-------------------------------------------------------------------------
void QsBaseComboBox::onCloseWindow()
{
 	_hideWindowConfirmed = true;
	QTimer::singleShot(0, this, SLOT(processHideWindow()));
}

//-------------------------------------------------------------------------
void QsBaseComboBox::onCancelCloseWindow()
{
	_hideWindowConfirmed = false;
}


//-------------------------------------------------------------------------
//						PRIVATE METHODES
//-------------------------------------------------------------------------
void QsBaseComboBox::placeAndShowWidget(int heightSize)
{
	// Size en hauteur de la zone de liste déroulante
	_searchView->setFixedHeight(heightSize);
	QEvent tmp(QEvent::Resize);
	eventFilter(parent(), &tmp); // Fait appel au Move

	// Set sur l'item précédement sélectionné
	_searchView->getCustomListWidget()->setCurrentSelection(_lastSelectedValue);

	_searchView->show();
}


//-------------------------------------------------------------------------
void QsBaseComboBox::onFocusLineEdit()
{
	_ui->lineEdit->setFocus();
	activateWindow();
}


//-------------------------------------------------------------------------
void QsBaseComboBox::focusPushButton()
{
	_ui->pushButton->setFocus();
	checkPushButton(false);
	
}

//-------------------------------------------------------------------------
void QsBaseComboBox::checkPushButton(bool checkValue)
{
	_ui->pushButton->blockSignals(true);
	_ui->pushButton->setChecked(checkValue);
	_ui->pushButton->blockSignals(false);
}

//-------------------------------------------------------------------------
//						PUBLIC METHODES
//-------------------------------------------------------------------------
void QsBaseComboBox::displayAllItemList()
{
	if (_itemList.size() > 0)
	{
		_searchView->getCustomListWidget()->clearAll();

		// Mise à jour du modèle interne de la QListWidget avec les valeurs de la itemList
		_searchView->getCustomListWidget()->buildItemList();

		_searchView->displayRefValue(_lastSelectedValue);
		_ui->lineEdit->setText(_lastSelectedValue);
	}
}




//------------------------------------------------------------------------- 
void QsBaseComboBox::closeEvent(QCloseEvent* /* e */)
{
	delete _searchView;
}


//-------------------------------------------------------------------------
void QsBaseComboBox::keyPressEvent(QKeyEvent *event)
{
	// C'est le widget parent qui a le focus, il faut rediriger le keyPressEvent vers le fils
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || 
		event->key() == Qt::Key_Down || event->key() == Qt::Key_Up || event->key() == Qt::Key_Escape)
	{
		if (_searchView->isVisible())
			_searchView->keyPressEvent(event);
	}
	QWidget::keyPressEvent(event);
}


 //-------------------------------------------------------------------------
 bool QsBaseComboBox::eventFilter(QObject * object, QEvent * event)
 {
	 if (object == parent())
	 {
		 // Déplacement des widgets
		 if (event->type() == QEvent::Move)
		 {
			 QPoint globalCoord = mapToGlobal(QPoint(0,0));
			 globalCoord.setY(globalCoord.y() + _ui->lineEdit->pos().y() + _ui->lineEdit->height());
			 _searchView->move(globalCoord);
		 }
		 // Resize des widgets
		 if (event->type() == QEvent::Resize)
		 {
			_searchView->setFixedWidth(_ui->lineEdit->width() + _ui->pushButton->width());
			_searchView->getCustomListWidget()->setFixedWidth(_ui->lineEdit->width() + _ui->pushButton->width());

			 // Le resize implique un move des coordonnées
			QEvent tmp(QEvent::Move);
			eventFilter(object, &tmp);
		 }
	 }
 	 else if (object == _ui->lineEdit)
 	 {
 		 // Sélection totale si la recherche n'est pas en cours
		 if ((event->type() == QEvent::MouseButtonPress && !_searchView->isVisible()) )
 		 {
			 _ui->lineEdit->selectAll();
			 _ui->pushButton->toggle();
			 return true;
 		 }
		 // Efface tout si la lineEdit est sélectionnée
		 if (event->type() == QEvent::KeyPress && _ui->lineEdit->hasSelectedText())
		 {
			 QKeyEvent *ke = static_cast<QKeyEvent *>(event);
			 if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
			 {
				 _ui->pushButton->toggle();
				 return true;
			 }
		 }
		 if (event->type() == QEvent::Enter)
		 {
			 emit mouseEnter();
		 }
		 if (event->type() == QEvent::Leave)
		 {
			 emit mouseLeave();
		 }
		 // Gestion des focus pour hide de la fenêtre
		 return manageFocusListview(event);
 	 }
	 else if (object == _ui->pushButton)
	 {
		 // Gestion des focus pour hide de la fenêtre
		 return manageFocusListview(event);
	 }
	 else if (object == _searchView->getCustomListWidget())
	 {
		// Gestion des focus pour hide de la fenêtre
		 return manageFocusListview(event);
	 }

	 // Gestion des clics souris pour fermeture de la combo si clic en dehors
	 if (event->type() == QEvent::MouseButtonPress)
	 {
		 bool test1 = _ui->lineEdit->underMouse();
		 bool test2 = _searchView->underMouse();
		 bool test3 = false;
		 if (_searchView->getCustomListWidget())
			 test3 = _searchView->getCustomListWidget()->underMouse();

		 if (!test1 && !test2 && !test3)
		 {
			 onCloseWindow();
		 }
	 }

	return false;
 }


 //-------------------------------------------------------------------------
 bool QsBaseComboBox::manageFocusListview(QEvent * event)
 {
	 bool ret = false;
	 if (event->type() == QEvent::FocusIn)
	 {
		 // On empêche/intercepte le close en cours de la liste déroulée
		 onCancelCloseWindow();
		 ret = true;
	 }
	 if (event->type() == QEvent::FocusOut)
	 {
		 // On demande le close de la liste déroulée
		 onCloseWindow();
		 ret = true;
	 }
	 return ret;
 }

 //-------------------------------------------------------------------------
 void QsBaseComboBox::showLastSelection()
 {
	 _ui->lineEdit->setText("");
	 _ui->lineEdit->setPlaceholderText(_placeHolderText);
	 checkPushButton(false);
	
	 // Affichage de la dernière valeur choisie uniquement
	 _searchView->displayRefValue(_lastSelectedValue);
	 _searchView->getCustomListWidget()->clearAll();

	 // Placement / rezise / Affichage
	 placeAndShowWidget(25);
	
	 onFocusLineEdit();
 }

 //-------------------------------------------------------------------------
 void QsBaseComboBox::removeItems(QStringList items)
 {
	 QStringListIterator it(items);

	 while (it.hasNext())
	 {
		 QString item = it.next();

		 if (_itemList.contains(item))
			 _itemList.removeOne(item);
	 }

	 setComboValues(_itemList);
 }
