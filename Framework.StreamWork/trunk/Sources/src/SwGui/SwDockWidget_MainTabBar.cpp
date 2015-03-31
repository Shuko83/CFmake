#include "SwDockWidget_MainTabBar.h"

#include <QtGui/QMouseEvent>
#include <QKeyEvent>
#include <QAction>

//-----------------------------------------------------------------------------
SwDockWidget_MainTabBar::SwDockWidget_MainTabBar(QWidget * parent, Qt::DockWidgetArea area)
: QTabBar(parent), _nameEdit(NULL), _name(""), _moving(false), _area(area)
{
	//Menu contextuel

	//Renommage
	QAction * renameAction = new QAction(tr("Rename"), this);
	connect(renameAction, SIGNAL(triggered()), this, SLOT(renameAction()));

	//Transfert d'un onglet dans un autre dock principal
	QIcon icoRight(":/DockWidget/rightArrow");
	icoRight.addPixmap(QPixmap(":/DockWidget/rightArrowDsb"), QIcon::Disabled);
	QAction * moveToRight = new QAction(icoRight, tr("Move to right"), this);
	connect(moveToRight, SIGNAL(triggered()), this, SLOT(moveToRight()));

	QIcon icoLeft(":/DockWidget/leftArrow");
	icoLeft.addPixmap(QPixmap(":/DockWidget/leftArrowDsb"), QIcon::Disabled);
	QAction * moveToLeft = new QAction(icoLeft, tr("Move to left"), this);
	connect(moveToLeft, SIGNAL(triggered()), this, SLOT(moveToLeft()));

	QIcon icoBottom(":/DockWidget/downArrow");
	icoBottom.addPixmap(QPixmap(":/DockWidget/downArrowDsb"), QIcon::Disabled);
	QAction * moveToBottom = new QAction(icoBottom, tr("Move to bottom"), this);
	connect(moveToBottom, SIGNAL(triggered()), this, SLOT(moveToBottom()));

	QIcon icoTop(":/DockWidget/upArrow");
	icoTop.addPixmap(QPixmap(":/DockWidget/upArrowDsb"), QIcon::Disabled);
	QAction * moveToTop = new QAction(icoTop, tr("Move to top"), this);
	connect(moveToTop, SIGNAL(triggered()), this, SLOT(moveToTop()));

	QIcon icoSecond(":/DockWidget/secondScreen");
	icoSecond.addPixmap(QPixmap(":/DockWidget/secondScreenDsb"), QIcon::Disabled);
	QAction * moveToSecondScreen = new QAction(icoSecond, tr("Move to second screen"), this);
	connect(moveToSecondScreen, SIGNAL(triggered()), this, SLOT(moveToSecondScreen()));

	//Fermeture
	QAction * closeAction = new QAction(QIcon(":/DockWidget/close"), tr("Close"), this);
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeTab()));

	//Separateurs
	QAction * sep1 = new QAction(this);
	sep1->setSeparator(true);
	QAction * sep2 = new QAction(this);
	sep2->setSeparator(true);
	QAction * sep3 = new QAction(this);
	sep3->setSeparator(true);

	//Ajouts des actions dans le menu contextuel
	this->addAction(renameAction);
	this->addAction(sep1);
	this->addAction(moveToRight);
	this->addAction(moveToLeft);
	this->addAction(moveToBottom);
	this->addAction(moveToTop);
	this->addAction(sep2);
	this->addAction(moveToSecondScreen);
	this->addAction(sep3);
	this->addAction(closeAction);

	//Le transfert d'un onglet vers son propre cote n'est pas possible
	if (_area == Qt::RightDockWidgetArea)
		moveToRight->setEnabled(false);
	if (_area == Qt::LeftDockWidgetArea)
		moveToLeft->setEnabled(false);
	if (_area == Qt::BottomDockWidgetArea)
		moveToBottom->setEnabled(false);
	if (_area == Qt::TopDockWidgetArea)
		moveToTop->setEnabled(false);
	if (_area == Qt::NoDockWidgetArea)
		moveToSecondScreen->setEnabled(false);

	this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//-----------------------------------------------------------------------------
SwDockWidget_MainTabBar::~SwDockWidget_MainTabBar()
{
	
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::mousePressEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton && tabsClosable())
	{
		//Si demande d'ajout d'onglet
		if (tabRect(count()-1).contains(e->pos()) && this->parent())
		{
			emit insertNewTab();
			return;
		}

		//Enregistrement de la distance entre le curseur et l'origine de l'onglet
		_clickPos = e->pos();
		_moving = false;
	}
	if (e->button() == Qt::RightButton)
	{
		//Activation de l'onglet situe sous le pointeur
		activeTabUnder(e->pos());
	}

	QTabBar::mousePressEvent(e);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (e->button () == Qt::LeftButton && tabsClosable())
	{
		//Si demande d'ajout d'onglet
		if (tabRect(count()-1).contains(e->pos()))
		{
			return;
		}

		renameAction();
		return;
	}

	QTabBar::mouseDoubleClickEvent(e);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::keyPressEvent(QKeyEvent * event)
{
	switch(event->key())
	{
		case Qt::Key_Escape:
			//Si edition en cours, annulation
			cancelNameEdit();
			break;

		/*case Qt::Key_F2:
			renameAction();
			break;*/

		default:
			break;
	}
	QTabBar::keyPressEvent(event);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::paintEvent(QPaintEvent * event)
{
	if (_nameEdit && _nameEdit->isVisible())
	{
		int index = currentIndex();
		//Mise a jour des dimensions du LineEdit
		QRect rect(tabRect(index));
		_nameEdit->resize(rect.size());
		_nameEdit->move(QPoint(rect.x(), rect.y()));
	}
	QTabBar::paintEvent(event);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::renameAction()
{
	//Creation d'une LineEdit pour editer le nom de l'onglet
	int index = currentIndex();
	_name = tabText(index);
	if (_nameEdit == NULL)
	{
		_nameEdit = new QLineEdit(_name, this);
		_nameEdit->setObjectName("mainTabRename");
		_nameEdit->setFrame(false);
	}
	connectNameEdit();
	_nameEdit->setText(_name);
	_nameEdit->setFocus();
	_nameEdit->selectAll();
	//Placement de la LineEdit sur l'onglet
	QRect rect(tabRect(index));
	//Ajustement de la taille et de la position
	_nameEdit->move(QPoint(rect.x(), rect.y()));
	_nameEdit->resize(rect.size());
	_nameEdit->show();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::renameTabTemporarily(QString name)
{
	if (_nameEdit && _nameEdit->isVisible())
	{
		int index = currentIndex();
		//Mise a jour du texte
		setTabText(index, name);
	}
}

//-----------------------------------------------------------------------------
//Validation de la modification du nom de l'onglet en cours d'edition
void SwDockWidget_MainTabBar::renameTab()
{
	if (_nameEdit && _nameEdit->isVisible())
	{
		int index = currentIndex();
		//Un onglet ne peut pas avoir un nom vide
		if (_nameEdit->text().isEmpty())
			setTabText(index, _name);
		else
			setTabText(index, _nameEdit->text());
		disconnectNameEdit();
		_nameEdit->hide();
	}
}

//-----------------------------------------------------------------------------
//Annulation de la modification du nom de l'onglet en cours d'edition
void SwDockWidget_MainTabBar::cancelNameEdit()
{
	if (_nameEdit && _nameEdit->isVisible())
	{
		int index = currentIndex();
		setTabText(index, _name);
		disconnectNameEdit();
		_nameEdit->hide();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::connectNameEdit()
{
	if (_nameEdit)
	{
		connect(_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(renameTabTemporarily(QString)));
		connect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(renameTab()));
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::disconnectNameEdit()
{
	if (_nameEdit)
	{
		disconnect(_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(renameTabTemporarily(QString)));
		disconnect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(renameTab()));
	}
}

//-----------------------------------------------------------------------------
QSize SwDockWidget_MainTabBar::tabSizeHint( int index ) const
{
	//Taille de l'onglet "+" fixe
	if (tabsClosable() && index == count()-1 && count() > 1)
	{
		return QSize(QTabBar::tabSizeHint(index).height(), QTabBar::tabSizeHint(index).height());
	}
	else
		return QTabBar::tabSizeHint(index);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::closeTab()
{
	emit closeTabRequested(currentIndex());
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::moveToLeft()
{
	emit moveTabRequested(currentIndex(), Qt::LeftDockWidgetArea);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::moveToRight()
{
	emit moveTabRequested(currentIndex(), Qt::RightDockWidgetArea);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::moveToTop()
{
	emit moveTabRequested(currentIndex(), Qt::TopDockWidgetArea);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::moveToBottom()
{
	emit moveTabRequested(currentIndex(), Qt::BottomDockWidgetArea);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::moveToSecondScreen()
{
	emit moveTabRequested(currentIndex(), Qt::NoDockWidgetArea);
}

//-----------------------------------------------------------------------------
int SwDockWidget_MainTabBar::activeTabUnder(QPoint pos, bool toGlobal)
{
	//Activation de l'onglet situe sous le pointeur
	int numTab = -1;
	int i = 0;
	QPoint posG = toGlobal?mapToGlobal(pos):pos;
	while ((numTab==-1) && i < count())
	{
		QRect rect = tabRect(i);
		QPoint tabPos = mapToGlobal(QPoint(rect.x(), rect.y()));
		if ((posG.x() >= tabPos.x()) && (posG.x() < (tabPos.x() + rect.width())))
		{
			//Si l'onglet survole est l'onglet d'ajout, on ne l'active pas
			if (i < count()-1)
				setCurrentIndex(i);
			numTab = i;
		}
		i++;
	}

	return numTab;
}