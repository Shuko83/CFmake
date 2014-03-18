#include "SwDockWidget_MainTabWidget.h"
#include "SwDockWidget_MainTabBar.h"

#include <QMessageBox>

//-----------------------------------------------------------------------------
SwDockWidget_MainTabWidget::SwDockWidget_MainTabWidget(QWidget * parent, bool withAddButton)
 : SwDockWidget_TabWidget(parent), _lock(false), _addWidget(NULL)
{
	SwDockWidget_MainTabBar * tabBar = new SwDockWidget_MainTabBar(this);
	setTabBar(tabBar);
	//setMovable(true);
	setTabsClosable(true);
	
	//Creation d'un onglet vide pour l'ajout d'un nouvel onglet
	if (withAddButton)
	{
		_addWidget = new QWidget(this);
		insertTab(0, _addWidget, QIcon(":/DockWidget/images/DockWidget/add.png"), "");
		tabBar->setTabButton(0, QTabBar::RightSide, 0);
	}

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateCurrent(int)));
	
	connect(tabBar, SIGNAL(moveTabRequested(QPoint)), this, SIGNAL(moveTabRequested(QPoint)));
	connect(tabBar, SIGNAL(freeTabRequested(int, QPoint)), this, SIGNAL(freeTabRequested(int, QPoint)));
	connect(tabBar, SIGNAL(stopMovingTabRequested()), this, SIGNAL(stopMovingTabRequested()));
	connect(tabBar, SIGNAL(insertNewTab()), this, SLOT(insertNewTab()));

}

//-----------------------------------------------------------------------------
SwDockWidget_MainTabWidget::~SwDockWidget_MainTabWidget()
{
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::closeTab(int index)
{
	QWidget * widget = this->widget(index);

	if (!widget)
		return;

	//Demande de confirmation avant fermeture, si l'onglet n'est pas valide
	bool empty = false;
	
	QLayout * layout = widget->layout();
	if (layout && (layout->count() == 0))
	{
		empty = true; 
	}
	else if (!layout)
		empty = true;

	if (!empty)
	{
		QMessageBox msgBox;
		msgBox.setText("Etes-vous sûr de vouloir fermer l'onglet " + tabText(index) + " ?");
		msgBox.setStandardButtons(QMessageBox::Yes |QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No)
			return;
	}

	//Fermeture du widget
	widget->close();
	//Fermeture du widget contenu dans l'onglet pour propager l'evenement
	if (widget->layout())
	{
		QLayoutItem * item = widget->layout()->itemAt(0);
		if (item && item->widget())
			item->widget()->close();
	}

	//Suppression de l'onglet (fait automatiquement lors de la suppression du widget)
	if (empty)
		this->removeTab(index);

	//Mise a jour de l'onglet actif
	setCurrentIndex(index);
}

//-----------------------------------------------------------------------------
int SwDockWidget_MainTabWidget::count()
{
	if (_addWidget)
		return SwDockWidget_TabWidget::count() - 1;
	else
		return SwDockWidget_TabWidget::count();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::updateCurrent(int index)
{
	if (index >= count())
	{
		SwDockWidget_TabWidget::setCurrentIndex(count()-1);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::insertNewTab ()
{
	int index = insertTab(count(), new QWidget(this), "Tab " + QString::number(count() + 1));
	setCurrentIndex(index);
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_MainTabWidget::getTabBar()
{
	return this->tabBar();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::lock()
{
	if (!_lock)
	{
		_lock = true;
		setTabsClosable(false);
		removeTab(count());
		_addWidget = 0;
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::releaseLock()
{
	if (_lock)
	{
		_lock = false;
		setTabsClosable(true);
		if (!_addWidget)
		{
			_addWidget = new QWidget(this);
			int index = QTabWidget::addTab(_addWidget, QIcon(":/DockWidget/images/DockWidget/add.png"), "");
			tabBar()->setTabButton(index, QTabBar::RightSide, 0);
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::clear()
{
	//Reinitialisation
	QTabWidget::clear();

	//Creation d'un onglet vide pour l'ajout d'un nouvel onglet
	_addWidget = new QWidget(this);
	insertTab(0, _addWidget, QIcon(":/DockWidget/images/DockWidget/add.png"), "");
	tabBar()->setTabButton(0, QTabBar::RightSide, 0);
}