#include "SwDockWidget_DockWidget.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPicture>
#include <QPropertyAnimation>
#include <QDesktopWidget>

#ifdef Q_OS_WIN
#	include "qt_windows.h"
#endif

#include "ui_SwDockWidget_DockWidget.h"

/*
 * Valeurs pour les evenements Windows de redimensionnement
 * 
0xF000 (Center cursor on the form)
0xF001 (Resize from left)
0xF002 (Resize from right)
0xF003 (Resize from up)
0xF004 (Lock the bottom right corner of the form, the up left corner move for resize)
0xF005 (Same from bottom left corner)
0xF006 (Lock up right and left border, resize other) (AAY : Ca semble etre le bottom)
0xF007 (Lock up and right border, resize other border)
0xF008 (Lock left and up border and resize other)
0xF009 (Drag from anywhere)
0xF010 (Put cursor centered at the upper order)
0xF020 (Auto-Minimize Form)
0xF030 (Auto-Maximize Form)
*/

//Enumeration pour les evenements Windows de redimensionnement
#define SZ_SIZELEFT			0xF001
#define SZ_SIZERIGHT		0xF002
#define SZ_SIZETOP			0xF003
#define SZ_SIZEBOTTOM		0xF006
#define SZ_SIZETOPLEFT		0xF004
#define SZ_SIZETOPRIGHT		0xF005
#define SZ_SIZEBOTTOMLEFT	0xF007
#define SZ_SIZEBOTTOMRIGHT  0xF008

#define REDUCED_OPACITY		0.7

//-----------------------------------------------------------------------------
SwDockWidget_DockWidget::SwDockWidget_DockWidget(QWidget *parent)
	: QWidget(parent), _widget(NULL), _emptyWidget(NULL), _tabWidget(NULL), _toolBarItem(NULL), _action(NULL), _mainArea(NULL),
	_inTabWidget(false), _inToolBar(false), _canMove(false), _isMinResizing(Qt::NoDockWidgetArea),
	_canBeClose(true), _canBePin(true), _couldBePin(true), _lock(false), _canBeResize(true), _canBeMoved(true),
	_canBeOutside(true), _isResizing(false), _alwaysOnTop(true), _shownShadow(0),_shadowColor(QColor(109, 183, 255, 120))
{
	ui = new Ui::DockWidget();
	ui->setupUi(this);
	_mainArea = parent;

	ui->contentLayout->removeItem(ui->verticalSpacer); //rendre parametrable?

	this->setVisible(false);

	//Aspect du dock
	//Couleurs
	//setStyleSheet("#L_Title{color: #FFF;} #DockFrame{background:#353535} #DockContent{border:1px solid #AAA;background:#707070;} #scrollArea{background:#707070;}");
	setupShadow(_shadowColor);
	//Suppression du cadre de fenetre windows et affichage du dock au premier plan
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	//Affichage du cadre en transparence
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	//Taille par defaut
	setFreeSize(QSize(0,0));

	//Tooltip
	ui->PB_Close->setToolTip("Close");

	//Gestion des boutons
	setCanBeClose(_canBeClose);
	connect(ui->toToolBarBtn, SIGNAL(clicked()), this, SIGNAL(reduceInToolBarAsked()));
	connect(ui->outToolBarBtn, SIGNAL(clicked()), this, SIGNAL(releaseFromToolBarAsked()));

	//Aspect du pointeur pour le redimensionnement
	setCursorAspect();

	//SwDockWidget_DockWidget independant par defaut
	_area = Qt::NoDockWidgetArea;

	//Boutons masques
	ui->outToolBarBtn->hide();

	//Creation d'un widget vide provisoire
	_emptyWidget = new QWidget(this);
	this->setWidget(_emptyWidget);

	_blinkStatus = 0;
	_timer = 0;
}

//-----------------------------------------------------------------------------
SwDockWidget_DockWidget::~SwDockWidget_DockWidget()
{
	if (ui)
		delete ui;
	if (_widget && _widget != _emptyWidget)
		_widget->setParent(0);
}

//-----------------------------------------------------------------------------
//Enregistrement du contenu du SwDockWidget_DockWidget
void SwDockWidget_DockWidget::setWidget(QWidget * widget)
{
	if (widget)
	{
		//Suppression si necessaire du widget precedent
		if (_widget)
		{
			//ui->contentLayout->removeWidget(_widget);
			_widget->setParent(NULL);
			if (_widget == _emptyWidget)
				delete _emptyWidget;
		}

		//Widget
		ui->contentLayout->insertWidget(0, widget);
		_widget = widget;
		this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		
		//Icon
		this->setWindowIcon(_widget->windowIcon());
		ui->icone->setPixmap(this->windowIcon().pixmap(16));

		//Nom du widget
		setTitle(_widget->windowTitle());

		//Taille
		this->setRawSize(_widget->size() + QSize(4, ui->DockFrame->height() + 4));
	}
	else
	{
		//Creation d'un widget vide provisoire
		_emptyWidget = new QWidget();
		_widget = _emptyWidget;
		ui->contentLayout->insertWidget(0, _widget);
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_DockWidget::getWidget()
{
	return _widget;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setTitle(QString title)
{
	if (_title != title)
	{
		//Mise a jour du nom du SwDockWidget_DockWidget
		_title = title;
		_toolBarItemName = title;
		this->setObjectName(title);
		setTextToLabel(ui->L_Title, _title);
		//Mise a jour du bouton d'affichage du widget
		if (_action)
			_action->setText(_title);
	}
}

//-----------------------------------------------------------------------------
QString SwDockWidget_DockWidget::getTitle()
{
	return _title;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setMainArea(QWidget * area)
{
	if ( (_mainArea != area) || (parent() != area) )
	{
		_mainArea = area;
		setParent(_mainArea);
		setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
		setWindowOpacity(1);
		if (!_freeSize.isNull())
			setRawSize(_freeSize);
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_DockWidget::getMainArea()
{
	return _mainArea;
}

//-----------------------------------------------------------------------------
// Options
//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCanBeMoved(bool state)
{
	if (_canBeMoved != state)
		_canBeMoved = state;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCanBeClose(bool state)
{
	if (_canBeClose != state)
		_canBeClose = state;

	if (_canBeClose)
	{
		ui->PB_Close->show();
		connect(ui->PB_Close, SIGNAL(clicked()), this, SLOT(buttonsSlots()));
	}
	else
	{
		ui->PB_Close->hide();
		disconnect(ui->PB_Close, SIGNAL(clicked()), this, SLOT(buttonsSlots()));
	}
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::canBeClose()
{
	return _canBeClose;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCanBePin(bool state)
{
	if (_canBePin != state)
		_canBePin = state;

	//Liberation du dock si ancre ou minimise
	if (_canBePin == false && this->parent() != _mainArea)
		emit releaseDockAsked();
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::canBePin()
{
	return _canBePin;
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::canBeResize()
{
	return _canBeResize;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCanBeResize(bool state)
{
	if (_canBeResize != state)
		_canBeResize = state;

	if (_canBeResize)
		showShadow();
	else
		hideShadow();
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::canMove()
{
	return _canBeMoved;
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::canBeOutside()
{
	return _canBeOutside;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCanBeOutside(bool state)
{
	if (_canBeOutside != state)
	{
		_canBeOutside = state;
	}
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::isAlwaysOnTop()
{
	return _alwaysOnTop;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setAlwaysOnTop(bool state)
{
	if (_alwaysOnTop != state)
	{
		_alwaysOnTop = state;
		if (_alwaysOnTop)
			setParent(_mainArea);
		else
			setParent(0);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::updateBtn()
{
	if (!_lock)
	{
		ui->PB_Close->show();
		if (parent() == _mainArea)
		{
			ui->toToolBarBtn->setVisible(!isInToolBar());
			ui->outToolBarBtn->setVisible(isInToolBar());
		}
		//Si le dock est attache ou en toolbar
		else
		{
			//Boutons invisibles
			ui->toToolBarBtn->hide();
			ui->outToolBarBtn->hide();
		}
	}
	else
	{
		ui->toToolBarBtn->hide();
		ui->outToolBarBtn->hide();
		ui->PB_Close->hide();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::hideFrame()
{
	ui->DockFrame->hide();
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::showFrame()
{
	ui->DockFrame->show();
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::removeSpacer()
{
	delete ui->verticalSpacer;
}

//-----------------------------------------------------------------------------
// Evenements
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Gestion du clic
void SwDockWidget_DockWidget::mousePressEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton && !_lock)
	{
		//Verification d'une demande de redimensionnement
		if(_canBeResize && manageResize(event))
			return;

		//Verification d'une demande de deplacement (clic sur la barre de titre)
		int border = ui->bottom->isVisible()?getShadowSize():0;
		if(_canBeMoved && (event->y() >= 0 && event->y() <= ui->DockFrame->height() + border ))
		{	
			//Enregistrement de la position, et preparation du deplacement
			this->setInitialMoving(event->pos());
			return;
		}
		else
			_canMove = false;
		this->setFocus(); // On force le focus sur le Dock pour propager l'evenement
	}
	QWidget::mousePressEvent(event);
}

//-----------------------------------------------------------------------------
//Relachement du bouton = fin du clic
void SwDockWidget_DockWidget::mouseReleaseEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton && !_lock)
	{
		_isMinResizing = Qt::NoDockWidgetArea;
		_isResizing = false;
		setMoving(false);
	}
	QWidget::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
//Deplacement de la souris avec clic actif
void SwDockWidget_DockWidget::mouseMoveEvent( QMouseEvent * event )
{
	//Si un deplacement a ete initialise (clic sur la barre de titre)
	if (_canMove && !_lock)
	{
		if ((event->pos() - _clickPos).manhattanLength() > QApplication::startDragDistance())
		{
			move(mapToGlobal(event->pos() - _clickPos));
		}
	}
}

//-----------------------------------------------------------------------------
//Gestion des evenements
bool SwDockWidget_DockWidget::event( QEvent * event )
{
	switch(event->type())
	{
		//Redimensionnement
		case QEvent::Resize:
			setTextToLabel(ui->L_Title, _title);
			//Si dock flottant et non reduit, enregistrement de sa taille
			if (parent() == _mainArea && _isResizing)
				setFreeSize(getRawSize());
			break;

		//Changement de parent = fenetre ancree ou liberee
		case QEvent::ParentChange:
			setTextToLabel(ui->L_Title, _title);
			if (_widget)
			{
				//Liberation du dock
				if(parent() == _mainArea)
				{
					//Affichage de l'ombre de contour de la fenetre
					showShadow();
					updateBtn();
					//Transparent aux evenements souris pour ne pas masquer les zones dockables
					setAttribute(Qt::WA_TransparentForMouseEvents, false);
					//Si dock libere mais masque, mise a jour du bouton d'action
					if (!this->isVisible() && _action)
						_action->setChecked(false);
				}
				//Ancrage du dock
				else
				{
					if (_freeSize.width() == 0 || _freeSize.height() == 0)
						setFreeSize(this->size());
					//Si le dock a un parent, masquage de l'ombre de contour de la fenetre
					hideShadow();
					updateBtn();
					//Sensible aux evenements souris
					setAttribute(Qt::WA_TransparentForMouseEvents, false);
				}
			}
			break;

		//Si masquage d'un dock flottant, mise a jour du bouton d'action
		case QEvent::Hide:
			if (parent() == _mainArea && _action && !isInToolBar())
				_action->setChecked(false);
			break;
	}

	//DEBUG ONLY : affichage du nom de l'event (necessite d'inclure <QMetaEnum> et <QDebug>)
	/*{
		const QMetaObject & mo = QEvent::staticMetaObject;
		QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Type"));
		QString test(me.valueToKey(event->type()));
		static int num = 0;
		qDebug() << num++ << ": " << this->objectName() << " : " << test << "(" << event->type() << ")";
	}*/
	//Fin DEBUG ONLY

	return QWidget::event(event);
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setInitialMoving(QPoint pos)
{
	if (!_lock && !_inToolBar)
	{
		setMoving(true);
		_clickPos = pos;
		show();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setMoving(bool state)
{
	if (state)
	{
		setAttribute(Qt::WA_TransparentForMouseEvents);
		_canMove = true;
		setCursor (Qt::ClosedHandCursor);
		setWindowOpacity(0.6);
	}
	else
	{
		setAttribute(Qt::WA_TransparentForMouseEvents, false);
		_canMove = false;
		setCursor (Qt::ArrowCursor);
		setWindowOpacity(1);
	}
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::isMoving()
{
	return _canMove;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::move(int x, int y)
{
	QWidget::move(getAdjustedPosition(x,y));
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::move(const QPoint & pos)
{
	this->move(pos.x(), pos.y());
}

//-----------------------------------------------------------------------------
//Ajustement de la position pour que le widget reste integralement dans l'ecran
QPoint SwDockWidget_DockWidget::getAdjustedPosition(int x, int y)
{
	QPoint point(x,y);

	//Si le dock peut sortir de l'ecran, on ne modifie pas les coordonnees
	if (!_canBeOutside)
	{
		//Verification de la position du dock pour eviter qu'il ne sorte de l'ecran
		QDesktopWidget desktop;
		QRect rect = desktop.geometry ();
		//Si position en dehors de l'ecran, on s'arrete au bord
		if (x + ui->left->width() < rect.x()) //Bord gauche
			point.setX(rect.x() - ui->left->width());
		else if (x + this->width() - ui->right->width() > rect.x() + rect.width()) //Bord droit
			point.setX(rect.x() + rect.width() - this->width() + ui->right->width());
		if (y + ui->top->height() < rect.y()) //Bord haut
			point.setY(rect.y() - ui->top->height());
		else if (y + this->height() - ui->bottom->height() > rect.y() + rect.height()) //Bord bas
			point.setY(rect.y() + rect.height() - this->height() + ui->bottom->height());
	}

	return point;
}

//-----------------------------------------------------------------------------
//Redimensionnement du dock
bool SwDockWidget_DockWidget::manageResize(QMouseEvent * event)
{
	//Redimensionnement d'un dock flottant
	if(parent() == _mainArea)
	{
		int offset = getShadowSize();
		//Definition des zones de selection - coins
		QRect topLeft = QRect(QPoint(0,0),QSize(offset,offset));
		QRect topLeftExtRight = QRect(QPoint(offset,0),QSize(offset,offset));
		QRect topLeftExtBottom = QRect(QPoint(0,offset),QSize(offset,offset));
		
		QRect topRight = QRect(QPoint(width()-offset,0),QSize(offset,offset));
		QRect topRightExtLeft = QRect(QPoint(width()-offset*2,0),QSize(offset,offset));
		QRect topRightExtBottom = QRect(QPoint(width()-offset,offset),QSize(offset,offset));
		
		QRect bottomLeft = QRect(QPoint(0,height()-offset),QSize(offset,offset));
		QRect bottomLeftExtRight = QRect(QPoint(offset,height()-offset),QSize(offset,offset));
		QRect bottomLeftExtUp = QRect(QPoint(0,height()-offset*2),QSize(offset,offset));
	
		QRect bottomRight = QRect(QPoint(width()-offset,height()-offset),QSize(offset,offset));
		QRect bottomRightExtLeft = QRect(QPoint(width()-offset*2,height()-offset),QSize(offset*2,offset));
		QRect bottomRightExtUp = QRect(QPoint(width()-offset,height()-offset*2),QSize(offset,offset*2));

		//Recherche de l'emplacement du curseur parmis les zones de selection actives - coins
		if (ui->bottomRight->isVisible() &&
			(bottomRight.contains(event->pos()) ||
			bottomRightExtLeft.contains(event->pos()) ||
			bottomRightExtUp.contains(event->pos())))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZEBOTTOMRIGHT, 0);
			#endif
			return true;
		}

		if (ui->topLeft->isVisible() && 
			(topLeft.contains(event->pos()) ||
			topLeftExtRight.contains(event->pos()) ||
			topLeftExtBottom.contains(event->pos())))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZETOPLEFT, 0);
			#endif
			return true;
		}

		if (ui->topRight->isVisible() &&
			(topRight.contains(event->pos()) ||
			topRightExtLeft.contains(event->pos()) ||
			topRightExtBottom.contains(event->pos())))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZETOPRIGHT, 0);
			#endif
			return true;
		}

		if (ui->bottomLeft->isVisible() &&
			(bottomLeft.contains(event->pos()) ||
			bottomLeftExtRight.contains(event->pos()) ||
			bottomLeftExtUp.contains(event->pos())))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZEBOTTOMLEFT, 0);
			#endif
			return true;
		}

		//Definition des zones de selection - cotes
		QRect bottom = QRect(QPoint(offset*2,height()-offset),QSize(width()-offset*2,offset));
		QRect left = QRect(QPoint(0,offset*2),QSize(offset,height()-offset*2));
		QRect right = QRect(QPoint(width() - offset,offset*2),QSize(width()-offset,height() - offset*2));
		QRect top = QRect(QPoint(offset*2,0),QSize(width()-offset*2,offset));

		//Recherche de l'emplacement du curseur parmis les zones de selection - cotes
		if (ui->bottom->isVisible() && bottom.contains(event->pos()))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZEBOTTOM, 0);
			#endif
			return true;
		}

		if (ui->left->isVisible() && left.contains(event->pos()))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZELEFT, 0);
			#endif
			return true;
		}

		if (ui->right->isVisible() && right.contains(event->pos()))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZERIGHT, 0);
			#endif
			return true;
		}

		if (ui->top->isVisible() && top.contains(event->pos()))
		{
			_isResizing = true;
			#ifdef Q_OS_WIN
			ReleaseCapture();
			PostMessage((HWND)this->winId(), WM_SYSCOMMAND, SZ_SIZETOP, 0);
			#endif
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//Mise a jour de la taille apres liberation du dock (pour prise en compte de taille du contour)
void SwDockWidget_DockWidget::undockedSize(const QSize & size)
{
	//QWidget::resize(size);
	setRawSize(size);
	showShadow();
}

//-----------------------------------------------------------------------------
//Mise a jour de la taille du contenu, sans compter l'ombre de contour
void SwDockWidget_DockWidget::setRawSize(const QSize & size)
{
	QSize temp = size;
	int offset = getShadowSize();

	if (ui->top->isVisibleTo(this))
		temp += QSize(0,offset);
	if (ui->bottom->isVisibleTo(this))
		temp += QSize(0,offset);
	if (ui->left->isVisibleTo(this))
		temp += QSize(offset,0);
	if (ui->right->isVisibleTo(this))
		temp += QSize(offset,0);

	QWidget::resize(temp);
}

//-----------------------------------------------------------------------------
//Recuperation de la taille, sans compter l'ombre de contour
QSize SwDockWidget_DockWidget::getRawSize()
{
	QSize size = QWidget::size();
	int offset = getShadowSize();

	if (ui->top->isVisibleTo(this))
		size -= QSize(0,offset);
	if (ui->bottom->isVisibleTo(this))
		size -= QSize(0,offset);
	if (ui->left->isVisibleTo(this))
		size -= QSize(offset,0);
	if (ui->right->isVisibleTo(this))
		size -= QSize(offset,0);

	return size;
}

//-----------------------------------------------------------------------------
//Recuperation de la taille du widget lorsqu'il est flottant
QSize SwDockWidget_DockWidget::getFreeSize()
{
	return _freeSize;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setFreeSize(QSize size)
{
	if (_freeSize != size)
		_freeSize = size;
}

//-----------------------------------------------------------------------------
//Restauration de la taille avant d'etre ancre
void SwDockWidget_DockWidget::restoreSize()
{
	if (_freeSize.width() > 0 && _freeSize.height() > 0)
		setRawSize(_freeSize);

	//Deplacement de la fenetre si le redimensionnement fait sortir le curseur de la fenetre
	if (_clickPos.x() > width())
		_clickPos.setX(width() / 2);
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::isInToolBar()
{
	return _inToolBar;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setInToolBar(bool state, QWidget * toolBarItem)
{
	if (state != _inToolBar)
	{
		//Si le dock est ajoute dans une toolbar
		if (state)
		{
			if (toolBarItem)
				setToolBarItem(toolBarItem);

			//Mise a jour des boutons
			ui->toToolBarBtn->hide();
			ui->outToolBarBtn->setVisible(!_lock);
			ui->PB_Close->hide();

			//Mise a jour du bouton d'action
			if (_action)
				_action->setChecked(true);
		}
		//Si le dock est sorti d'une toolbar
		else
		{
			setToolBarItem(NULL);

			//Mise a jour des boutons
			ui->toToolBarBtn->setVisible(!_lock);
			ui->outToolBarBtn->hide();
			ui->PB_Close->setVisible(!_lock);

			//Mise a jour du contour
			showShadow();

			//Affichage si necessaire du dock
			if (!isVisible())
				show();
		}
		_inToolBar = state;
	}
	else if (toolBarItem)
	{
		setToolBarItem(toolBarItem);
	}
}

//-----------------------------------------------------------------------------
//Mise a jour du texte en fonction de la taille du label
void SwDockWidget_DockWidget::setTextToLabel( QLabel *label, QString text )
{
	QFontMetrics metrix(label->font());
	int width = label->width() - 2;
	//Le texte est tronque s'il ne peut etre affiche en entier
	QString clippedText = metrix.elidedText(text, Qt::ElideRight, width);
	label->setText(clippedText);
}

//-----------------------------------------------------------------------------
//Gestion des boutons de la barre de titre
void SwDockWidget_DockWidget::buttonsSlots()
{
	QPushButton * send = qobject_cast<QPushButton*>(sender());

	//Fermeture du Dock Widget
	if(send == ui->PB_Close)
	{
		close();
	}
}

//-----------------------------------------------------------------------------
//Affichage de l'ombre de contour de la fenetre
void SwDockWidget_DockWidget::showShadow()
{
	QSize size = this->getRawSize();
	_shownShadow = 0; //Reinit

	//Cotes
	ui->top->show();
	ui->bottom->show();
	ui->right->show();
	ui->left->show();

	//Angle haut gauche
	ui->topLeft->show();
	ui->topLeft_ext1->show();
	ui->topLeft_ext2->show();

	//Angle bas gauche
	ui->bottomLeft->show();
	ui->bottomLeft_ext1->show();
	ui->bottomLeft_ext2->show();

	//Angle haut droit
	ui->topRight->show();
	ui->topRight_ext1->show();
	ui->topRight_ext2->show();

	//Angle bas droit
	ui->bottomRight->show();
	ui->bottomRight_ext1->show();
	ui->bottomRight_ext2->show();

	setupShadow(_shadowColor);

	this->setRawSize(size);
}

//-----------------------------------------------------------------------------
//Affichage de l'ombre de contour de la fenetre d'un seul cote
void SwDockWidget_DockWidget::showShadow(int area)
{
	QSize size = this->getRawSize();
	
	//Mise a jour si necessaire
	if (area != _shownShadow)
	{
		//Masquage de toutes les ombres
		hideShadow();

		//Affichage des cotes
		if (area & Qt::LeftDockWidgetArea)
			ui->left->show();
		if (area & Qt::RightDockWidgetArea)
			ui->right->show();
		if (area & Qt::TopDockWidgetArea)
			ui->top->show();
		if (area & Qt::BottomDockWidgetArea)
			ui->bottom->show();

		//Affichages des angles entre deux cotes affiches
		if ((area & Qt::LeftDockWidgetArea) && (area & Qt::TopDockWidgetArea))
			ui->topLeft->show();
		if ((area & Qt::LeftDockWidgetArea) && (area & Qt::BottomDockWidgetArea))
			ui->bottomLeft->show();
		if ((area & Qt::RightDockWidgetArea) && (area & Qt::TopDockWidgetArea))
			ui->topRight->show();
		if ((area & Qt::RightDockWidgetArea) && (area & Qt::BottomDockWidgetArea))
			ui->bottomRight->show();

		//Changement de style si docké
		if((area & Qt::TopDockWidgetArea) && (area & Qt::BottomDockWidgetArea) && (area & Qt::RightDockWidgetArea) && (area & Qt::LeftDockWidgetArea))
			setupShadow(_shadowColor);
		else
			setupShadow(QColor(0,0,0,10)); // transparent

		//Mise a jour de la taille de la fenetre
		this->setRawSize(size);

		_shownShadow = area;
	}
}

//-----------------------------------------------------------------------------
//Masquage de l'ombre de contour de la fenetre
void SwDockWidget_DockWidget::hideShadow()
{
	QSize size = this->getRawSize();
	_shownShadow = 0; //Reinit

	//Cotes
	ui->top->hide();
	ui->bottom->hide();
	ui->right->hide();
	ui->left->hide();

	//Angle haut gauche
	ui->topLeft->hide();
	ui->topLeft_ext1->hide();
	ui->topLeft_ext2->hide();

	//Angle bas gauche
	ui->bottomLeft->hide();
	ui->bottomLeft_ext1->hide();
	ui->bottomLeft_ext2->hide();

	//Angle haut droit
	ui->topRight->hide();
	ui->topRight_ext1->hide();
	ui->topRight_ext2->hide();

	//Angle bas droit
	ui->bottomRight->hide();
	ui->bottomRight_ext1->hide();
	ui->bottomRight_ext2->hide();

	setupShadow(QColor(0,0,0,10)); // transparent

	QWidget::updateGeometry();
	this->setRawSize(size);
}

//-----------------------------------------------------------------------------
//Aspect de l'ombre
void SwDockWidget_DockWidget::setupShadow(QColor color)
{
	QString strcolor = QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());

	ui->top->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->topLeft_ext1->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->topRight_ext2->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");

	ui->right->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->topRight_ext1->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->bottomRight_ext2->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");

	ui->bottom->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->bottomRight_ext1->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->bottomLeft_ext2->setStyleSheet("background:qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");

	ui->left->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->bottomLeft_ext1->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");
	ui->topLeft_ext2->setStyleSheet("background:qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 " + strcolor + ", stop:0.4 rgba(255, 255, 255, 0))");

	ui->topLeft->setStyleSheet("background:qradialgradient(spread:pad, cx:1, cy:1, radius:0.7, fx:1, fy:1, stop:0 " + strcolor + ", stop:0.6 rgba(255, 255, 255, 0))");
	ui->topRight->setStyleSheet("background:qradialgradient(spread:pad, cx:0, cy:1, radius:0.7, fx:0, fy:1, stop:0 " + strcolor + ", stop:0.6 rgba(255, 255, 255, 0))");
	ui->bottomRight->setStyleSheet("background:qradialgradient(spread:pad, cx:0, cy:0, radius:0.7, fx:0, fy:0, stop:0 " + strcolor + ", stop:0.6 rgba(255, 255, 255, 0))");
	ui->bottomLeft->setStyleSheet("background:qradialgradient(spread:pad, cx:1, cy:0, radius:0.7, fx:1, fy:0, stop:0 " + strcolor + ", stop:0.6 rgba(255, 255, 255, 0))");

}


//-----------------------------------------------------------------------------
int SwDockWidget_DockWidget::getShadowSize()
{
	return ui->top->height();
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setCursorAspect()
{
	QCursor verCursor = Qt::SizeVerCursor;
	QCursor horCursor = Qt::SizeHorCursor;
	QCursor fdiagCursor = Qt::SizeFDiagCursor;
	QCursor bdiagCursor = Qt::SizeBDiagCursor;

	//Aspect du pointeur sur le bord de la fenetre
	ui->top->setCursor(verCursor);
	ui->bottom->setCursor(verCursor);
	ui->right->setCursor(horCursor);
	ui->left->setCursor(horCursor);

	//Aspect du pointeur sur les angles de la fenetre
	ui->topLeft->setCursor(fdiagCursor);
	ui->topLeft_ext1->setCursor(fdiagCursor); //Agrandissement de la zone de selection de l'angle
	ui->topLeft_ext2->setCursor(fdiagCursor); //Agrandissement de la zone de selection de l'angle

	ui->bottomLeft->setCursor(bdiagCursor);
	ui->bottomLeft_ext1->setCursor(bdiagCursor); //Agrandissement de la zone de selection de l'angle
	ui->bottomLeft_ext2->setCursor(bdiagCursor); //Agrandissement de la zone de selection de l'angle

	ui->topRight->setCursor(bdiagCursor);
	ui->topRight_ext1->setCursor(bdiagCursor); //Agrandissement de la zone de selection de l'angle
	ui->topRight_ext2->setCursor(bdiagCursor); //Agrandissement de la zone de selection de l'angle

	ui->bottomRight->setCursor(fdiagCursor);
	ui->bottomRight_ext1->setCursor(fdiagCursor); //Agrandissement de la zone de selection de l'angle
	ui->bottomRight_ext2->setCursor(fdiagCursor); //Agrandissement de la zone de selection de l'angle
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setInTab(bool state)
{
	if (state != _inTabWidget)
		_inTabWidget = state;
}

//-----------------------------------------------------------------------------
bool SwDockWidget_DockWidget::isInTab()
{
	return _inTabWidget;
}

//-----------------------------------------------------------------------------
//Mise a jour du cote d'attachement
void SwDockWidget_DockWidget::setArea(Qt::DockWidgetArea area)
{
	if (area != _area)
		_area = area;

	updateBtn();
}

//-----------------------------------------------------------------------------
Qt::DockWidgetArea SwDockWidget_DockWidget::getArea()
{
	return _area;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setTabWidget(QWidget * widget)
{
	_tabWidget = widget;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_DockWidget::getTabWidget()
{
	return _tabWidget;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_DockWidget::getToolBarItem()
{
	return _toolBarItem;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setToolBarItem(QWidget * toolbar)
{
	if (toolbar != _toolBarItem)
	{
		_toolBarItem = toolbar;
	}
}

//-----------------------------------------------------------------------------
QAction * SwDockWidget_DockWidget::getMenuAction()
{
	return _action;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setMenuAction(QAction * action)
{
	if (action != _action)
		_action = action;
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::returnToMainArea()
{
	if (isAlwaysOnTop())
		this->setParent(_mainArea);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setRawSize(_freeSize);
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::lock()
{
	if (!_lock)
	{
		_lock = true;
		updateBtn();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::releaseLock()
{
	if (_lock)
	{
		_lock = false;
		updateBtn();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::blink()
{
	if (this->parent() == _mainArea && !_inToolBar)
	{
		switch(_blinkStatus)
		{
			//Demarrage
			case 0:
				if (_timer == NULL)
				{
					_timer = new QTimer(this);
					connect(_timer, SIGNAL(timeout()), this, SLOT(blink()));
				}
				_timer->start(300);
				_blinkStatus++;
				break;

			//Arret
			case 7: //Doit etre impaire
				_timer->stop();
				_blinkStatus = 0;
				break;

			//Mise a jour
			default:
				if (_blinkStatus%2)
					setupShadow(QColor(255,0,0));
				else
					setupShadow(_shadowColor);
				_blinkStatus++;
				break;
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_DockWidget::setToolbarItemName(QString name)
{
	if (_toolBarItemName != name && !name.isEmpty())
		_toolBarItemName = name;
}

//-----------------------------------------------------------------------------
QString SwDockWidget_DockWidget::getToolbarItemName()
{
	return _toolBarItemName;
}
