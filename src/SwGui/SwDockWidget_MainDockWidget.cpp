#include "SwDockWidget_MainDockWidget.h"
#include "SwDockWidget_MainTabBar.h"

//-----------------------------------------------------------------------------
SwDockWidget_MainDockWidget::SwDockWidget_MainDockWidget(QWidget * parent, QString name, bool withAddButton)
 : SwDockWidget_DockWidget(parent)
{
	this->setObjectName(name);

	//Apparence
	setCanBeMoved(false);
	hideFrame();

	//Creation d'onglets
	_tab = new SwDockWidget_MainTabWidget(this, withAddButton);
	setWidget(_tab);
	connect(_tab, SIGNAL(moveTabRequested(QPoint)), this, SIGNAL(moveTabRequested(QPoint)));
	connect(_tab, SIGNAL(freeTabRequested(int, QPoint)), this, SIGNAL(freeTabRequested(int, QPoint)));
	connect(_tab, SIGNAL(stopMovingTabRequested()), this, SIGNAL(stopMovingTabRequested()));
	connect(_tab, SIGNAL(currentChanged(int)), this, SLOT(updateContents()));

	//Suppression du spacer present par defaut dans un DockWidget
	ui.contentLayout->removeItem(ui.verticalSpacer);
}

//-----------------------------------------------------------------------------
SwDockWidget_MainDockWidget::~SwDockWidget_MainDockWidget()
{

}

//-----------------------------------------------------------------------------
bool SwDockWidget_MainDockWidget::event( QEvent * event )
{
	switch(event->type())
	{
		//Redimensionnement
		case QEvent::Resize:
			if (isVisible())
				setFreeSize(this->size());
			break;
	}

	return SwDockWidget_DockWidget::event(event);
}

//-----------------------------------------------------------------------------
//Enregistrement du contenu du DockWidget
void SwDockWidget_MainDockWidget::addWidget(QWidget * widget, QString name, QSize * size)
{
	if (widget)
	{
		int index = _tab->count();
		QSize widgetSize = widget->size();

		//Nom affiche
		if (name.isEmpty())
			name = "Tab " + QString::number(index + 1);

		//Ajout du widget dans un nouveal onglet
		QWidget * temp = new QWidget(this);
		temp->setObjectName("tab_" + QString::number(index));
		QGridLayout * lay = new QGridLayout(temp);
		lay->setObjectName("layout_" + QString::number(index));
		lay->addWidget(widget);
		lay->setMargin(0);

		_tab->insertTab(index, temp, name);
		_tab->setCurrentIndex(index);
		
		//Affichage si necessaire
		if (QWidget::isVisible() == false)
			show();

		//Taille du dock si c'est le premier widget ajoute (creation de l'onglet)
		if (index == 0)
		{
			if (size)
				setRawSize(*size);
			else
				setRawSize(widgetSize);
		}
	}
}

//-----------------------------------------------------------------------------
//Ajout d'un widget dans un onglet vide
void SwDockWidget_MainDockWidget::insertWidget(int index, QWidget * widget)
{
	if (widget && index < count())
	{
		//Verification du contenu de l'onglet
		if (_tab->widget(index)/* && !_tab->widget(index)->layout()*/)
		{
			//Ajout du widget dans un nouveal onglet
			QGridLayout * lay = qobject_cast<QGridLayout*>(_tab->widget(index)->layout());
			if (!lay)
				lay = new QGridLayout(_tab->widget(index));
			if (lay)
			{
				lay->setObjectName("layout_" + QString::number(index));
				lay->addWidget(widget);
				lay->setMargin(0);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockWidget::removeWidget(int index)
{
	_tab->removeTab(index);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockWidget::updateContents(/*bool light*/)
{
	//Si un onglet est vide, on le supprime
	int index = 0;
	while(index < _tab->count())
	{
		QWidget * widget = _tab->widget(index);
		if (widget && widget->layout())
		{
			if (widget->layout()->count() == 0)
			{
				_tab->removeTab(index);
				//Si un onglet est supprime, on n'incremente pas l'index
				continue;
			}
		}
		//Passage a l'onglet suivant
		index++;
	}

	//S'il n'y a plus aucun onglet, on masque le dock principal
	if (/*this->isVisible() && */_tab->count() == 0)
	{
		/*if (light)
			this->resize(0,0);
		else*/
			//hide();
			close();
	}
}

//-----------------------------------------------------------------------------
int SwDockWidget_MainDockWidget::count()
{
	return _tab->count();
}

//-----------------------------------------------------------------------------
bool SwDockWidget_MainDockWidget::empty()
{
	return (count() == 0);
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_MainDockWidget::getWidget(int index)
{
	if (index < _tab->QTabWidget::count())
	{
		return _tab->widget(index);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainDockWidget::tabText(int index)
{
	return _tab->tabText(index);
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_MainDockWidget::getTabWidget()
{
	return _tab;
}

//-----------------------------------------------------------------------------
int SwDockWidget_MainDockWidget::currentIndex()
{
	return _tab->currentIndex();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockWidget::setCurrentWidget(int index)
{
	if (index < _tab->QTabWidget::count())
	{
		_tab->setCurrentIndex(index);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainDockWidget::setMoving(bool state)
{
	if (state)
	{
		//Mise a jour des proprietes
		setCanBeMoved(true);
		//Sauvegarde de l'aspect actuel
		_styleSheet = this->styleSheet();
		//Modification de l'aspect
		this->setStyleSheet("background: transparent; border:none;");
		this->hideShadow();
		setWindowOpacity(0.6);
	}
	else
	{
		//Restauration de l'aspect
		if (!_styleSheet.isEmpty())
			this->setStyleSheet(_styleSheet);
		this->showShadow();
		setWindowOpacity(1);
	}
}

//-----------------------------------------------------------------------------
bool SwDockWidget_MainDockWidget::isCurrentEmpty()
{
	bool empty = false;

	QWidget * widget = _tab->currentWidget();
	if (widget)
	{
		QLayout * layout = widget->layout();
		if (layout && (layout->count() == 0))
		{
			empty =true; 
		}
		else if (!layout)
			empty = true;
	}

	return empty;
}