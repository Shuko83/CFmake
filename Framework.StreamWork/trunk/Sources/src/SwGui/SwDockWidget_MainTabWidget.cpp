#include "SwDockWidget_MainTabWidget.h"
#include "SwDockWidget_MainTabBar.h"

#include <QMessageBox>

//-----------------------------------------------------------------------------
SwDockWidget_MainTabWidget::SwDockWidget_MainTabWidget(QWidget * parent, bool withAddButton, Qt::DockWidgetArea area)
 : SwDockWidget_TabWidget(parent), _lock(false), _addWidget(NULL), _area(area)
{
	SwDockWidget_MainTabBar * tabBar = new SwDockWidget_MainTabBar(this, area);
	setTabBar(tabBar);
	//setMovable(true); //Si option active, il faut gerer le cas de l'onglet "+" qui doit rester en derniere position
	setTabsClosable(true);
	
	//Creation d'un onglet vide pour l'ajout d'un nouvel onglet
	if (withAddButton)
	{
		createAddTabWidget();
	}

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateCurrent(int)));

	connect(tabBar, SIGNAL(moveTabRequested(int, Qt::DockWidgetArea)), this, SIGNAL(moveTabRequested(int, Qt::DockWidgetArea)));
	connect(tabBar, SIGNAL(insertNewTab()), this, SLOT(insertNewTab()));
	connect(tabBar, SIGNAL(closeTabRequested(int)), this, SLOT(closeTab(int)));

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

	//Demande de confirmation avant fermeture, si l'onglet n'est pas vide
	bool empty = false;
	
	QLayout * layout = widget->layout();
	//Si l'onglet contient un layout
	if (layout)
	{
		//Si le layout est vide, l'onglet est vide
		if (layout->count() == 0)
			empty = true; 
		else
		{
			//Si le layout contient un widget, on verifie s'il s'agit du widget par defaut d'un onglet vide
			QLayoutItem * item = layout->itemAt(0);
			if (item && item->widget() && !item->widget()->objectName().compare("emptyWidget"))
				empty = true;
		}
	}
	//Si l'onglet ne contient pas de layout, il est vide
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
	if (layout)
	{
		QLayoutItem * item = layout->itemAt(0);
		if (item && item->widget())
		{
			QString name = item->widget()->objectName();
			item->widget()->close();
		}
	}

	//Suppression de l'onglet
	//if (empty)
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
	QWidget * temp = new QWidget(this);
	temp->setObjectName("emptyWidget");
	int index = insertTab(count(), temp, "Tab " + QString::number(count() + 1));
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
			createAddTabWidget();
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::clear()
{
	//Reinitialisation
	QTabWidget::clear();

	//Creation d'un onglet vide pour l'ajout d'un nouvel onglet
	createAddTabWidget();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabWidget::createAddTabWidget()
{
	_addWidget = new QWidget(this);
	_addWidget->setObjectName("AddTabWidget");
	int index = QTabWidget::addTab(_addWidget, QIcon(":/DockWidget/images/DockWidget/add.png"), "");
	tabBar()->setTabButton(index, QTabBar::RightSide, 0);
}