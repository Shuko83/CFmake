#include "SwDockWidget_MainArea.h"
#include "SwDockWidget_DockWidget.h"
#include "SwDockWidget_TabWidget.h"
#include "SwDockWidget_MainDockMenuAction.h"
#include "SwDockWidget_MainDockConfiguration.h"

#include "SwDockWidget_MainTabBar.h" //DEBUG ONLY

//Qt
#include <QtCore/QTime>
#include <QStackedWidget>
#include <QPainter>
#include <QtXml/QDomDocument>
#include <QtCore/QFile>
#include <QSettings>
#include <QDir>
#include <QtCore/QCoreApplication>
#include <QKeyEvent>
#include <QAction>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QProxyStyle>

#define BTN_SIZE		32
#define BTN_CENTER_SIZE 96
#define PADDING			4

#define SAVE_TIMER				1000 * 60 * 5 //Valeur par defaut : 5 minutes (en millisecondes)
#define SAVE_TIMER_MIN_VALUE	1000 //Minimum 1 seconde

#define WIDGET_OVERLAY_COLOR			QColor(237, 254, 65,  130)
#define CENTRAL_WIDGET_OVERLAY_COLOR	QColor(03, 169, 244, 120)



//-----------------------------------------------------------------------------
//	Classe SwDockWidget_MainArea
//-----------------------------------------------------------------------------
SwDockWidget_MainArea::SwDockWidget_MainArea(QWidget *parent, QMenuBar * menuBar)
	: SwDockWidget_ToolBarWindow(parent), _menuBar(menuBar), _widgetMenu(NULL),
	_movingDock(""), _tempDock(NULL), _emptyWidget(NULL), _isReleasing(false),
	_isMovingDock(false), _mainDockConf(NULL), _posEffect(NULL), _lockAction(NULL),
	_saveTimer(NULL), _saveAutoPeriod(SAVE_TIMER)
{
	ui.setupUi(this);
	this->setMinimumSize(50,50);

	//Creation des boutons de positionnement des SwDockWidget_DockWidget
	createArrowsButton();
	//Creation de la zone de pre-positionnement
	_overlay = new SwDockWidget_Overlay(this, ui.topwidget);

	//Filtre sur les events
	if (parent)
		parent->installEventFilter(this);

	//Widgets principaux
	_leftMainDock = setupMainDockWidget("leftMainDock", Qt::LeftDockWidgetArea);
	_rightMainDock = setupMainDockWidget("rightMainDock", Qt::RightDockWidgetArea);
	_topMainDock = setupMainDockWidget("topMainDock", Qt::TopDockWidgetArea);
	_bottomMainDock = setupMainDockWidget("bottomMainDock", Qt::BottomDockWidgetArea);
	_secondScreenMainDock = setupMainDockWidget("secondScreenMainDock", Qt::NoDockWidgetArea);
	_secondScreenMainDock->setCanBeResize(false);
	_secondScreenMainDock->hideShadow();
	_secondScreenMainDock->setCanBePin(false);

	//Correctif maindock indédockable (Test si conforme mais règle le soucis : Ne déparente pas la fenêtre)
	//_secondScreenMainDock->setAlwaysOnTop(false);
	_quitOnClose = false;
	//Fichier de configuration par defaut mis lors du load

	//Creation d'un widget vide provisoire
	_emptyWidget = new QWidget(this);
	setMainWidget(_emptyWidget, _quitOnClose);

	//Initialisation
	init();

	installEventFilter(this);

	//Modification du style de l'application pour activer l'option "SloppySubMenu" permettant d'ajouter un delais a l'ouverture des menus
	/*QStyle * style = QApplication::style(); //Recuperation du style actif
	QApplication::setStyle(new SwDockWidget_Style(style)); //Application*/
	//Attention, le setStyle doit etre fait pour toute l'application!!!
}

//-----------------------------------------------------------------------------
SwDockWidget_MainArea::~SwDockWidget_MainArea()
{
	if (_mainWidget && _mainWidget != _emptyWidget)
		_mainWidget->setParent(0);
}

//-----------------------------------------------------------------------------
//Initialisation
void SwDockWidget_MainArea::init()
{
	//Creation des options de la barre titre
	setMenu();

	//Restauration des positions
	//loadDockPosition();
}

//-----------------------------------------------------------------------------
//Enregistrement du widget principal
void SwDockWidget_MainArea::setMainWidget(QWidget * widget, bool quitOnClose)
{
	if (widget)
	{
		if (ui.centralLayout->itemAt(0))
			ui.centralLayout->itemAt(0)->widget()->setParent(NULL);
		
		ui.centralLayout->addWidget(widget);

		_mainWidget = widget;
		_mainWidget->setObjectName("MainWidget");
		_mainWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

		//Mise a jour du widget dans les toolbar
		SwDockWidget_ToolBarWindow::updateMainWidget();

		if (_emptyWidget && _mainWidget != _emptyWidget)
		{
			delete _emptyWidget;
			_emptyWidget = 0;
		}
	}
	else
	{
		if (ui.centralLayout->itemAt(0))
			ui.centralLayout->itemAt(0)->widget()->setParent(NULL);

		//Creation d'un widget vide provisoire
		_emptyWidget = new QWidget(this);
		ui.centralLayout->addWidget(_emptyWidget);
		_mainWidget = _emptyWidget;

		//Mise a jour du widget dans les toolbar
		SwDockWidget_ToolBarWindow::updateMainWidget();
	}

	_quitOnClose = quitOnClose;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_MainArea::getMainWidget()
{
	return _mainWidget;
}

//-----------------------------------------------------------------------------
// Barre de menu
//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::setMenu()
{
	if (_menuBar)
	{
		//Liste des widgets existants pour afficher ou masquer
		_widgetMenu = _menuBar->addMenu(tr("Widgets"));

		// Choix de la configuration
		_mainDockConf = new SwDockWidget_MainDockConfiguration("Desktop", _menuBar);
		connect(_mainDockConf, SIGNAL(changeConf(SwDockWidget_MainDockConfiguration::ConfigurationIndex)), this, SLOT(changeDockConfiguration(SwDockWidget_MainDockConfiguration::ConfigurationIndex)));
		_menuBar->addMenu(_mainDockConf);

		//Gestion des docks principaux
		createMainWidgetAction(_topMainDock , ":/DockWidget/images/DockWidget/hideTop.png"
											, ":/DockWidget/images/DockWidget/hideTopDisabled.png"
											, ":/DockWidget/images/DockWidget/hideTopSelected.png");
		createMainWidgetAction(_leftMainDock, ":/DockWidget/images/DockWidget/hideLeft.png"
											, ":/DockWidget/images/DockWidget/hideLeftDisabled.png"
											, ":/DockWidget/images/DockWidget/hideLeftSelected.png");
		createMainWidgetAction(_rightMainDock, ":/DockWidget/images/DockWidget/hideRight.png"
											 , ":/DockWidget/images/DockWidget/hideRightDisabled.png"
											 , ":/DockWidget/images/DockWidget/hideRightSelected.png");
		createMainWidgetAction(_bottomMainDock, ":/DockWidget/images/DockWidget/hideBottom.png"
											  , ":/DockWidget/images/DockWidget/hideBottomDisabled.png"
											  , ":/DockWidget/images/DockWidget/hideBottomSelected.png");

		//Lock / Unlock
		_lockAction = new SwDockWidget_MainDockMenuAction(this,
				":/DockWidget/images/DockWidget/unlocked.png",
				":/DockWidget/images/DockWidget/unlocked.png",
				":/DockWidget/images/DockWidget/locked.png");
		_lockAction->setEnabled(true);
		_menuBar->addAction(_lockAction);
		connect(_lockAction, SIGNAL(toggled(bool)), this, SLOT(setLock(bool)));
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::createMainWidgetAction(SwDockWidget_DockWidget * dock, QString enabledIcon, QString disabledIcon, QString checkedIcon)
{
	if (dock && !enabledIcon.isEmpty())
	{
		//Creation de l'icone
		SwDockWidget_MainDockMenuAction * action = new SwDockWidget_MainDockMenuAction(dock, enabledIcon, disabledIcon, checkedIcon);
		_menuBar->addAction(action);
		connect(action, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
		dock->setMenuAction(action);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::addWidgetInMenu(SwDockWidget_DockWidget * widget, QString menuName)
{
	if (_widgetMenu && widget)
	{
		QMenu * menu = _widgetMenu;
		//Si ajout dans un sous-menu
		if (menuName.compare(""))
		{
			menu = NULL;
			//Recherche de l'existance du sous-menu
			if (_listSousMenu.contains(menuName))
				menu = _listSousMenu.value(menuName);
			//S'il n'existe pas, on le cree
			else
			{
				menu = new QMenu(menuName, _widgetMenu);
				_widgetMenu->addMenu(menu);
				_listSousMenu.insert(menuName, menu);
			}
		}

		QAction * action = menu->addAction(widget->getTitle());
		action->setCheckable(true);
		action->setChecked(true);
		connect(action, SIGNAL(triggered(bool)), this, SLOT(switchWidgetStateFromMenu(bool)));
		widget->setMenuAction(action);
		action->setParent(widget);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::switchWidgetStateFromMenu(bool visible)
{
	QAction * a = qobject_cast<QAction*>(this->sender());
	SwDockWidget_DockWidget * dock = NULL;
	if (a)	
		dock = qobject_cast<SwDockWidget_DockWidget*>(a->parentWidget());

	if (dock)
	{
		if (visible)
		{
			//Affichage centre sur l'ecran en cours
			dock->move(QApplication::desktop()->availableGeometry(QCursor::pos()).center() - dock->rect().center());
			
			//Affichage du dock
			dock->setWindowOpacity(1);
			dock->show();

			//Mise a jour du menu
			if (dock->getMenuAction())
				dock->getMenuAction()->setChecked(true);
		}
		else
		{
			//Liberation du dock
			releaseDock(dock);
			//Suppression de la toolbar associee si necessaire
			releaseFromToolBar(dock);
			//Mise a jour du menu
			if (dock->getMenuAction())
				dock->getMenuAction()->setChecked(false);
			//Mise a jour des docks principaux
			updateMainDock();
			//Masquage du dock
			dock->hide();
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::changeDockConfiguration(SwDockWidget_MainDockConfiguration::ConfigurationIndex index)
{
	QList<SwDockWidget_MainDockWidget*> list;

	switch(index)
	{
		case SwDockWidget_MainDockConfiguration::BLTR:
			list.push_back(_bottomMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			list.push_back(_rightMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::BRLT:
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::BRTL:
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_topMainDock);
			list.push_back(_leftMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::LBRT:
			list.push_back(_leftMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_topMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::LTBR:
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::LTRB:
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_bottomMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::RBLT:
			list.push_back(_rightMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::RLTB:
			list.push_back(_rightMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_topMainDock);
			list.push_back(_bottomMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::RTBL:
			list.push_back(_rightMainDock);
			list.push_back(_topMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_leftMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::RTLB:
			list.push_back(_rightMainDock);
			list.push_back(_topMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_bottomMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::TBRL:
			list.push_back(_topMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_leftMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::TLBR:
			list.push_back(_topMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_rightMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::TRBL:
			list.push_back(_topMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_bottomMainDock);
			list.push_back(_leftMainDock);
			break;

		case SwDockWidget_MainDockConfiguration::TRLB:
			list.push_back(_topMainDock);
			list.push_back(_rightMainDock);
			list.push_back(_leftMainDock);
			list.push_back(_bottomMainDock);
			break;

		default:
			break;
	}

	//Liberation des docks principaux
	QList<int> listSize;
	for(SwDockWidget_MainDockWidget * w : list)
	{
		SwDockWidget_Splitter * splitter = qobject_cast<SwDockWidget_Splitter*>(w->parentWidget());
		if (w && !w->empty())
		{
			QSize s = w->getRawSize();
			Qt::DockWidgetArea area = w->getArea();

			switch(w->getArea())
			{
				case Qt::LeftDockWidgetArea:
					listSize.push_back(splitter->sizes().first());
					break;
				case Qt::RightDockWidgetArea:
					listSize.push_back(splitter->sizes().last());
					break;
				case Qt::TopDockWidgetArea:
					listSize.push_back(splitter->sizes().first());
					break;
				case Qt::BottomDockWidgetArea:
					listSize.push_back(splitter->sizes().last());
					break;
				default:
					break;
			}

			releaseDock(w);
			w->setArea(area);
			w->setRawSize(s);
		}
	}

	//Application de la nouvelle configuration
	for(SwDockWidget_MainDockWidget * w : list)
	{
		if (w && !w->empty())
			pinDockTo(w, _mainWidget, w->getArea(), true);
	}
	_mainDockConf->setActiveConfiguration(index);

	//Restauration des dimensions
	qApp->processEvents(); //Force le rafraichissement pour mettre a jour les dimensions
	for (int i = listSize.size()-1; i >= 0; i--)
	{
		SwDockWidget_MainDockWidget * w = list.at(i);
		SwDockWidget_Splitter * s = qobject_cast<SwDockWidget_Splitter*>(w->parentWidget());
		if (w && s)
		{
			int value = 0;
			switch(w->getArea())
			{
				case Qt::LeftDockWidgetArea:
					value = listSize.at(i);
					break;
				case Qt::RightDockWidgetArea:
					value = s->width() - listSize.at(i);
					break;
				case Qt::TopDockWidgetArea:
					value = listSize.at(i);
					break;
				case Qt::BottomDockWidgetArea:
					value = s->height() - listSize.at(i);
					break;
				default:
					break;
			}
			s->moveSplitter(value);
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::updateConfiguration()
{
	//Element de plus haut niveau
	if (ui.mainLayout->itemAt(0))
	{
		SwDockWidget_Splitter * ds = qobject_cast<SwDockWidget_Splitter*>(ui.mainLayout->itemAt(0)->widget());
		if (ds)
			_mainDockConf->setActiveConfiguration(checkConfiguration(ds));
	}
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainArea::checkConfiguration(QSplitter * splitter)
{
	QString s("");
	bool found = false;
	if (splitter)
	{
		int index = 1;
		SwDockWidget_MainDockWidget * w = qobject_cast<SwDockWidget_MainDockWidget*>(splitter->widget(0));
		if (w)
		{
			found = true;
		}
		else
		{
			w = qobject_cast<SwDockWidget_MainDockWidget*>(splitter->widget(1));
			index = 0;
			if (w)
			{
				found = true;
			}
		}
		if (found)
		{
			//Si un dock a ete trouve mais qu'il est vide, on continue la recherche sans l'ajouter
			if (w && w->empty())
				return checkConfiguration(qobject_cast<QSplitter*>(splitter->widget(index)));
			//Sinon, on l'ajoute et on continue
			else if (w)
				return checkConfiguration(qobject_cast<QSplitter*>(splitter->widget(index))) + getStringFromMainDock(w);
		}
		else
		{
			//Si le splitter n'a qu'un seul element, on verifie sa descendance
			if (splitter->count() == 1)
				return checkConfiguration(qobject_cast<QSplitter*>(splitter->widget(index)));
		}
	}
	return s;
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainArea::getStringFromMainDock(SwDockWidget_MainDockWidget * dock)
{
	QString s("");
	if (dock)
	{
		if (dock == _leftMainDock)
			s = QString("L");
		else if (dock == _rightMainDock)
			s = QString("R");
		else if (dock == _topMainDock)
			s = QString("T");
		else if (dock == _bottomMainDock)
			s = QString("B");
	}
	return s;
}

//-----------------------------------------------------------------------------
// Evenements
//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::closeEvent(QCloseEvent * event )
{
	close();

	event->accept();
}

//-----------------------------------------------------------------------------
bool SwDockWidget_MainArea::close()
{
//	EXCEPTION_TRY();

	//Masquage de la fenetre
	hide();

	//Sauvegarde des positions et dimensions de tous les elements
	saveAllPositions();

	//Fermeture du dock principal de deuxieme ecran si necessaire
	_secondScreenMainDock->close();

	//Fermeture des toolbars
	closeToolbars();

	//Fermeture des docks flottants
	for(QObject * obj : _list)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
		if (dock && dock->parent() == this && !dock->isInToolBar())
		{
			dock->close();
		}
	}

	if (_quitOnClose)
		qApp->quit();	

	return true;

	//EXCEPTION_CATCH();
}

//-----------------------------------------------------------------------------
//Masquage de tous les dock non ancres lorsque l'application est minimisee
void SwDockWidget_MainArea::hideAll()
{
	//Masquage du dock de deuxieme ecran
	if (_secondScreenMainDock->isVisible())
		_secondScreenMainDock->hide();

	//Sauvegarde des positions et dimensions de tous les elements
	saveAllPositions();
}

//-----------------------------------------------------------------------------
//Gestion des evenements de l'Area
bool SwDockWidget_MainArea::event( QEvent * event )
{
	switch(event->type())
	{
		//Fermeture de l'application
		case QEvent::Close:
			close();
			break;
		
		//Reduction de l'application
		case QEvent::Hide:
			hideAll();
			break;

		//Affichage de l'application
		case QEvent::Show:
			//Affichage si necessaire du dock de deuxieme ecran
			if (!_secondScreenMainDock->empty())
				_secondScreenMainDock->show();
			break;
	}

	return QWidget::event(event);
}

//-----------------------------------------------------------------------------
//Gestion des evenements des DockWidgets
bool SwDockWidget_MainArea::eventFilter( QObject *obj , QEvent * event )
{
	//Reperage de tous les onglets ouverts
	if (event->type() == QEvent::KeyRelease)
	{
		QKeyEvent * e = (QKeyEvent*)event;
		if (e && e->key() == Qt::Key_F12 && e->modifiers() & Qt::ControlModifier && e->modifiers() & Qt::ShiftModifier)
		{
			for(QObject * o : _list)
			{
				SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(o);
				if (dock)
				{
					dock->blink();
				}
			}
		}
	}

	//Evenements filtres sur les SwDockWidget_DockWidget
	else if (_list.contains(obj) || _listMainDock.contains(obj))
	{
		SwDockWidget_DockWidget * dock = NULL;
		switch(event->type())
		{
			//Clic initial : selection de l'objet, debut de deplacement
			case QEvent::MouseButtonPress:
				dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
				if (dock && dock->canBePin() && !locked())
				{
					_isMovingDock = true;
					_movingDock = obj->objectName();
				}
				break;

			//Deplacement
			case QEvent::Move:
				if (!_isReleasing && !locked())
				{
					//Si un dock est deplace suite a un clic (pour differencier d'un deplacement automatique non initie par l'utilisateur)
					if (_isMovingDock && !_movingDock.compare(obj->objectName()))
					{
						//Verification du deplacement du dock
						dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
						if (dock && dock->canMove() && dock->isMoving())
						{
							//Liberation du dock si necessaire
							if (obj->parent() != this)
								releaseDock(obj);

							//Gestion des fleches
							if (obj->parent() == this && dock->canBePin())
								manageArrows(obj);
						}
					}
				}
				break;

			//Fin de clic, relachement, fin de deplacement
			case QEvent::MouseButtonRelease:
				if (_isMovingDock && !locked())
				{
					_isMovingDock = false;
					_movingDock = "";

					//Masquage des fleches de positionnement
					hideArrows();

					//Ajout du dock si necessaire
					managePinDock(obj, getDockableWidget(/*obj*/));

					//Mise a jour des docks principaux
					updateMainDock();
				}
				break;

			//Fermeture du SwDockWidget_DockWidget
			case QEvent::Close:
				dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
				
				if (_list.contains(obj))
				{
					//Liberation du dock avant fermeture
					releaseDock(obj);
					//Suppression de la toolbar associee si necessaire
					releaseFromToolBar(dock);
					//Mise a jour des docks principaux
					updateMainDock();
				}
				//Desactivation du bouton de masquage si dock principal
				if (_listMainDock.contains(obj))
				{
					SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
					if (mainDock && mainDock->getMenuAction() && mainDock->empty())
						mainDock->getMenuAction()->setEnabled(false);
				}
				break;

			case QEvent::Show:
				//Reactivation si necessaire du bouton de masquage du dock principal
				if (_listMainDock.contains(obj))
				{
					SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
					if (mainDock && mainDock->getMenuAction())
					{
						mainDock->getMenuAction()->setEnabled(true);
						mainDock->getMenuAction()->setChecked(true);
					}
				}
				else if (_list.contains(obj))
				{
					dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
					if (dock && dock->getMenuAction())
						dock->getMenuAction()->setChecked(true);
				}
				break;

			case QEvent::Hide:
				//Desactivation du bouton de masquage si dock principal
				if (_listMainDock.contains(obj))
				{
					SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
					if (mainDock && mainDock->getMenuAction() && mainDock->empty())
						mainDock->getMenuAction()->setEnabled(false);
				}
				break;
		}

	}

	//Evenement sur le parent
	else if (obj == this->parent())
	{
		switch(event->type())
		{
			case QEvent::Move:
				//Verification de la position globale de la fenetre pour positionner si necessaire le dock principal de deuxieme ecran
				if (!_secondScreenMainDock->empty())
				{
					int appScreen = QApplication::desktop()->screenNumber(mapToGlobal(this->rect().center()));
					int dockScreen = QApplication::desktop()->screenNumber(_secondScreenMainDock->mapToGlobal(QPoint(0,0)));
					if (appScreen == dockScreen)
					{
						QRect rect = QApplication::desktop()->availableGeometry(1-dockScreen);

						//Changement d'ecran
						if (!_posEffect || //Si aucun changement d'ecran n'a ete initialise
							_posEffect->state() != QAbstractAnimation::Running || //Si le dernier changement est termine
							_posEffect->endValue() != QPoint(rect.x(), rect.y())) //Si la cible doit etre changee
						{
							if (_posEffect && _posEffect->state() == QAbstractAnimation::Running)
								_posEffect->stop();
							
							//Deplacement dans l'autre ecran, avec effet de glissement rapide
							_posEffect = new QPropertyAnimation(_secondScreenMainDock, "pos");
							_posEffect->setDuration(220);
							_posEffect->setStartValue(_secondScreenMainDock->mapToGlobal(QPoint(0,0)));
							_posEffect->setEndValue(QPoint(rect.x(), rect.y()));
							_posEffect->start();					
							//Redimensionnement si necessaire
							_secondScreenMainDock->resize(rect.size());
						}
					}
				}
				break;

			case QEvent::Close:
				close();
				break;
		}
	}

	//Evenement non filtre (Gestion normale de l'event)
	return false;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::lockConf(bool state)
{
	if (state == true)
		lock();
	else
		releaseLock();
}

//-----------------------------------------------------------------------------
// Gestion des docks
//-----------------------------------------------------------------------------
//Enregistrement d'un dock dans la liste
void SwDockWidget_MainArea::addDockWidget( SwDockWidget_DockWidget * dock, QString menuName)
{
	//Enregistrement du dock
	_list.append(dock);

	//Mise a jour des proprietes du dock
	dock->setMainArea(this);

	//Ecoute des evenements du dock
	dock->installEventFilter(this);

	//Gestion du masquage
	connect(dock, SIGNAL(releaseDockAsked()), this, SLOT(releaseDock()));
	connect(dock, SIGNAL(reduceInToolBarAsked()), this, SLOT(reduceInToolBar()));
	connect(dock, SIGNAL(releaseFromToolBarAsked()), this, SLOT(releaseFromToolBar()));

	//Ajout du dock dans le menu
	addWidgetInMenu(dock, menuName);

	//Affichage du dock
	dock->setWindowOpacity(1);
	dock->show();
	if (this->locked())
		dock->lock();
}

//-----------------------------------------------------------------------------
//Suppression d'un dock
void SwDockWidget_MainArea::removeDockWidget(SwDockWidget_DockWidget * dock)
{
	if (_list.contains(dock))
	{
		//Liberation du dock
		dock->close();
		dock->setParent(0);

		//Suppression de la liste
		_list.removeOne(dock);

		//Ecoute des evenements du dock
		dock->removeEventFilter(this);

		//Gestion du masquage
		disconnect(dock, SIGNAL(releaseDockAsked()), this, SLOT(releaseDock()));
		disconnect(dock, SIGNAL(reduceInToolBarAsked()), this, SLOT(reduceInToolBar()));
		disconnect(dock, SIGNAL(releaseFromToolBarAsked()), this, SLOT(releaseFromToolBar()));

		//Mise a jour du menu
		if (dock->getMenuAction())
		{
			delete dock->getMenuAction();
			dock->setMenuAction(0);
		}

		if (dock->getToolBarItem())
			dock->setToolBarItem(0);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::removeMenu(QString menu)
{
	if (_listSousMenu.contains(menu))
	{
		_widgetMenu->removeAction(_listSousMenu.value(menu)->menuAction());
		_listSousMenu.remove(menu);
	}
}

//-----------------------------------------------------------------------------
//Ancrage d'un dock
QWidget * SwDockWidget_MainArea::pinDockTo(QObject * obj, QWidget * mainWidget, Qt::DockWidgetArea area, bool absolute)
{
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
	if (mainWidget && dock)
	{
		//Recuperation de la position du widget principal
		QSize size = dock->getRawSize();
		//Verification de tous les cas possible (horizontal ou vertical layout, grid layout, splitter, tabWidget)
		QGridLayout *glayout = NULL;
		QVBoxLayout *vlayout = NULL;
		QHBoxLayout *hlayout = NULL;
		SwDockWidget_Splitter *parentSplitter = NULL;
		QWidget *parentWidget = mainWidget->parentWidget();
		//Preparation de la creation du widget qui va accueillir le SwDockWidget_DockWidget
		SwDockWidget_TabWidget *tabWidget = qobject_cast<SwDockWidget_TabWidget*>(mainWidget);
		SwDockWidget_Splitter *splitter = NULL;
		QWidget *toAdd = NULL;		
		//Verification du widget auquel est accroche le SwDockWidget_DockWidget
		SwDockWidget_DockWidget *secondDock = qobject_cast<SwDockWidget_DockWidget*>(mainWidget);
		
		//Si le SwDockWidget_DockWidget est attache au widget principal, on recupere le layout principal
		if (parentWidget == this)
			glayout = ui.mainLayout;
		//Sinon on cherche le layout parent
		else if (parentWidget)
		{
			glayout = qobject_cast<QGridLayout*>(parentWidget->layout());
			vlayout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
			hlayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
			parentSplitter = qobject_cast<SwDockWidget_Splitter*>(parentWidget);
		}

		int num = 0;
		//Recuperation de la position du widget principal dans le parent pour le repositionner apres ancrage du SwDockWidget_DockWidget
		if (tabWidget)
		{
			num = tabWidget->indexOf(mainWidget);
			secondDock = qobject_cast<SwDockWidget_DockWidget*>(tabWidget->widget(0));
		}
		int row = 0, column = 0, rowSpan = 0, columnSpan = 0;
		if (glayout)
		{
			if (glayout->indexOf(mainWidget) < 0)
				return NULL;
			glayout->getItemPosition(glayout->indexOf(mainWidget), &row, &column, &rowSpan, &columnSpan);
		}
		else if (vlayout)
			row = vlayout->indexOf(mainWidget);
		else if (hlayout)
			column = hlayout->indexOf(mainWidget);
		else if (parentSplitter)
			num = parentSplitter->indexOf(mainWidget);

		//---------------
		//Positionnement dans un dock principal
		if (absolute == true)
		{
			if (ui.mainLayout->itemAt(0))
			{
				QWidget * widget = ui.mainLayout->itemAt(0)->widget();
				if (widget)
				{
					switch(area)
					{
						case Qt::LeftDockWidgetArea :
							//Creation d'un splitter
							splitter = new SwDockWidget_Splitter(Qt::Horizontal, this);
							splitter->fill(dock, widget, area, Qt::NoDockWidgetArea, this->width()/3);
							break;

						case Qt::RightDockWidgetArea :
							//Creation d'un splitter
							splitter = new SwDockWidget_Splitter(Qt::Horizontal, this);
							splitter->fill(widget, dock, area, Qt::NoDockWidgetArea, this->width()/3);
							break;

						case Qt::TopDockWidgetArea:
							//Creation d'un splitter
							splitter = new SwDockWidget_Splitter(Qt::Vertical, this);
							splitter->fill(dock, widget, area, Qt::NoDockWidgetArea, this->height()/3);
							break;

						case Qt::BottomDockWidgetArea:
							//Creation d'un splitter
							splitter = new SwDockWidget_Splitter(Qt::Vertical, this);
							splitter->fill(widget, dock, area, Qt::NoDockWidgetArea, this->height()/3);
							break;
					}
					
					glayout = ui.mainLayout;
					row = 0;
					column = 0;
					rowSpan = 0;
					columnSpan = 0;
				}
			}
		}
		//---------------
		//Positionnement par rapport au widget
		else
		{
			switch(area)
			{
				case Qt::LeftDockWidgetArea:
					//Creation d'un splitter
					splitter = new SwDockWidget_Splitter(Qt::Horizontal, this);
					splitter->fill(dock, mainWidget, area, (secondDock?secondDock->getArea():Qt::NoDockWidgetArea));
					break;

				case Qt::RightDockWidgetArea:
					//Creation d'un splitter
					splitter = new SwDockWidget_Splitter(Qt::Horizontal, this);
					splitter->fill(mainWidget, dock, area, (secondDock?secondDock->getArea():Qt::NoDockWidgetArea));
					break;

				case Qt::TopDockWidgetArea:
					//Creation d'un splitter
					splitter = new SwDockWidget_Splitter(Qt::Vertical, this);
					splitter->fill(dock, mainWidget, area, (secondDock?secondDock->getArea():Qt::NoDockWidgetArea));
					break;

				case Qt::BottomDockWidgetArea:
					//Creation d'un splitter
					splitter = new SwDockWidget_Splitter(Qt::Vertical, this);
					splitter->fill(mainWidget, dock, area, (secondDock?secondDock->getArea():Qt::NoDockWidgetArea));
					break;

				//Ajout dans un onglet (possible uniquement dans un dock)
				case Qt::NoDockWidgetArea:
				default:
					if (_mainWidget && mainWidget != _mainWidget)
					{
						//Si un onglet est deja present, ajout dans l'existant
						if (tabWidget)
						{
							
						}
						//Sinon creation d'un nouvel onglet
						else
						{
							tabWidget = new SwDockWidget_TabWidget(this);
							tabWidget->addTab(mainWidget);
						}
						tabWidget->addTab(dock);
						if (secondDock)
							dock->setArea(secondDock->getArea());
						//Focus sur le dock ajoute
						tabWidget->setCurrentWidget(dock);
					}
					break;
			}
		}
		
		//Ajout du splitter ou de l'onglet
		if (splitter)
			toAdd = splitter;
		else if (tabWidget)
			toAdd = tabWidget;
		if (dock && toAdd)
		{
			if (glayout)
				glayout->addWidget(toAdd, row, column, rowSpan, columnSpan);
			else if (vlayout)
				vlayout->insertWidget(row, toAdd);
			else if (hlayout)
				hlayout->insertWidget(column, toAdd);
			else if (parentSplitter)
				parentSplitter->replaceWidget(toAdd, num);
			else
			{
				
			}
		}

		if (tabWidget)
			return toAdd;
		else
			return dock;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//Ancrage d'un dock
QWidget * SwDockWidget_MainArea::managePinDock(QObject * obj, QWidget * mainWidget)
{
	QWidget * toReturn = NULL;
	if (mainWidget && obj)
	{
		QPoint pos = QCursor::pos();
		SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
		SwDockWidget_MainTabWidget * toMainTabBar = qobject_cast<SwDockWidget_MainTabWidget*>(mainWidget);
		//---------------
		//Positionnement dans un dock principal
		if (_mainWidget && mainWidget == _mainWidget)
		{
			//Si le dock en cours de deplacement est un dock principal, on ne deplace que son contenu
			QWidget * dock = qobject_cast<QWidget*>(obj);
			QString name = "";
			QSize * size = NULL;
			if (mainDock)
			{
				//Recuperation du contenu de l'onglet principal
				dock = mainDock->getWidget(0);
				if (dock && dock->layout())
				{
					QLayoutItem * litem = dock->layout()->itemAt(0);
					if (litem)
						toReturn = dock = litem->widget();
				}
				//Recuperation du nom et de la taille de l'onglet
				name = mainDock->tabText(0);
				size = new QSize(mainDock->getRawSize());
			}

			//Si dock a ancrer a gauche
			if(_leftCentralBtn->isVisible() && QRect(_leftCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
			{
				_leftMainDock->addWidget(dock, name, size);
				if (_leftMainDock->count() == 1)
					toReturn = pinDockTo(_leftMainDock, mainWidget, Qt::LeftDockWidgetArea, true);
			}

			//Si dock a ancrer a droite
			else if(_rightCentralBtn->isVisible() && QRect(_rightCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
			{
				_rightMainDock->addWidget(dock, name, size);
				if (_rightMainDock->count() == 1)
					toReturn = pinDockTo(_rightMainDock, mainWidget, Qt::RightDockWidgetArea, true);
			}

			//Si dock a ancrer en haut
			else if(_topCentralBtn->isVisible() && QRect(_topCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
			{
				_topMainDock->addWidget(dock, name, size);
				if (_topMainDock->count() == 1)
					toReturn = pinDockTo(_topMainDock, mainWidget, Qt::TopDockWidgetArea, true);
			}

			//Si dock a ancrer en bas
			else if(_bottomCentralBtn->isVisible() && QRect(_bottomCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
			{
				_bottomMainDock->addWidget(dock, name, size);
				if (_bottomMainDock->count() == 1)
					toReturn = pinDockTo(_bottomMainDock, mainWidget, Qt::BottomDockWidgetArea, true);
			}
			else
				toReturn = NULL;

			//Affichage si necessaire du dock
			if (mainDock && toReturn)
				dock->show();
		}

		//---------------
		//Positionnement par rapport au widget (non propose si le dock deplace est un dock principal)
		else if (!mainDock &&!toMainTabBar)
		{
			//Ajout dans un dock principal
			if (qobject_cast<SwDockWidget_MainDockWidget*>(mainWidget))
			{
				mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(mainWidget);

				if(_centerDockBtn->isVisible() && QRect(_centerDockBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
				{
					int index = mainDock->currentIndex();
					//Si l'onglet en cours est vide
					mainDock->insertWidget(index, qobject_cast<QWidget*>(obj));
				}
			}
			else
			{
				//Si le dock cible est dans un onglet, on insere le nouveau dock dans cet onglet
				if (mainWidget->parentWidget() && qobject_cast<QStackedWidget*>(mainWidget->parentWidget()))
				{
					QWidget * w = qobject_cast<QStackedWidget*>(mainWidget->parentWidget());
					if (w->parentWidget() && qobject_cast<SwDockWidget_TabWidget*>(w->parentWidget()))
						mainWidget = qobject_cast<SwDockWidget_TabWidget*>(w->parentWidget());
				}

				//Si dock a ancrer a gauche
				if(_leftWidgetBtn->isVisible() && QRect(_leftWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
					toReturn = pinDockTo(obj, mainWidget, Qt::LeftDockWidgetArea);

				//Si dock a ancrer a droite
				else if(_rightWidgetBtn->isVisible() && QRect(_rightWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
					toReturn = pinDockTo(obj, mainWidget, Qt::RightDockWidgetArea);

				//Si dock a ancrer en haut
				else if(_topWidgetBtn->isVisible() && QRect(_topWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
					toReturn = pinDockTo(obj, mainWidget, Qt::TopDockWidgetArea);

				//Si dock a ancrer en bas
				else if(_bottomWidgetBtn->isVisible() && QRect(_bottomWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
					toReturn = pinDockTo(obj, mainWidget, Qt::BottomDockWidgetArea);

				//Ajout dans un onglet
				else if(_tabBtn->isVisible() && QRect(_tabBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
					toReturn = pinDockTo(obj, mainWidget, Qt::NoDockWidgetArea);
			}
		}
		//Si ajout dans un nouvel onglet d'un dock principal
		else if (toMainTabBar)
		{
			//Recherche du main dock parent
			QWidget * parent = toMainTabBar->parentWidget();
			mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(parent);
			int itr = 0; //Protection!
			while(!mainDock && itr < 20)
			{
				parent = (parent->parentWidget());
				mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(parent);
				itr++;
			}
			if (mainDock)
			{
				mainDock->addWidget(qobject_cast<QWidget*>(obj));
			}
		}
		if (toReturn)
			return toReturn;
	}
	//Ajout dans un dock principal dans le deuxieme ecran
	if (obj && QRect(_secondScreenBtn->pos(),QSize(BTN_CENTER_SIZE,BTN_CENTER_SIZE)).contains(QCursor::pos()))
	{
		QRect rect = QApplication::desktop()->availableGeometry(QCursor::pos());
		//Si le dock en cours de deplacement est un dock principal, on ne deplace que son contenu
		QWidget * dock = qobject_cast<QWidget*>(obj);
		SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
		QString name = "";
		QSize * size = NULL;
		if (mainDock)
		{
			//Recuperation du contenu de l'onglet principal
			dock = mainDock->getWidget(0);
			if (dock && dock->layout())
			{
				QLayoutItem * litem = dock->layout()->itemAt(0);
				if (litem)
					dock = litem->widget();
			}
			//Recuperation du nom et de la taille de l'onglet
			name = mainDock->tabText(0);
			size = new QSize(mainDock->getRawSize());
		}

		//Ajout du widget dans le dock principal de deuxieme ecran
		_secondScreenMainDock->addWidget(dock, name);
		_secondScreenMainDock->resize(rect.size());
		_secondScreenMainDock->move(rect.x(), rect.y());
		_secondScreenMainDock->show();

		toReturn = dock;
	}
	return toReturn;
}

//-----------------------------------------------------------------------------
//Liberation d'un dock
void SwDockWidget_MainArea::releaseDock(QObject * obj)
{
	SwDockWidget_DockWidget * dock = NULL;

	//Recuperation du dock selectionne
	if (!obj)
		dock = qobject_cast<SwDockWidget_DockWidget*>(sender());
	else
		dock = qobject_cast<SwDockWidget_DockWidget*>(obj);

	if(dock && dock->parent() != this && dock->parent())
	{
		//Si widget dans un onglet
		if (dock->isInTab())
		{
			QStackedWidget * stacked = qobject_cast<QStackedWidget*>(dock->parent());
			if (stacked && stacked->parent())
			{
				_isReleasing = true;				

				//Recuperation de la position du dock
				QPoint pos = dock->pos();
				
				SwDockWidget_TabWidget * tab = qobject_cast<SwDockWidget_TabWidget*>(stacked->parent());

				//Liberation du dock
				if (tab)
				{
					dock->returnToMainArea();
					dock->setInTab(false);
					dock->setArea(Qt::NoDockWidgetArea);
					//Mise a jour de la position
					dock->move(mapToGlobal(pos));
					dock->restoreSize();
					dock->show();
				}
				else
				{
					return;
				}
				//Si l'onglet ne contient plus qu'un element, on le libere
				if ( (tab->count() == 1) && tab->widget(0))
				{
					SwDockWidget_DockWidget * dock2 = qobject_cast<SwDockWidget_DockWidget*>(tab->widget(0));

					//Recuperation du parent du splitter
					QWidget * parentWidget = tab->parentWidget();
					QGridLayout * glayout = qobject_cast<QGridLayout*>(parentWidget->layout());
					QVBoxLayout * vlayout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
					QHBoxLayout * hlayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
					SwDockWidget_Splitter *parentSplitter = qobject_cast<SwDockWidget_Splitter*>(parentWidget);

					//Recuperation de la position dans le parent
					int row = 0 , column = 0 , rowSpan = 0 , columnSpan = 0 , num = 0 ;
					if (glayout)
						glayout->getItemPosition(glayout->indexOf(tab), &row, &column, &rowSpan, &columnSpan);
					else if (vlayout)
						row = vlayout->indexOf(tab);
					else if (hlayout)
						column = hlayout->indexOf(tab);
					else if (parentSplitter)
						num = parentSplitter->indexOf(tab);
					
					if (dock2)
					{
						//On supprime le splitter, et on remet le widget qui n'est pas le dock dans le parent
						if (glayout)
						{
							glayout->addWidget(dock2, row, column, rowSpan, columnSpan);
							glayout->removeWidget(tab);
							dock2->show();
						}
						else if (vlayout)
						{
							vlayout->removeWidget(tab);
							vlayout->insertWidget(row, dock2);
						}
						else if (hlayout)
						{
							hlayout->removeWidget(tab);
							hlayout->insertWidget(column, dock2);
						}
						else if (parentSplitter)
						{
							tab->setParent(0);
							parentSplitter->replaceWidget(dock2, num);
						}

						dock2->setInTab(false);
					}
				}
				_isReleasing = false;
			}
		}
		else
		{
			//Si splitter a supprimer (liberation du dock precedemment ancre)
			SwDockWidget_Splitter *splitter = qobject_cast<SwDockWidget_Splitter*>(dock->parent());
			if (splitter)
			{
				//Recuperation des deux elements qui constituent le splitter
				int nb = splitter->count();
				if ((nb >= 2) && !_isReleasing)
				{
					_isReleasing = true;

					//Recuperation de la position du dock
					QPoint pos = dock->pos();
					
					//Recuperation des elements du splitter
					QWidget * wid1 = splitter->widget(0);
					QWidget * wid2 = splitter->widget(1);

					//Recherche de l'element du splitter qui est le dock actif
					QWidget * toMain = NULL;
					if (dock == wid1)
						toMain = wid2;
					else
						toMain = wid1;

					//Liberation des deux elements
					if (wid1)
					{
						SwDockWidget_DockWidget * tempdock = qobject_cast<SwDockWidget_DockWidget*>(wid1);
						if (tempdock)
							tempdock->returnToMainArea();
						else
							wid1->setParent(0);
					}
					if (wid2)
					{
						SwDockWidget_DockWidget * tempdock = qobject_cast<SwDockWidget_DockWidget*>(wid2);
						if (tempdock)
							tempdock->returnToMainArea();
						else
							wid2->setParent(0);
					}
					dock->setArea(Qt::NoDockWidgetArea);

					//Mise a jour de la position et de la taille du dock
					dock->move(mapToGlobal(pos));
					dock->restoreSize();
					dock->show();

					//Recuperation du parent du splitter
					QWidget * parentWidget = splitter->parentWidget();
					QGridLayout * glayout = NULL;
					QVBoxLayout * vlayout = NULL;
					QHBoxLayout * hlayout = NULL;
					SwDockWidget_Splitter *parentSplitter = NULL;
					
					if (parentWidget == this)
						glayout = ui.mainLayout;
					else if (parentWidget)
					{
						glayout = qobject_cast<QGridLayout*>(parentWidget->layout());
						vlayout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
						hlayout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
						parentSplitter = qobject_cast<SwDockWidget_Splitter*>(parentWidget);
					}

					//Recuperation de la position du splitter dans le parent
					int row = 0 , column = 0 , rowSpan = 0 , columnSpan = 0 , num = 0 ;
					if (glayout)
						glayout->getItemPosition(glayout->indexOf(splitter), &row, &column, &rowSpan, &columnSpan);
					else if (vlayout)
						row = vlayout->indexOf(splitter);
					else if (hlayout)
						column = hlayout->indexOf(splitter);
					else if (parentSplitter)
						num = parentSplitter->indexOf(splitter);
					
					if (toMain)
					{
						//On supprime le splitter, et on remet le widget qui n'est pas le dock dans le parent
						if (glayout)
						{
							glayout->removeWidget(splitter);
							glayout->addWidget(toMain, row, column, rowSpan, columnSpan);
						}
						else if (vlayout)
						{
							vlayout->removeWidget(splitter);
							vlayout->insertWidget(row, toMain);
						}
						else if (hlayout)
						{
							hlayout->removeWidget(splitter);
							hlayout->insertWidget(column, toMain);
						}
						else if (parentSplitter)
						{
							splitter->setParent(0);
							parentSplitter->replaceWidget(toMain, num);
						}
					}

					_isReleasing = false;
				}
			}
			//Sinon, liberation d'un dock
			else
			{
				_isReleasing = true;

				//Recuperation de la position du dock
				QPoint pos = dock->mapToGlobal(QPoint(0,0));
				QSize dockSize = dock->size();

				//Liberation
				dock->returnToMainArea();

				//Mise a jour de la position et de la taille du dock
				dock->move(pos);
				dock->restoreSize();
				dock->show();

				_isReleasing = false;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::updateMainDock()
{
	_leftMainDock->updateContents();
	_rightMainDock->updateContents();
	_topMainDock->updateContents();
	_bottomMainDock->updateContents();
	_secondScreenMainDock->updateContents();

	//Mise a jour de la configuration des docks principaux
	updateConfiguration();
}

//-----------------------------------------------------------------------------
// Gestion des fleches de positionnement
//-----------------------------------------------------------------------------
//Creation des boutons de positionnement
void SwDockWidget_MainArea::createArrowsButton()
{
	//Positionnement sur les cotes du widget central
	//Fleche en haut
	addButton(&_topCentralBtn, BTN_SIZE, QIcon(":/DockWidget/topMain"));
	//Fleche a droite
	addButton(&_rightCentralBtn, BTN_SIZE, QIcon(":/DockWidget/rightMain"));
	//Fleche a gauche
	addButton(&_leftCentralBtn, BTN_SIZE, QIcon(":/DockWidget/leftMain"));
	//Fleche en bas
	addButton(&_bottomCentralBtn, BTN_SIZE, QIcon(":/DockWidget/bottomMain"));

	//Positionnement sur les bords du dock actif
	//Center
	//addButton(&_centerWidgetBtn, BTN_CENTER_SIZE, QIcon(":/DockWidget/center"));
	//Fleche en haut
	addButton(&_topWidgetBtn, BTN_SIZE, QIcon(":/DockWidget/top"));
	//Fleche a droite
	addButton(&_rightWidgetBtn, BTN_SIZE, QIcon(":/DockWidget/right"));
	//Fleche a gauche
	addButton(&_leftWidgetBtn, BTN_SIZE, QIcon(":/DockWidget/left"));
	//Fleche en bas
	addButton(&_bottomWidgetBtn, BTN_SIZE, QIcon(":/DockWidget/bottom"));

	//Positionnement en onglet
	addButton(&_tabBtn, BTN_SIZE, QIcon(":/DockWidget/images/DockWidget/tab.png"));

	//Positionnement dans un onglet vide de dock principal
	addButton(&_centerDockBtn, BTN_SIZE, QIcon(":/DockWidget/emptyMainDock"));

	//Positionnement sur le deuxieme ecran
	addButton(&_secondScreenBtn, BTN_CENTER_SIZE, QIcon(":/DockWidget/images/DockWidget/secondScreen.png"));
}

//-----------------------------------------------------------------------------
//Creation d'un bouton de positionnement
void SwDockWidget_MainArea::addButton(QPushButton ** button, int size, QIcon icon)
{
	(*button) = new QPushButton(/*this*/); //!Si parent == this, pas d'apparition de fenetres dans la barre des taches pour chaque fleche, mais mouvement saccade
										   //!Si parent == 0   , deplacement plus fluide, mais chaque fleche apparait comme une application dans la barre des taches windows
	(*button)->setFlat(true);
	(*button)->setFixedSize(size,size);
	(*button)->setIcon(icon);
	(*button)->setIconSize(QSize(size, size));
	(*button)->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	(*button)->setAttribute(Qt::WA_TransparentForMouseEvents);
}

//-----------------------------------------------------------------------------
//Affichage des boutons de positionnement
void SwDockWidget_MainArea::showArrows(QWidget * widget)
{
	if (widget)
	{
		//Si positionnement dans un widget, affichage des fleches au centre du widget
		if (widget != _mainWidget)
		{
			QPoint p = widget->mapToGlobal(QPoint(0,0));

			//Positionnement des boutons en fonction de la taille du widget
			int centerX = p.x() + ( widget->width()  / 2);
			int centerY = p.y() + ( widget->height() / 2);
			
			//_centerWidgetBtn->move(QPoint(centerX - BTN_CENTER_SIZE / 2, centerY - BTN_CENTER_SIZE / 2));
			_topWidgetBtn->move( QPoint(centerX - (BTN_SIZE / 2), centerY - BTN_SIZE - PADDING - BTN_SIZE / 2));
			_rightWidgetBtn->move(QPoint(centerX + (BTN_SIZE / 2) + PADDING, centerY - BTN_SIZE / 2));
			_bottomWidgetBtn->move(QPoint(centerX - (BTN_SIZE / 2), centerY + BTN_SIZE + PADDING - BTN_SIZE / 2));
			_leftWidgetBtn->move(QPoint(centerX - BTN_SIZE - (BTN_SIZE / 2) - PADDING, centerY - BTN_SIZE / 2));

			_tabBtn->move(QPoint(centerX - BTN_SIZE / 2, centerY - BTN_SIZE / 2));

			//Affichage
			//_centerWidgetBtn->show();
			_topWidgetBtn->show();
			_rightWidgetBtn->show();
			_leftWidgetBtn->show();
			_bottomWidgetBtn->show();
			_tabBtn->show();

			_topCentralBtn->hide();
			_rightCentralBtn->hide();
			_bottomCentralBtn->hide();
			_leftCentralBtn->hide();
		}

		//Si positionnement sur le widget central, affichage des fleches sur les bords
		else
		{
			QPoint p = widget->mapToGlobal(QPoint(0,0));

			//Positionnement des boutons en fonction de la taille du widget
			int centerX = p.x() + ( widget->width()  / 2);
			int centerY = p.y() + ( widget->height() / 2);

			_topCentralBtn->move( QPoint(centerX - (BTN_SIZE / 2), p.y() + PADDING));
			_rightCentralBtn->move(QPoint(p.x() + widget->width() - BTN_SIZE - PADDING, centerY - BTN_SIZE / 2));
			_bottomCentralBtn->move(QPoint(centerX - (BTN_SIZE / 2), p.y() + widget->height() - BTN_SIZE - PADDING));
			_leftCentralBtn->move(QPoint(p.x() + PADDING, centerY - BTN_SIZE / 2));

			//Affichage des fleches lorsque le dock principal correspondant n'est pas affiche
			if (_topMainDock->empty())
				_topCentralBtn->show();
			if (_rightMainDock->empty())
				_rightCentralBtn->show();
			if (_bottomMainDock->empty())
				_bottomCentralBtn->show();
			if (_leftMainDock->empty())
				_leftCentralBtn->show();

			//_centerWidgetBtn->hide();
			_topWidgetBtn->hide();
			_rightWidgetBtn->hide();
			_bottomWidgetBtn->hide();
			_leftWidgetBtn->hide();
			_tabBtn->hide();
		}

		_centerDockBtn->hide();
		_secondScreenBtn->hide();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::showArrowsOnEmptyMainDock(QWidget * widget)
{
	if (widget)
	{
		QPoint p = widget->mapToGlobal(QPoint(0,0));

		//Positionnement des boutons en fonction de la taille du widget
		int centerX = p.x() + ( widget->width()  / 2);
		int centerY = p.y() + ( widget->height() / 2);

		_centerDockBtn->move(QPoint(centerX - BTN_SIZE / 2, centerY - BTN_SIZE / 2));
		_centerDockBtn->show();

		//On masque les autres fleches
		//_centerWidgetBtn->hide();
		_topWidgetBtn->hide();
		_rightWidgetBtn->hide();
		_leftWidgetBtn->hide();
		_bottomWidgetBtn->hide();

		_topCentralBtn->hide();
		_rightCentralBtn->hide();
		_leftCentralBtn->hide();
		_bottomCentralBtn->hide();

		_tabBtn->hide();

		_secondScreenBtn->hide();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::showArrowsOnSecondScreen(QRect rect)
{
	_secondScreenBtn->move(QPoint(rect.center().x() - BTN_CENTER_SIZE / 2, rect.center().y() - BTN_CENTER_SIZE / 2));
	_secondScreenBtn->show();

	//On masque les autres fleches
	//_centerWidgetBtn->hide();
	_topWidgetBtn->hide();
	_rightWidgetBtn->hide();
	_leftWidgetBtn->hide();
	_bottomWidgetBtn->hide();

	_topCentralBtn->hide();
	_rightCentralBtn->hide();
	_leftCentralBtn->hide();
	_bottomCentralBtn->hide();

	_tabBtn->hide();

	_centerDockBtn->hide();
}

//-----------------------------------------------------------------------------
//Masquage des boutons de positionnement
void SwDockWidget_MainArea::hideArrows(bool withOverlay)
{
	//_centerWidgetBtn->hide();
	_topWidgetBtn->hide();
	_rightWidgetBtn->hide();
	_leftWidgetBtn->hide();
	_bottomWidgetBtn->hide();

	_topCentralBtn->hide();
	_rightCentralBtn->hide();
	_leftCentralBtn->hide();
	_bottomCentralBtn->hide();

	_tabBtn->hide();

	_secondScreenBtn->hide();

	_centerDockBtn->hide();

	if (withOverlay)
		hideOverlay();
}

//-----------------------------------------------------------------------------
//Gestion des fleches
void SwDockWidget_MainArea::manageArrows(QObject * obj)
{
	if (_isMovingDock && _mainWidget)
	{
		//Recuperation du widget sur lequel afficher les fleches
		QWidget * widget = getDockableWidget();
		if (widget)
		{
			SwDockWidget_MainDockWidget * toMainDock = qobject_cast<SwDockWidget_MainDockWidget*>(widget);
			SwDockWidget_MainTabWidget * toMainTabBar = qobject_cast<SwDockWidget_MainTabWidget*>(widget);
			//Si deplacement dans un autre dock
			if (!toMainDock && !toMainTabBar)
			{
				//Affichage des fleches de positionnement sur la zone survolee
				showArrows(widget);
				//Parametrage de l'ombre de positionnement
				setupOverlay(obj, widget);
				//Mise en surbrillance du bouton survole
				highlightArrows(QCursor::pos(), widget == _mainWidget);
				return;
			}
			//Si deplacement dans un onglet vide d'un main dock
			else if (toMainDock && toMainDock->isCurrentEmpty())
			{
				//Affichage d'un symbole de positionnement centrale
				showArrowsOnEmptyMainDock(toMainDock);
				//Parametrage de l'ombre de positionnement
				setupEmptyMainDockOverlay(toMainDock);
				//Mise en surbrillance du bouton survole
				highlightArrows(QCursor::pos(), false);
				return;
			}
			//Si deplacement dans un nouvel onglet d'un main dock 
			else if (toMainTabBar)
			{
				//Recherche du main dock parent
				QWidget * parent = toMainTabBar->parentWidget();
				toMainDock = qobject_cast<SwDockWidget_MainDockWidget*>(parent);
				int itr = 0; //Protection!
				while(!toMainDock && itr < 20)
				{
					parent = (parent->parentWidget());
					toMainDock = qobject_cast<SwDockWidget_MainDockWidget*>(parent);
					itr++;
				}
				if (toMainDock)
				{
					hideArrows();
					//Parametrage de l'ombre de positionnement
					setupEmptyMainDockOverlay(toMainDock);
					//Affichage de l'ombre de positionnement
					_overlay->show();
					return;
				}
			}
		}
		//Si le pointeur est dans le deuxieme ecran
		int appScreen = QApplication::desktop()->screenNumber(mapToGlobal(this->rect().center()));
		int currentScreen = QApplication::desktop()->screenNumber(QCursor::pos());
		if (appScreen != currentScreen)
		{
			SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
			//Si le dock du deuxieme ecran est vide ou si le deplacement concerne un onglet
			if (_secondScreenMainDock->empty() || mainDock)
			{
				QRect rect = QApplication::desktop()->availableGeometry(QCursor::pos());
				//Affichage d'un symbole de positionnement centrale
				showArrowsOnSecondScreen(rect);
				//Mise en surbrillance du bouton survole
				highlightArrows(QCursor::pos(), false);
				return;
			}
		}
	}
	hideArrows();
}

//-----------------------------------------------------------------------------
//Gestion du hover (mise en surbrillance du bouton survole)
void SwDockWidget_MainArea::highlightArrows(QPoint pos, bool toMain)
{
	hideOverlay();

	//----------------
	//Bord du widget central
	if(_topCentralBtn->isVisible() && QRect(_topCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_topCentralBtn->setIcon(QIcon(":/DockWidget/topMainHover"));
		showOverlay(Qt::TopDockWidgetArea, toMain);
		return;
	}
	else
		_topCentralBtn->setIcon(QIcon(":/DockWidget/topMain"));

	if(_bottomCentralBtn->isVisible() && QRect(_bottomCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_bottomCentralBtn->setIcon(QIcon(":/DockWidget/bottomMainHover"));
		showOverlay(Qt::BottomDockWidgetArea, toMain);
		return;
	}
	else
		_bottomCentralBtn->setIcon(QIcon(":/DockWidget/bottomMain"));

	if(_leftCentralBtn->isVisible() && QRect(_leftCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_leftCentralBtn->setIcon(QIcon(":/DockWidget/leftMainHover"));
		showOverlay(Qt::LeftDockWidgetArea, toMain);
		return;
	}
	else
		_leftCentralBtn->setIcon(QIcon(":/DockWidget/leftMain"));

	if(_rightCentralBtn->isVisible() && QRect(_rightCentralBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_rightCentralBtn->setIcon(QIcon(":/DockWidget/rightMainHover"));
		showOverlay(Qt::RightDockWidgetArea, toMain);
		return;
	}
	else
		_rightCentralBtn->setIcon(QIcon(":/DockWidget/rightMain"));

	//----------------
	//Bord du widget
	if(_topWidgetBtn->isVisible() && QRect(_topWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_topWidgetBtn->setIcon(QIcon(":/DockWidget/topHover"));
		showOverlay(Qt::TopDockWidgetArea, toMain);
		return;
	}
	else
		_topWidgetBtn->setIcon(QIcon(":/DockWidget/top"));

	if(_bottomWidgetBtn->isVisible() && QRect(_bottomWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_bottomWidgetBtn->setIcon(QIcon(":/DockWidget/bottomHover"));
		showOverlay(Qt::BottomDockWidgetArea, toMain);
		return;
	}
	else
		_bottomWidgetBtn->setIcon(QIcon(":/DockWidget/bottom"));

	if(_leftWidgetBtn->isVisible() && QRect(_leftWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_leftWidgetBtn->setIcon(QIcon(":/DockWidget/leftHover"));
		showOverlay(Qt::LeftDockWidgetArea, toMain);
		return;
	}
	else
		_leftWidgetBtn->setIcon(QIcon(":/DockWidget/left"));

	if(_rightWidgetBtn->isVisible() && QRect(_rightWidgetBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_rightWidgetBtn->setIcon(QIcon(":/DockWidget/rightHover"));
		showOverlay(Qt::RightDockWidgetArea, toMain);
		return;
	}
	else
		_rightWidgetBtn->setIcon(QIcon(":/DockWidget/right"));

	//Onglet
	if(_tabBtn->isVisible() && QRect(_tabBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_tabBtn->setIcon(QIcon(":/DockWidget/images/DockWidget/tabHover.png"));
		showOverlay(Qt::AllDockWidgetAreas);
	}
	else
		_tabBtn->setIcon(QIcon(":/DockWidget/images/DockWidget/tab.png"));

	//Onglet principal
	if(_centerDockBtn->isVisible() && QRect(_centerDockBtn->pos(),QSize(BTN_SIZE,BTN_SIZE)).contains(pos))
	{
		_centerDockBtn->setIcon(QIcon(":/DockWidget/emptyMainDockHover"));
		_overlay->show();
	}
	else
		_centerDockBtn->setIcon(QIcon(":/DockWidget/emptyMainDock"));

	//Deuxieme ecran
	if(_secondScreenBtn->isVisible() && QRect(_secondScreenBtn->pos(),QSize(BTN_CENTER_SIZE,BTN_CENTER_SIZE)).contains(pos))
		_secondScreenBtn->setIcon(QIcon(":/DockWidget/images/DockWidget/secondScreenHover.png"));
	else
		_secondScreenBtn->setIcon(QIcon(":/DockWidget/images/DockWidget/secondScreen.png"));
}

//-----------------------------------------------------------------------------
// Gestion des aires de prepositionnement
//-----------------------------------------------------------------------------
//Parametrage de l'aire de prepositionnement (Overlay)
void SwDockWidget_MainArea::setupOverlay(QObject * obj, QWidget * widget)
{
	if (!obj || !widget)
		return;

	QWidget * dock = qobject_cast<QWidget *>(obj);
	if (!widget)
		return;

	//Enregistrement des deux elements : SwDockWidget_DockWidget a positionner, et widget sur lequel positionner
	_overlay->setWidgets(widget, dock);
}

//-----------------------------------------------------------------------------
//Parametrage de l'aire de prepositionnement dans un onglet vide d'un dock principal
void SwDockWidget_MainArea::setupEmptyMainDockOverlay(QWidget * widget)
{
	SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(widget);
	if (mainDock)
	{
		QWidget * emptyWidget = mainDock->getWidget(mainDock->currentIndex());
		QRect rect = emptyWidget->rect();
		QSize osize(rect.size());
		QPoint opos(0,0);
		SwDockWidget_MainTabWidget * tab = qobject_cast<SwDockWidget_MainTabWidget * >(mainDock->getTabWidget());
		if (tab)
			opos = QPoint(0, tab->getTabBar()->height());

		_overlay->setColor(CENTRAL_WIDGET_OVERLAY_COLOR);
		_overlay->setParent(widget);
		_overlay->resize(osize);
		_overlay->move(opos);
	}
}

//-----------------------------------------------------------------------------
//Affichage de l'aire de prepositionnement
//toMain : si affichage sur la fenetre a la place du widget
void SwDockWidget_MainArea::showOverlay(Qt::DockWidgetArea area, bool toMain)
{
	//Si affichage sur l'aire principal, configuration en fonction des docks principaux
	if (toMain)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(_overlay->getDockWidget());
		if (!dock)
			return;

		QSize osize(dock->getRawSize());
		QPoint opos(0,0);

		switch(area)
		{
			case Qt::LeftDockWidgetArea:
				//Si le dock principal gauche est affiche, l'overlay est sur le dock
				if (_leftMainDock->isVisible())
				{
					osize = _leftMainDock->size();
					opos = getPosition(_leftMainDock);
				}
				//Sinon il prend tout le cote gauche
				else
				{
					//Hauteur max
					osize.setHeight(ui.topwidget->height());
					//Largeur fonction de la place disponible
					if (ui.mainLayout->itemAt(0)->widget()->width() - dock->width() < ui.mainLayout->itemAt(0)->widget()->minimumSizeHint().width())
						osize.setWidth(ui.mainLayout->itemAt(0)->widget()->width()/2);
				}
				break;

			case Qt::RightDockWidgetArea:
				//Si le dock principal droit est affiche, l'overlay est sur le dock
				if (_rightMainDock->isVisible())
				{
					osize = _rightMainDock->size();
					opos = getPosition(_rightMainDock);
				}
				//Sinon il prend tout le cote droit
				else
				{
					//Hauteur max
					osize.setHeight(ui.topwidget->height());
					//Largeur fonction de la place disponible
					if (ui.mainLayout->itemAt(0)->widget()->width() - dock->width() < ui.mainLayout->itemAt(0)->widget()->minimumSizeHint().width())
						osize.setWidth(ui.mainLayout->itemAt(0)->widget()->width()/2);
					opos.setX(ui.topwidget->width() - osize.width());
				}
				break;

			case Qt::TopDockWidgetArea:
				//Si le dock principal haut est affiche, l'overlay est sur le dock
				if (_topMainDock->isVisible())
				{
					osize = _topMainDock->size();
					opos = getPosition(_topMainDock);
				}
				//Sinon il prend tout le haut
				else
				{
					//Largeur max
					osize.setWidth(ui.topwidget->width());
					//Hauteur fonction de la place disponible
					if (ui.mainLayout->itemAt(0)->widget()->height() - dock->height() < ui.mainLayout->itemAt(0)->widget()->minimumSizeHint().height())
						osize.setHeight(ui.mainLayout->itemAt(0)->widget()->height()/2);
				}
				break;

			case Qt::BottomDockWidgetArea:
				//Si le dock principal bas est affiche, l'overlay est sur le dock
				if (_bottomMainDock->isVisible())
				{
					osize = _bottomMainDock->size();
					opos = getPosition(_bottomMainDock);
				}
				//Sinon il prend tout le bas
				else
				{
					//Largeur max
					osize.setWidth(ui.topwidget->width());
					//Hauteur fonction de la place disponible
					if (ui.mainLayout->itemAt(0)->widget()->height() - dock->height() < ui.mainLayout->itemAt(0)->widget()->minimumSizeHint().height())
						osize.setHeight(ui.mainLayout->itemAt(0)->widget()->height()/2);
					opos.setY(ui.topwidget->height() - osize.height());
				}
				break;
		}
		
		//Affichage
		_overlay->setColor(CENTRAL_WIDGET_OVERLAY_COLOR);
		_overlay->setParent(0);
		_overlay->setParent(ui.topwidget);
		_overlay->resize(osize);
		_overlay->move(opos);
		_overlay->show();
	}
	//Si affichage sur un dock, configuration en fonction des dimensions (fait dans Overlay)
	else
	{
		_overlay->setColor(WIDGET_OVERLAY_COLOR);
		_overlay->showOn(area, toMain);
	}
}

//-----------------------------------------------------------------------------
//Masquage de l'aire de prepositionnement
void SwDockWidget_MainArea::hideOverlay()
{
	//if (_overlay->isVisible())
		_overlay->hide();
}

//-----------------------------------------------------------------------------
// Fonctions locales
//-----------------------------------------------------------------------------
//Recuperation du dock situe sous le curseur
QWidget * SwDockWidget_MainArea::getDockableWidget()
{
	if (!_mainWidget)
		return NULL;

	//Recuperation du widget situe sous le pointeur (en excluant le dock en cours de deplacement)
	QWidget * widget = qApp->widgetAt(QCursor::pos());
	if (widget)
	{
		//Parcours des parents du widget pour retrouver le dock, ou l'area
		while (widget &&
				(!qobject_cast<SwDockWidget_DockWidget*>(widget) && !qobject_cast<SwDockWidget_MainTabBar*>(widget))&&
				(widget != _mainWidget))
		{
			widget = widget->parentWidget();
		}
	}
	//Si un parent a ete trouve parmi les docks et l'area
	if (widget)
	{
		//Cas particulier : si le curseur survole un onglet principal, on l'active
		SwDockWidget_MainTabBar * tabBar = qobject_cast<SwDockWidget_MainTabBar*>(widget);
		if (tabBar)
		{
			int numTab = tabBar->activeTabUnder(QCursor::pos(), false);
			//Si un onglet a ete trouve, on regarde s'il s'agit de l'onglet d'ajout
			if (numTab >= 0)
			{
				SwDockWidget_MainTabWidget * tabWidget = qobject_cast<SwDockWidget_MainTabWidget*>(tabBar->parentWidget());
				if (tabWidget && tabWidget->widget(numTab))
				{
					if (tabWidget->widget(numTab)->objectName() == "AddTabWidget")
						return tabWidget;
				}
			}
			return NULL;
		}

		//S'il s'agit d'un dock, il ne doit pas etre flottant (donc avoir l'area pour parent)
		if (widget->parentWidget() != this)
			return widget;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//Recuperation de la position d'un widget dans le SwDockWidget_MainArea
//Fonction recursive, on remonte les elements jusqu'a celui de plus haut niveau
QPoint SwDockWidget_MainArea::getPosition(QWidget * widget)
{
	if (widget)
	{
		//Si le widget est celui de plus haut niveau de l'Area, on ne monte pas plus haut
		if (widget == ui.topwidget)
			return widget->pos();

		if (widget->parent())
		{
			//Si parent est un splitter
			SwDockWidget_Splitter * splitter = qobject_cast<SwDockWidget_Splitter*>(widget->parent());
			QStackedWidget * stack = qobject_cast<QStackedWidget*>(widget->parent());
			SwDockWidget_TabWidget * tab = qobject_cast<SwDockWidget_TabWidget*>(widget->parent());
			SwDockWidget_MainDockWidget * secondScreen = qobject_cast<SwDockWidget_MainDockWidget*>(widget->parent());
			if (splitter)
				return ( widget->pos() + getPosition(splitter) );
			//Si le parent est un QStackedWidget (Intermediaire entre le widget et le tabwidget)
			else if (stack)
				return ( getPosition(stack) );
			//Si le parent est un SwDockWidget_TabWidget
			else if (tab)
				return ( widget->pos() + getPosition(tab) );
			//Si le parent est le dock principal du deuxieme ecran
			else if (secondScreen && secondScreen == _secondScreenMainDock)
			{
				QRect rect = QApplication::desktop()->availableGeometry(secondScreen);
				return QPoint(rect.x(), rect.y());
			}
			//Sinon (autre widget), on propage
			return ( widget->pos() + getPosition(qobject_cast<QWidget*>(widget->parent())) );
		}
		return widget->pos();
	}

	return QPoint(0,0);
}

//-----------------------------------------------------------------------------
// Persistence de la configuration des DockWidgets
//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::setConfigurationFileName(QString name, bool apply)
{
	if (_configurationFileName != name)
	{
		//Sauvegarde de la configuration active
		saveAllPositions();
		//Mise a jour du fichier de configuration
		_configurationFileName = name;

		QFile file(_configurationFileName);
		//Si demande de mise a jour, application de la nouvelle configuration
		if (apply && file.exists())
		{
			//Arret de la sauvegarde automatique
			if (_saveTimer && _saveTimer->isActive())
				_saveTimer->stop();

			//Reinitialisation des docks
			for(QObject * obj : _list)
			{
				SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
				if (dock)
				{
					if (dock->isInToolBar())
						releaseFromToolBar(dock);
					else
						releaseDock(dock);
				}
			}
			//Liberation de la configuration actuelle
			if (ui.mainLayout->itemAt(0))
			{
				QWidget * widget = ui.mainLayout->itemAt(0)->widget();
				if (widget)
					widget->setParent(0);
			}
			//Chargement de la nouvelle configuration
			loadDockPosition(_configurationFileName);
			hideArrows(true); //Protection pour masquer les fleches qui peuvent apparaitre pendant le deplacement des fenetres
		}
	}
}

//-----------------------------------------------------------------------------
QString SwDockWidget_MainArea::getConfigurationFileName()
{
	return _configurationFileName;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::setSaveAutoPeriod(unsigned int value)
{
	if (_saveAutoPeriod != value && (value >= SAVE_TIMER_MIN_VALUE || value == 0))
	{
		_saveAutoPeriod = value;
		if (_saveTimer && _saveAutoPeriod)
		{
			if (_saveTimer->isActive())
				_saveTimer->setInterval(_saveAutoPeriod);
			else
				_saveTimer->start(_saveAutoPeriod);
		}
		else if (_saveTimer)
		{
			_saveTimer->stop();
		}
	}
}

//-----------------------------------------------------------------------------
//Sauvegarde de la position des docks
void SwDockWidget_MainArea::saveAllPositions()
{
	//Creation du document de sauvegarde
	QDomDocument doc("dockparameters");
	QDomElement root = doc.createElement("SwDockWidget_MainArea");
	doc.appendChild(root);

	//Aire principale
	if (ui.mainLayout->itemAt(0))
	{
		QWidget * widget = ui.mainLayout->itemAt(0)->widget();
		if (widget)
		{
			savePosition(doc, root, widget);
		}
	}

	//Docks flottants
	for(QObject * obj : _list)
	{
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
		if (dock && dock->parent() == this && !dock->isInToolBar())
		{
			savePosition(doc, root, dock);
		}
	}

	//Dock principal sur le deuxieme ecran
	if (!_secondScreenMainDock->empty())
		savePosition(doc, root, _secondScreenMainDock);

	//ToolBar
	saveToolBar(doc, root);

	//Ecriture
	QFile file(_configurationFileName);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(doc.toByteArray());
		file.close();
	}
}

//-----------------------------------------------------------------------------
//Sauvegarde de la configuration d'un widget et de ses enfants en fonction de son type
void SwDockWidget_MainArea::savePosition(QDomDocument doc, QDomElement dom, QWidget * widget)
{
	if (widget)
	{
		SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(widget);
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		SwDockWidget_Splitter * splitter = qobject_cast<SwDockWidget_Splitter*>(widget);
		SwDockWidget_TabWidget * tab = qobject_cast<SwDockWidget_TabWidget*>(widget);

		//Si le widget est le centralWidget
		if (widget == ui.centralWidget)
		{
			QGridLayout * layout = ui.centralLayout;
			//Sauvegarde du centralWidget
			QDomElement noeud = writeWidgetParameters(doc, dom, widget);
			noeud.setAttribute("locked", QString::number(this->locked()));
			if (layout->count())
			{
				//Sauvegarde du contenu du centralWidget
				savePosition(doc, noeud, layout->itemAt(0)->widget());
			}
		}

		//Si le widget est un dock principal
		else if (mainDock)
		{
			QDomElement mainNoeud = writeWidgetParameters(doc, dom, mainDock);
			//Onglet actif
			mainNoeud.setAttribute("currentTab", QString::number(mainDock->currentIndex()));
			//Dock affiche ou masque
			mainNoeud.setAttribute("visible", QString::number(mainDock->isVisibleTo(qobject_cast<QWidget*>(mainDock->parent()))));
			//Configuration des docks
			mainNoeud.setAttribute("configuration", QString::number(_mainDockConf->getActiveConfiguration()));
			//Sauvegarde du contenu de chacun des onglets
			for (int i = 0; i < mainDock->count(); i++)
			{
				//Parametres generaux
				QDomElement noeud = writeWidgetParameters(doc, mainNoeud, mainDock->getWidget(i));
				noeud.setAttribute("name", mainDock->tabText(i));
				//Contenu de l'onglet
				QWidget * w = mainDock->getWidget(i);
				if (w->layout())
				{
					QLayoutItem * litem = w->layout()->itemAt(0);
					if (litem)
						savePosition(doc, noeud, litem->widget());
				}
				mainNoeud.appendChild(noeud);
			}
			dom.appendChild(mainNoeud);
		}

		//Si le widget est un SwDockWidget_DockWidget
		else if (dock)
		{
			//Sauvegarde du widget
			writeWidgetParameters(doc, dom, dock);
		}

		//Si le widget est un SwDockWidget_Splitter
		else if (splitter)
		{
			//Sauvegarde du splitter
			QDomElement noeud = writeWidgetParameters(doc, dom, splitter);

			//Sauvegarde des deux widgets
			savePosition(doc, noeud, splitter->widget(0));
			savePosition(doc, noeud, splitter->widget(1));
		}

		//Si le widget est un SwDockWidget_TabWidget
		else if (tab)
		{
			//Sauvegarde du SwDockWidget_TabWidget
			QDomElement noeud = writeWidgetParameters(doc, dom, tab);

			//Sauvegarde de tous les onglets
			int num = tab->count();
			for (int i = 0; i < num; i++)
				savePosition(doc, noeud, tab->widget(i));
		}

		//Si le widget est le MainWidget
		else if (widget == _mainWidget)
		{
			//Sauvegarde du mainwidget
			writeWidgetParameters(doc, dom, widget);
		}

		//Sinon, sauvegarde des parametres generaux
		else
		{
			writeWidgetParameters(doc, dom, widget);
		}
	}
}

//-----------------------------------------------------------------------------
//Sauvegarde des parametres d'un widget (position et dimensions)
QDomElement SwDockWidget_MainArea::writeWidgetParameters(QDomDocument doc, QDomElement dom, QWidget * widget)
{
	if (widget)
	{
		//Classe
		QDomElement noeud = doc.createElement(widget->metaObject()->className());
		//Nom
		noeud.setAttribute("objectName", widget->objectName());
		//Position
		QDomElement pos = doc.createElement("pos");
		pos.setAttribute("x", widget->pos().x());
		pos.setAttribute("y", widget->pos().y());
		noeud.appendChild(pos);
		//Dimensions
		QDomElement size = doc.createElement("size");
		size.setAttribute("width", widget->size().width());
		size.setAttribute("height", widget->size().height());
		noeud.appendChild(size);

		//Parametres specifiques
		SwDockWidget_Splitter * splitter = qobject_cast<SwDockWidget_Splitter*>(widget);
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
		QTabWidget * tab = qobject_cast<QTabWidget*>(widget);

		//Si SwDockWidget_Splitter
		if (splitter)
		{
			//Enregistrement de l'orientation et de la position du splitter
			noeud.setAttribute("orientation", QString::number(splitter->orientation()));
		}
		//Si SwDockWidget_DockWidget
		else if (dock)
		{
			//Affichage (pris en compte uniquement pour les docks flottants)
			if (dock->parent() == this && !dock->isInToolBar())
				noeud.setAttribute("visible", widget->isVisible());
			else
				noeud.setAttribute("visible", true);

			//Sauvegarde du cote d'attachement si attache
			noeud.setAttribute("area", QString::number(dock->getArea()));
			//Sauvegarde du nom du dock reduit
			noeud.setAttribute("ToolbarItemName", dock->getToolbarItemName());
			//Enregistrement de la taille du dock
			size.setAttribute("width", dock->getRawSize().width());
			size.setAttribute("height", dock->getRawSize().height());
			noeud.appendChild(size);
			//Enregistrement de la taille du dock lorsqu'il est flottant
			QDomElement freeSize = doc.createElement("freeSize");
			freeSize.setAttribute("width", dock->getFreeSize().width());
			freeSize.setAttribute("height", dock->getFreeSize().height());
			noeud.appendChild(freeSize);
			//Si dock principal, on fusionne taille et "freesize"
			SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(dock);
			if (mainDock)
			{
				size.setAttribute("width", dock->getFreeSize().width());
				size.setAttribute("height", dock->getFreeSize().height());
			}
		}
		//QTabWidget
		else if (tab)
		{
			noeud.setAttribute("currentTab", QString::number(tab->currentIndex()));
		}

		dom.appendChild(noeud);

		return noeud;
	}

	return QDomElement();
}

//-----------------------------------------------------------------------------
//Chargement de la position des docks
void SwDockWidget_MainArea::loadDockPosition(QString filePath)
{
	//Creation du document
	QDomDocument doc("dockparameters");
	
	_configurationFileName = filePath;
	//Lecture
	QFile file(_configurationFileName);
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();

	//Parcours du document
	QDomElement docElem = doc.documentElement();

	//Lecture du premier element, pour le contenu du SwDockWidget_MainArea
	QDomNode n = docElem.firstChild();
	QWidget * widget = readWidgetParameters(n);
	if (widget)
		ui.mainLayout->addWidget(widget);

	//Lecture des elements suivants (docks flottants, toolbar)
	n = n.nextSibling();
	while (!n.isNull())
	{
		readWidgetParameters(n);
		n = n.nextSibling();
	}

	//Affichage du dock principal de deuxieme ecran si necessaire
	if (!_secondScreenMainDock->empty())
	{
		_secondScreenMainDock->show();
		//Positionnement sur le deuxieme ecran
		int appScreen = QApplication::desktop()->screenNumber(mapToGlobal(this->rect().center()));
		QRect rect = QApplication::desktop()->availableGeometry(1-appScreen);
		_secondScreenMainDock->move(QPoint(rect.x(), rect.y()));
		_secondScreenMainDock->resize(rect.size());
	}

	//Mise a jour des docks principaux
	updateMainDock();

	//Apres chargement de la configuration, sauvegarde periodique de la configuration
	if (_saveTimer)
	{
		_saveTimer->stop();
		delete _saveTimer;
	}
	_saveTimer = new QTimer(this);
	connect(_saveTimer, SIGNAL(timeout()), this, SLOT(saveAllPositions()));
	if (_saveAutoPeriod)
		_saveTimer->start(_saveAutoPeriod);
}

//-----------------------------------------------------------------------------
//Lecture d'un noeud
QWidget * SwDockWidget_MainArea::readWidgetParameters(QDomNode node)
{
	if (!node.isNull())
	{
		QDomElement e = node.toElement();
		QString name = e.nodeName();

		//Decodage
		//Si SwDockWidget_MainArea
		if (!name.compare("SwDockWidget_MainArea"))
		{
			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Position
				if (!e.nodeName().compare("pos"))
				{
					QPoint pos;
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
					this->move(pos);
				}
				//Dimensions
				else if (!e.nodeName().compare("size"))
				{
					QSize size;
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
					this->resize(size);
				}
			}
		}
		//Si Dock principal
		if (!name.compare("SwDockWidget_MainDockWidget"))
		{			
			QWidgetList widgetList; //Liste des onglets
			QList<QString> widgetName; //Noms des onglets

			SwDockWidget_MainDockWidget * mainDock = 0;
			QSize size(0,0);

			//Recherche du dock en cours de chargement
			name = e.attribute("objectName");
			for(QObject * obj : _listMainDock)
			{
				SwDockWidget_MainDockWidget * m = qobject_cast<SwDockWidget_MainDockWidget*>(obj);
				if (obj && !obj->objectName().compare(name))
					mainDock = m;
			}

			int index = e.attribute("currentTab").toInt();
			bool visible = e.attribute("visible").toInt();
			if (_mainDockConf)
				_mainDockConf->setActiveConfiguration((SwDockWidget_MainDockConfiguration::ConfigurationIndex)e.attribute("configuration").toInt());

			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Dimensions
				if (!e.nodeName().compare("size"))
				{
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
				}
				//Position
				if (!e.nodeName().compare("pos"))
				{
					QPoint pos;
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
					this->move(pos);
				}
				//MainTabWidget
				else if (!e.nodeName().compare("QWidget"))
				{
					QDomNodeList tabList = e.childNodes();
					for (int j = 0; j < tabList.count(); j++)
					{
						QDomNode tabNode = tabList.item(j);
						//Nom de l'onglet
						name = e.attribute("name");
						//Widgets contenus dans l'onglet
						QWidget * widget = readWidgetParameters(tabNode);
						if (widget)
						{
							widgetList.push_back(widget);
							widgetName.push_back(name);
						}
					}
				}
			}
			//Affichage ou non du dock principal
			mainDock->setVisible(visible);
			//Ajout des widgets charges
			if (!widgetList.isEmpty() && mainDock)
			{
				int num = 0;
				for (QWidget * widget : widgetList)
					mainDock->addWidget(widget, widgetName.at(num++), &size);

				mainDock->setCurrentWidget(index);
			}
			else
				mainDock->setVisible(false);
			
			if (mainDock->getMenuAction())
			{
				mainDock->getMenuAction()->setChecked(visible);
				mainDock->getMenuAction()->setEnabled(!mainDock->empty());
			}
			return mainDock;
		}
		//Si SwDockWidget_DockWidget
		else if (!name.compare("SwDockWidget_DockWidget"))
		{
			//Si le dock existe
			for(QObject * obj : _list)
			{
				SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(obj);
				if (dock && !dock->objectName().compare(e.attribute("objectName")))
				{
					//Aire d'attachement
					Qt::DockWidgetArea area = (Qt::DockWidgetArea)e.attribute("area").toInt();
					dock->setArea(area);

					//Nom du dock reduit
					dock->setToolbarItemName(e.attribute("ToolbarItemName"));
					
					//Affichage
					bool visible = e.attribute("visible").toInt();
					dock->setVisible(visible);

					QSize size(0,0);

					//Lecture des parametres
					QDomNodeList list = node.childNodes();
					for (int i = 0; i < list.count(); i++)
					{
						QDomNode attrNode = list.item(i);
						e = attrNode.toElement();
						//Position
						if (!e.nodeName().compare("pos"))
						{
							QPoint pos;
							pos.setX(e.attribute("x").toInt());
							pos.setY(e.attribute("y").toInt());
							dock->move(pos);
						}
						//Dimensions
						else if (!e.nodeName().compare("size"))
						{
							size.setWidth(e.attribute("width").toInt());
							size.setHeight(e.attribute("height").toInt());
							dock->setRawSize(size);
						}
						//Dimensions du dock flottant
						else if (!e.nodeName().compare("freeSize"))
						{
							QSize freeSize;
							freeSize.setWidth(e.attribute("width").toInt());
							freeSize.setHeight(e.attribute("height").toInt());
							dock->setFreeSize(freeSize);
						}
					}
					return dock;
				}
			}
			//Si le SwDockWidget_DockWidget n'existe pas
			return NULL;
		}
		//Si SwDockWidget_Splitter
		else if (!name.compare("SwDockWidget_Splitter"))
		{
			//Creation du SwDockWidget_Splitter
			Qt::Orientation o = (Qt::Orientation)e.attribute("orientation").toInt();
			SwDockWidget_Splitter * splitter = new SwDockWidget_Splitter(o);

			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			int numWidget = 0;
			QWidget * firstWidget = NULL;
			QWidget * secondWidget = NULL;
			QPoint pos;
			QSize size;
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Position
				if (!e.nodeName().compare("pos"))
				{
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
				}
				//Dimensions
				else if (!e.nodeName().compare("size"))
				{
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
				}
				//Widget
				else
				{
					if (numWidget == 0)
						firstWidget = readWidgetParameters(attrNode);
					else
						secondWidget = readWidgetParameters(attrNode);
					numWidget++;
				}
			}
			//Si les deux widgets existent, on cree le splitter
			if (firstWidget && secondWidget)
			{
				//Dimensions
				QSize size1, size2;

				//Premier widget
				SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(firstWidget);
				if (dock)
				{
					size1 = dock->getRawSize();
					splitter->addWidget(dock, 0, dock->getArea());
				}
				else
				{
					size1 = firstWidget->size();
					splitter->addWidget(firstWidget, 0);
				}
				//Deuxieme widget
				dock = qobject_cast<SwDockWidget_DockWidget*>(secondWidget);
				if (dock)
				{
					size2 = dock->getRawSize();
					splitter->addWidget(dock, 1, dock->getArea());
				}
				else
				{
					size2 = secondWidget->size();
					splitter->addWidget(secondWidget, 1);
				}

				//Dimensions des elements
				splitter->updateSizes(size1, size2);
				
				//Taille et position du splitter
				splitter->move(pos);
				splitter->resize(size);
			}
			//Sinon on renvoi celui qui existe, s'il en existe un
			else
			{
				if (firstWidget)
					return firstWidget;
				else if (secondWidget)
					return secondWidget;
				else
					return NULL;
			}
			return splitter;
		}
		//Si SwDockWidget_TabWidget
		else if (!name.compare("SwDockWidget_TabWidget"))
		{
			SwDockWidget_TabWidget * tab = new SwDockWidget_TabWidget();

			int index = e.attribute("currentTab").toInt();

			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			QWidgetList widgetList;
			QPoint pos;
			QSize size;
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Position
				if (!e.nodeName().compare("pos"))
				{
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
				}
				//Dimensions
				else if (!e.nodeName().compare("size"))
				{
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
				}
				//Widget
				else if (!e.nodeName().compare("SwDockWidget_DockWidget"))
				{
					QWidget * widget = readWidgetParameters(attrNode);
					if (widget)
						widgetList.push_back(widget);
				}
			}
			//Si au moins 2 widgets ont ete trouves, on cree un SwDockWidget_TabWidget
			if (widgetList.count() > 1)
			{
				for (QWidget * widget : widgetList)
					tab->addTab(widget);

				tab->setCurrentIndex(index);

				//Mise a jour de la position et des dimensions
				tab->move(pos);
				tab->resize(size);

				return tab;
			}
			//Sinon, on renvoi le widget trouve, ou NULL si aucun
			else if (!widgetList.isEmpty())
			{
				return widgetList.first();
			}
			return NULL;
		}
		//Si toolbar
		else if (!name.compare("SwDockWidget_ToolBar"))
		{
			return loadToolBar(node);
		}
		//Si centralWidget
		else if (!name.compare("QWidget") && !e.attribute("objectName").compare("centralWidget"))
		{
			bool locked = e.attribute("locked").toInt();
			if (locked)
			{
				lock();
				_lockAction->setChecked(true);
			}

			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Position
				if (!e.nodeName().compare("pos"))
				{
					QPoint pos;
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
					ui.centralWidget->move(pos);
				}
				//Dimensions
				else if (!e.nodeName().compare("size"))
				{
					QSize size;
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
					ui.centralWidget->resize(size);
				}
				//Autres elements
				else
				{
					QWidget * widget = readWidgetParameters(list.item(i));
					if (widget)
						ui.centralLayout->addWidget(widget);
				}
			}
			return ui.centralWidget;
		}
		//Si MainWidget
		else if (!name.compare("MainWidget") && _mainWidget)
		{
			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();
				//Position
				if (!e.nodeName().compare("pos"))
				{
					QPoint pos;
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
					_mainWidget->move(pos);
				}
				//Dimensions
				else if (!e.nodeName().compare("size"))
				{
					QSize size;
					size.setWidth(e.attribute("width").toInt());
					size.setHeight(e.attribute("height").toInt());
					_mainWidget->resize(size);
				}
			}
			return _mainWidget;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//Verrouillage de la disposition des docks
//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::lock()
{
	if (!locked())
	{
		//Propagation aux docks
		for(QObject * obj : _list)
		{
			SwDockWidget_DockWidget *dockWidget = qobject_cast<SwDockWidget_DockWidget*>(obj);
			if (dockWidget)
				dockWidget->lock();		
		}
		//Propagation aux docks principaux
		for (QObject * obj : _listMainDock)
		{
			SwDockWidget_DockWidget *dockWidget = qobject_cast<SwDockWidget_DockWidget*>(obj);
			if (dockWidget)
				dockWidget->lock();		
		}

		//Verrouillage des menus
		_mainDockConf->setEnabled(false);
		_widgetMenu->setEnabled(false);
	}

	SwDockWidget_ToolBarWindow::lock();
}

//-----------------------------------------------------------------------------
//Deverrouillage de la disposition des docks
void SwDockWidget_MainArea::releaseLock()
{
	if (locked())
	{
		//Propagation aux docks
		for(QObject * obj : _list)
		{
			SwDockWidget_DockWidget *dockWidget = qobject_cast<SwDockWidget_DockWidget*>(obj);
			if (dockWidget)
				dockWidget->releaseLock();		
		}
		//Propagation aux docks principaux
		for (QObject * obj : _listMainDock)
		{
			SwDockWidget_DockWidget *dockWidget = qobject_cast<SwDockWidget_DockWidget*>(obj);
			if (dockWidget)
				dockWidget->releaseLock();		
		}

		//Deverrouillage des menus
		_mainDockConf->setEnabled(true);
		_widgetMenu->setEnabled(true);
	}

	SwDockWidget_ToolBarWindow::releaseLock();
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::setLock(bool state)
{
	if (state)
		lock();
	else
		releaseLock();
}

//-----------------------------------------------------------------------------
//Gestion des docks principaux
//-----------------------------------------------------------------------------
SwDockWidget_MainDockWidget * SwDockWidget_MainArea::setupMainDockWidget(QString name, Qt::DockWidgetArea area, bool withAddButton)
{
	SwDockWidget_MainDockWidget * main = new SwDockWidget_MainDockWidget(this, name, area, withAddButton);
	//connect(main, SIGNAL(moveTabRequested(QPoint)), this, SLOT(moveMainTab(QPoint)));
	//connect(main, SIGNAL(freeTabRequested(int, QPoint)), this, SLOT(freeMainTab(int, QPoint)));
	//connect(main, SIGNAL(stopMovingTabRequested()), this, SLOT(stopMovingMainTab()));
	connect(main, SIGNAL(moveTabRequested(int, Qt::DockWidgetArea)), this, SLOT(moveMainTab(int, Qt::DockWidgetArea)));
	_listMainDock.push_back(main);
	main->installEventFilter(this);
	main->setArea(area);

	return main;
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainArea::moveMainTab(int index, Qt::DockWidgetArea area)
{
	SwDockWidget_MainDockWidget * mainDock = qobject_cast<SwDockWidget_MainDockWidget*>(this->sender());
	if (mainDock)
	{
		QWidget * dock = NULL;
		QWidget * toReturn = NULL;
		QString name = "";
		QSize * size = NULL;
		if (mainDock)
		{
			//Recuperation du contenu de l'onglet
			dock = mainDock->getWidget(index);
			if (dock && dock->layout())
			{
				QLayoutItem * litem = dock->layout()->itemAt(0);
				if (litem)
					dock = litem->widget();
			}
			//Recuperation du nom et de la taille de l'onglet
			name = mainDock->tabText(index);
			size = new QSize(mainDock->getRawSize());
		}

		//Si a ancrer a gauche
		if(area == Qt::LeftDockWidgetArea)
		{
			_leftMainDock->addWidget(dock, name, size);
			if (_leftMainDock->count() == 1)
				toReturn = pinDockTo(_leftMainDock, _mainWidget, Qt::LeftDockWidgetArea, true);
		}

		//Si a ancrer a droite
		else if(area == Qt::RightDockWidgetArea)
		{
			_rightMainDock->addWidget(dock, name, size);
			if (_rightMainDock->count() == 1)
				toReturn = pinDockTo(_rightMainDock, _mainWidget, Qt::RightDockWidgetArea, true);
		}

		//Si a ancrer en haut
		else if(area == Qt::TopDockWidgetArea)
		{
			_topMainDock->addWidget(dock, name, size);
			if (_topMainDock->count() == 1)
				toReturn = pinDockTo(_topMainDock, _mainWidget, Qt::TopDockWidgetArea, true);
		}

		//Si a ancrer en bas
		else if(area == Qt::BottomDockWidgetArea)
		{
			_bottomMainDock->addWidget(dock, name, size);
			if (_bottomMainDock->count() == 1)
				toReturn = pinDockTo(_bottomMainDock, _mainWidget, Qt::BottomDockWidgetArea, true);
		}

		//Si a ancrer dans le deuxieme ecran
		else if (area == Qt::NoDockWidgetArea)
		{
			//Recuperation des informations du deuxieme ecran
			QRect rect = QApplication::desktop()->availableGeometry(1-QApplication::desktop()->screenNumber(mapToGlobal(this->rect().center())));
			//Ajout du widget dans le dock principal de deuxieme ecran
			_secondScreenMainDock->addWidget(dock, name);
			_secondScreenMainDock->resize(rect.size());
			_secondScreenMainDock->move(rect.x(), rect.y());
			_secondScreenMainDock->show();

			toReturn = dock;
		}

		else
			toReturn = NULL;

		//Affichage si necessaire du dock
		if (mainDock && toReturn)
			dock->show();

		//Suppression de l'onglet contenant precedemment les docks
		mainDock->removeWidget(index);
	}
}
