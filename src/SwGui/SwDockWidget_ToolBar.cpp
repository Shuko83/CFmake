#include "SwDockWidget_ToolBar.h"

#include "SwDockWidget_DockWidget.h"

#include <QMouseEvent>
#include <QPushButton>
#include <QIcon>
#include <QTimer>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QtCore/qmath.h>

#define	MIN_DISTANCE_TO_STUCK	20
#define OVERLAY_COLOR			QColor(200,100,0,200)

//-----------------------------------------------------------------------------
SwDockWidget_ToolBar::SwDockWidget_ToolBar(QWidget * parent, QRect rect)
: QWidget(parent), _orientation(Qt::Vertical), _layout(NULL), _stuck(Qt::NoSection), _mainRect(rect),
  _lock(false), _canMove(false), _topParent(NULL)
{
	ui.setupUi(this);

	_layout = ui.mainLayout;

	//Aspect
	setMoving(false); //stylesheet defini dans la fonction
	//Suppression du cadre de fenetre windows et affichage au premier plan
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	//Affichage du cadre en transparence
	setAttribute(Qt::WA_TranslucentBackground);

	//Evenements de fermeture de la toolbar
	connect(ui.PB_Close, SIGNAL(clicked()), this, SIGNAL(closeToolBarAsked()));
	//Inversion du sens de la toolbar
	connect(ui.switchBtn, SIGNAL(clicked()), this, SLOT(switchOrientation()));

	getTopParent();
	installEventFilter(this);

	this->show();
}

//-----------------------------------------------------------------------------
SwDockWidget_ToolBar::~SwDockWidget_ToolBar()
{
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::setOrientation(Qt::Orientation orientation)
{
	if (orientation != _orientation)
	{
		switchOrientation();
	}
}

//-----------------------------------------------------------------------------
Qt::Orientation SwDockWidget_ToolBar::getOrientation()
{
	return _orientation;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::switchOrientation()
{
	if (_orientation == Qt::Vertical)
		_orientation = Qt::Horizontal;
	else
		_orientation = Qt::Vertical;

	updateOrientation();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::updateOrientation()
{
	//Liste des widgets presents dans le layout
	QList<QWidget*> list;

	//Recuperation des widgets presents dans le layout
	while(_layout->isEmpty() == false)
	{
		QLayoutItem * item = _layout->takeAt(0);
		QWidget * widg = NULL;
		if (item)
		{
			widg = qobject_cast<QWidget*>(item->widget());
			if (widg)
			{
				list.push_back(widg);
			}
		}
	}

	//Suppression du layout en cours
	delete _layout;

	//Creation d'un nouveau layout
	if (_orientation == Qt::Vertical)
		_layout = new QVBoxLayout(ui.Content);
	else
		_layout = new QHBoxLayout(ui.Content);
	_layout->setContentsMargins(QMargins(0,0,0,0));
	_layout->setSpacing(0);

	//Restauration des widgets
	if (list.isEmpty() == false)
	{
		foreach(QWidget * widg, list)
		{
			_layout->addWidget(widg);
			SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(widg);
			if (item)
				item->setOrientation(_orientation);
		}
	}

	updateSize();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::setMoving(bool state)
{
	if (state)
	{
		//Pendant le deplacement d'un bouton, on masque la barre de titre et le fond pour ne laisser visible que le bouton
		//setStyleSheet("#Frame{background:#353535} #Content{background:transparent;}");
		//setStyleSheet("#Content{background:transparent;}");
		ui.Frame->hide();
	}
	else
	{
		//A la fin du deplacement, on restaure le fond et la barre de titre
		//setStyleSheet("#Frame{background:#353535} #Content{border:1px solid #AAA;background:#707070;}");
		//setStyleSheet("#Content{border:1px solid #AAA;background:#707070;}");
		ui.Frame->show();
	}
}

//-----------------------------------------------------------------------------
//Gestion du clic
void SwDockWidget_ToolBar::mousePressEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton && !_lock)
	{
		//Verification d'une demande de deplacement (clic sur la barre de titre)
		if(event->y() >= 0 && event->y() <= ui.Frame->height())
		{	
			_canMove = true;
			_clickPos = event->pos();
			setCursor(Qt::ClosedHandCursor);
			return;
		}
		else
			_canMove = false;
		setFocus(); // On force le focus pour propager l'evenement
	}
	QWidget::mousePressEvent(event);
}

//-----------------------------------------------------------------------------
//Relachement du bouton = fin du clic
void SwDockWidget_ToolBar::mouseReleaseEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		if (_canMove)
			emit this->stopMoving();
		
		_canMove = false;
		setWindowOpacity(1);

		//Reset du pointeur
		setCursor(Qt::ArrowCursor);
	}
	QWidget::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
//Deplacement de la souris avec clic actif
void SwDockWidget_ToolBar::mouseMoveEvent( QMouseEvent * event )
{
	//Si un deplacement a ete initialise (clic sur la barre de titre)
	if (_canMove && !_lock)
	{
		if ((event->pos() - _clickPos).manhattanLength() > QApplication::startDragDistance())
		{
			setWindowOpacity(0.6);

			//move(mapToGlobal(event->pos() - _clickPos));
			QPoint point = getAdjustedPosition(mapToGlobal(event->pos() - _clickPos));
			point = checkStuckPosition(point.x(), point.y());
			move(point);

			emit this->isMoving();
		}
	}
}

//-----------------------------------------------------------------------------
//Deplacement de la toolbar
void SwDockWidget_ToolBar::moveEvent ( QMoveEvent * event )
{
	//Valeur de deplacement
	QPoint dep = event->pos() - event->oldPos();

	//Deplacement des widgets ouverts
	//Parcours de tous les elements
	for (int i=0; i<_layout->count(); i++)
	{
		QLayoutItem * lItem = _layout->itemAt(i);
		if (lItem)
		{
			//Recuperation de l'item en i-eme position
			SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(lItem->widget());
			if (item)
			{
				//Recuperation du dock associe
				QWidget * widget = item->getWidget();
				if (widget)
				{
					if (widget->isVisible())
					{
						moveItem(widget, item, widget->pos() + dep);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//Evenements generes par un autre widget
bool SwDockWidget_ToolBar::eventFilter( QObject *obj , QEvent * event )
{
	if (obj == this)
	{
		switch(event->type())
		{
			case QEvent::ParentChange:
				setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
				getTopParent();
				break;
		}
	}

	if (obj == _topParent || obj == this->parentWidget())
	{
		switch(event->type())
		{
			case QEvent::Resize:
			case QEvent::Move:
			case QEvent::Paint:
				_mainRect = QRect(QPoint(this->parentWidget()->mapToGlobal(QPoint(0,0))), this->parentWidget()->size());
				updatePosition();
				break;
		}
	}

	SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(obj);
	if (item)
	{
		switch (event->type())
		{
			case QEvent::MouseButtonRelease:
				//Fin du deplacement
				_canMove = false;
				setCursor(Qt::ArrowCursor);

				//On masque tous les autres widgets
				for (int i=0; i<_layout->count(); i++)
				{
					QLayoutItem * layoutItem = _layout->itemAt(i);
					if (layoutItem)
					{
						//Recuperation de l'item en i-eme position
						SwDockWidget_ToolBarItem * iemeItem = qobject_cast<SwDockWidget_ToolBarItem*>(layoutItem->widget());
						if (iemeItem && (iemeItem != item))
						{
							iemeItem->hideWidget(false); //Masquer sans utiliser d'effet
						}
					}
				}
				break;
		}
		return false;
	}

	return QWidget::eventFilter(obj, event);
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::addItem(QWidget * widget)
{
	if (widget)
	{
		_layout->addWidget(widget);
		widget->installEventFilter(this);
		SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(widget);
		if (item)
			item->setOrientation(_orientation);
		
		updateSize();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::addItem(QWidget * widget, QPoint pos)
{
	if (widget)
	{
		Qt::Orientation o = _orientation;

		//Calcul de la position du bouton
		int index = setupButtonPosition(pos, &o);

		//Si changement de layout
		if (_orientation != o)
		{
			_orientation = o;

			updateOrientation();
		}

		//Insertion de l'item
		if (_orientation == Qt::Horizontal)
		{
			QHBoxLayout * hlayout = qobject_cast<QHBoxLayout*>(_layout);
			if (hlayout)
			{
				hlayout->insertWidget(index, widget);
			}
		}
		else // if (_orientation == Qt::Vertical)
		{
			QVBoxLayout * vlayout = qobject_cast<QVBoxLayout*>(_layout);
			if (vlayout)
			{
				vlayout->insertWidget(index, widget);
			}
		}

		widget->installEventFilter(this);
		SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(widget);
		if (item)
			item->setOrientation(_orientation);
		
		updateSize();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::removeItem(QWidget * widget)
{
	if (widget)
	{
		widget->removeEventFilter(this);

		if (_layout->indexOf(widget) >= 0)
			_layout->removeWidget(widget);

		updateSize();
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBar::itemAt(int index)
{
	if (index >= 0 && index < this->count())
	{
		QLayoutItem * lItem = _layout->itemAt(index);
		if (lItem)
			return lItem->widget();
	}

	return NULL;
}

//-----------------------------------------------------------------------------
int SwDockWidget_ToolBar::indexOf(QWidget * widget)
{
	return _layout->indexOf(widget);
}

//-----------------------------------------------------------------------------
bool SwDockWidget_ToolBar::isEmpty()
{
	return _layout->isEmpty();
}

//-----------------------------------------------------------------------------
int SwDockWidget_ToolBar::count()
{
	return _layout->count();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::updateSize()
{
	//Affichage ou non du bouton de changement de sens
	if (_layout->count() > 1)
		ui.switchBtn->show();
	else
		ui.switchBtn->hide();

	//Mise a jour des dimensions
	QTimer::singleShot(10, this, SLOT(fixSize())); //Attente de la prise en compte des modifications avant mise a jour des dimensions
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::fixSize()
{
	QSize size = sizeHint();
    resize(size);
	updatePosition();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::move(int x, int y)
{
	QWidget::move(getAdjustedPosition(x,y));
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::move(const QPoint & pos)
{
	this->move(pos.x(), pos.y());
}

//-----------------------------------------------------------------------------
//Ajustement de la position pour que le widget reste integralement dans l'ecran
QPoint SwDockWidget_ToolBar::getAdjustedPosition(QPoint p)
{
	return getAdjustedPosition(p.x(), p.y());
}

//-----------------------------------------------------------------------------
QPoint SwDockWidget_ToolBar::getAdjustedPosition(int x, int y)
{
	QPoint point(x,y);

	//Verification de la position du dock pour eviter qu'il ne sorte de l'ecran
	QDesktopWidget desktop;
	QRect rect = desktop.geometry ();
	//Si position en dehors de l'ecran, on s'arrete au bord
	if (x < rect.x())
		point.setX(rect.x());
	else if (x + this->width() > rect.x() + rect.width())
		point.setX(rect.x() + rect.width() - this->width());
	if (y < rect.y())
		point.setY(rect.y());
	else if (y + this->height() > rect.y() + rect.height())
		point.setY(rect.y() + rect.height() - this->height());

	return point;
}

//-----------------------------------------------------------------------------
QPoint SwDockWidget_ToolBar::checkStuckPosition(int x, int y)
{
	QPoint point(x, y);
	
	//Si toolbar proche d'un bord du widget principal, on l'aimante
	_stuck = Qt::NoSection;

	//Si toolbar hors de la fenetre, pas de verification
	if (!_mainRect.contains(QPoint(x,y)))
		return point;
	
	//Bord gauche
	if (qAbs(x - _mainRect.x()) < MIN_DISTANCE_TO_STUCK)
	{
		_stuck = Qt::LeftSection;
		point.setX(_mainRect.x());
	}
	//Bord droit
	else if (qAbs(x + width() - _mainRect.x() - _mainRect.width()) < MIN_DISTANCE_TO_STUCK)
	{
		_stuck = Qt::RightSection;
		point.setX(_mainRect.x() + _mainRect.width() - width());
	}

	//Bord haut
	if (qAbs(y - _mainRect.y()) < MIN_DISTANCE_TO_STUCK)
	{
		//Si deja aimante d'un cote, alors en angle
		switch(_stuck)
		{
			//Angle haut gauche
			case Qt::LeftSection:
				_stuck = Qt::TopLeftSection;
				break;

			//Angle haut droit
			case Qt::RightSection:
				_stuck = Qt::TopRightSection;
				break;

			//Sinon, en haut
			default:
				_stuck = Qt::TopSection;
				break;
		}
		point.setY(_mainRect.y());
	}
	//Bord bas
	else if (qAbs(y + height() - _mainRect.y() - _mainRect.height()) < MIN_DISTANCE_TO_STUCK)
	{
		//Si deja aimante d'un cote, alors en angle
		switch(_stuck)
		{
			//Angle bas gauche
			case Qt::LeftSection:
				_stuck = Qt::BottomLeftSection;
				break;

			//Angle bas droit
			case Qt::RightSection:
				_stuck = Qt::BottomRightSection;
				break;

			//Sinon, en bas
			default:
				_stuck = Qt::BottomSection;
				break;
		}
		point.setY(_mainRect.y() + _mainRect.height() - height());
	}

	//Enregistrement de la distance de la toolbar au bord du widget principal (ratio)
	_dist.setX(point.x() - _mainRect.x());
	_dist.setY(point.y() - _mainRect.y());

	return point;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::moveItem(QWidget * widget, SwDockWidget_ToolBarItem * item, QPoint pos)
{
	if (widget && item)
	{
		bool anim = false;
		//Si le widget est deplace en dehors de l'ecran, on le change de cote par rapport au bouton
		QDesktopWidget desktop;
		QRect rect = desktop.geometry();
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);

		//Si position en dehors de l'ecran et que le dock ne doit pas sortir de l'ecran, on s'arrete au bord
		if (dock && !dock->canBeOutside())
		{
			if (pos.x() < rect.x() && pos.x())
			{
				anim = true;
				pos.setX(this->pos().x() + this->width());
				if (dock)
					dock->showShadow(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
			}
			else if (pos.x() + widget->width() > rect.x() + rect.width())
			{
				anim = true;
				pos.setX(this->pos().x() - widget->width());
				if (dock)
					dock->showShadow(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
			}
			if (pos.y() < rect.y())
			{
				anim = true;
				pos.setY(this->pos().y() + item->pos().y());
			}
			else if (pos.y() + widget->height() > rect.y() + rect.height())
			{
				anim = true;
				pos.setY(this->pos().y() + item->pos().y() + item->height() - widget->height());
			}
		}

		//Animation si changement de cote (Pose probleme car continue pendant qu'un autre deplacement est demande)
		/*if (anim)
		{
			QPropertyAnimation * effect = new QPropertyAnimation(widget, "pos");
			effect->setDuration(100);
			effect->setStartValue(widget->pos());
			effect->setEndValue(pos);
			effect->start();
		}
		else*/
			widget->move(pos);
	}
}

//-----------------------------------------------------------------------------
QList<QWidget*> SwDockWidget_ToolBar::getListWidget()
{
	QList<QWidget*> list;

	for (int i=0; i<_layout->count(); i++)
	{
		QLayoutItem * layoutItem = _layout->itemAt(i);
		if (layoutItem)
		{
			//Recuperation de l'item en i-eme position
			SwDockWidget_ToolBarItem * iemeItem = qobject_cast<SwDockWidget_ToolBarItem*>(layoutItem->widget());
			if (iemeItem)
			{
				if (list.contains(iemeItem->getWidget()) == false)
					list.push_back(iemeItem->getWidget());
			}
		}
	}

	return list;
}

//-----------------------------------------------------------------------------
int SwDockWidget_ToolBar::setupButtonPosition(QPoint pos, Qt::Orientation * o)
{
	if (o == NULL)
		return -1;

	int index = -1;
	
	for (int i=0; i<_layout->count(); i++)
	{
		QLayoutItem * layoutItem = _layout->itemAt(i);
		if (layoutItem)
		{
			//Recuperation de l'item en i-eme position
			SwDockWidget_ToolBarItem * btn = qobject_cast<SwDockWidget_ToolBarItem*>(layoutItem->widget());
			if (btn)
			{
				//Limites du bouton en cours
				int left = mapToGlobal(btn->pos()).x();
				int right = mapToGlobal(btn->pos()).x() + btn->width();
				int top = mapToGlobal(btn->pos()).y() + ui.Frame->height();
				int bottom = mapToGlobal(btn->pos()).y() + btn->height() + ui.Frame->height();

				/****
				a ----------- b
				  |			|
				  |    m    |
				  |			|
				d ----------- c
				****/

				QPoint a(left, top);
				QPoint b(right, top);
				QPoint c(right, bottom);
				QPoint d(left, bottom);
				QPoint m((left+right)/2, (top+bottom)/2);

				//Si un seul item dans la toolbar, on propose les 4 cotes
				if (_layout->count() == 1)
				{
					//Si curseur dans la partie haute du bouton (abm)
					if (isInTriangle(a,b,m,pos))
					{
						(*o) = Qt::Vertical;
						index = 0;
						break;
					}
					//Si curseur dans la partie basse du bouton (dmc)
					if (isInTriangle(d,m,c,pos))
					{
						(*o) = Qt::Vertical;
						index = 1;
						break;
					}
					//Si curseur dans la partie gauche du bouton (amd)
					if (isInTriangle(a,m,d,pos))
					{
						(*o) = Qt::Horizontal;
						index = 0;
						break;
					}
					//Si curseur dans la partie droite du bouton (mbc)
					if (isInTriangle(m,b,c,pos))
					{
						(*o) = Qt::Horizontal;
						index = 1;
						break;
					}
				}

				//Sinon, on ne propose que le sens en cours
				else
				{
					if (_orientation == Qt::Vertical)
					{
						//Si dans la partie superieure
						if (pos.y() >= top && pos.y() <= qFloor((top+bottom)/2.0))
						{
							index = _layout->indexOf(btn);
							break;
						}
						//Si dans la partie inferieure
						else if(pos.y() >= qCeil((top+bottom)/2.0) && pos.y() <= bottom)
						{
							index = _layout->indexOf(btn) + 1;
							break;
						}
					}
					else// if (orientation == Qt::Horizontal)
					{
						//Si dans la partie gauche
						if (pos.x() >= left && pos.x() <= qFloor((left+right)/2.0))
						{
							index = _layout->indexOf(btn);
							break;
						}
						//Si dans la partie droite
						else if(pos.x() >= qCeil((left+right)/2.0) && pos.x() <= right)
						{
							index = _layout->indexOf(btn) + 1;
							break;
						}
					}
				}
			}
		}
	}

	return index;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::showOverlay(QPoint pos, SwDockWidget_Overlay * overlay)
{
	if (overlay == NULL)
		return;

	QSize osize;
	QPoint opos;

	int ep = 4;

	Qt::Orientation o = _orientation;
	int index = setupButtonPosition(pos, &o);

	if (index >= 0)
	{
		if (o == Qt::Vertical)
		{
			//Position
			QLayoutItem * item = _layout->itemAt(index);
			QWidget * widg = NULL;
			if (item)
			{
				widg = item->widget();
				if (widg)
				{
					osize.setHeight(ep);
					osize.setWidth(widg->width());
					opos.setX(widg->pos().x());
					opos.setY(widg->pos().y() - ep + ui.Frame->height() + 2);
				}
			}
			else
			{
				item = _layout->itemAt(index-1);
				if (item)
				{
					widg = item->widget();
					if (widg)
					{
						osize.setHeight(ep);
						osize.setWidth(widg->width());
						opos.setX(widg->pos().x());
						opos.setY(widg->pos().y() + widg->height() + ui.Frame->height() - 2);
					}
				}
			}
			overlay->setParent(this);
			overlay->resize(osize);
			overlay->move(opos);
			overlay->show();
		}
		else if (o == Qt::Horizontal)
		{
			//Position
			QLayoutItem * item = _layout->itemAt(index);
			QWidget * widg = NULL;
			if (item)
			{
				widg = item->widget();
				if (widg)
				{
					osize.setHeight(widg->height());
					osize.setWidth(ep);
					opos.setX(widg->pos().x() - 2);
					opos.setY(widg->pos().y() + ui.Frame->height());
				}
			}
			else
			{
				item = _layout->itemAt(index-1);
				if (item)
				{
					widg = item->widget();
					if (widg)
					{
						osize.setHeight(widg->height());
						osize.setWidth(ep);
						opos.setX(widg->pos().x() + widg->width() - 2);
						opos.setY(widg->pos().y() + ui.Frame->height());
					}
				}
			}
			overlay->setParent(this);
			overlay->resize(osize);
			overlay->move(opos);
			overlay->show();
		}
	}
}

//-----------------------------------------------------------------------------
/**
 * @brief	Renvoi vrai si le point p est dans le triangle abc
*/
bool SwDockWidget_ToolBar::isInTriangle(QPoint a, QPoint b, QPoint c, QPoint p)
{
	double area = (b.x()-a.x())*(c.y()-a.y()) - (c.x()-a.x())*(b.y()-a.y());
	double a1 = ((b.x()-p.x())*(c.y()-p.y()) - (c.x()-p.x())*(b.y()-p.y())) / area;
	double a2 = ((c.x()-p.x())*(a.y()-p.y()) - (a.x()-p.x())*(c.y()-p.y())) / area;
	double a3 = ((a.x()-p.x())*(b.y()-p.y()) - (b.x()-p.x())*(a.y()-p.y())) / area;

	if (a1<0 || a2<0 || a3<0)
		return false;
	else
		return true;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::setMainRect(QRect rect)
{
	if (rect != _mainRect)
	{
		_mainRect = rect;
		updatePosition();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::updateMainRect()
{
	QPoint pos = QPoint(this->parentWidget()->mapToGlobal(QPoint(0,0)));
	_mainRect = QRect(pos, this->parentWidget()->size());
	updatePosition();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::updatePosition()
{
	QPoint pos(pos());
	switch(_stuck)
	{
		//Cotes
		case Qt::LeftSection:
			if (_dist.y() + height() > _mainRect.height())
				pos = QPoint(_mainRect.x(), _mainRect.y() + _mainRect.height() - height());
			else
				pos = QPoint(_mainRect.x(), _dist.y() + _mainRect.y());
			break;

		case Qt::TopSection:
			if (_dist.x() + width() > _mainRect.width())
				pos = QPoint(_mainRect.x() + _mainRect.width() - width(), _mainRect.y());
			else
				pos = QPoint(_dist.x() + _mainRect.x(), _mainRect.y());
			break;

		case Qt::RightSection:
			if (_dist.y() + height() > _mainRect.height())
				pos = QPoint(_mainRect.x() + _mainRect.width() - width() , _mainRect.y() + _mainRect.height() - height());
			else
				pos = QPoint(_mainRect.x() + _mainRect.width() - width() , _dist.y() + _mainRect.y());
			break;

		case Qt::BottomSection:
			if (_dist.x() + width() > _mainRect.width())
				pos = QPoint(_mainRect.x() + _mainRect.width() - width(), _mainRect.y() + _mainRect.height() - height());
			else
				pos = QPoint(_dist.x() + _mainRect.x(), _mainRect.y() + _mainRect.height() - height());
			break;

		//Angles
		case Qt::TopLeftSection:
			pos = QPoint(_mainRect.x(), _mainRect.y());
			break;

		case Qt::TopRightSection:
			pos = QPoint(_mainRect.x() + _mainRect.width() - width(), _mainRect.y());
			break;

		case Qt::BottomRightSection:
			pos = QPoint(_mainRect.x() + _mainRect.width() - width(), _mainRect.y() + _mainRect.height() - height());
			break;

		case Qt::BottomLeftSection:
			pos = QPoint(_mainRect.x(), _mainRect.y() + _mainRect.height() - height());
			break;
	}
	
	QWidget::move(pos);
	
	//Mise a jour de la position du widget ouvert si necessaire
	for (int i=0; i<_layout->count(); i++)
	{
		QLayoutItem * lItem = _layout->itemAt(i);
		if (lItem)
		{
			//Recuperation de l'item en i-eme position
			SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(lItem->widget());
			if (item)
			{
				item->updateWidgetPosition();
			}
		}
	}
}

//-----------------------------------------------------------------------------
Qt::WindowFrameSection SwDockWidget_ToolBar::getStuckPosition()
{
	return _stuck;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::setStuckPosition(Qt::WindowFrameSection section)
{
	if (section != _stuck)
	{
		_stuck = section;
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::setDist(QPoint dist)
{
	if (dist != _dist)
		_dist = dist;
}

//-----------------------------------------------------------------------------
QPoint SwDockWidget_ToolBar::getDist()
{
	return _dist;
}

//-----------------------------------------------------------------------------
QSize SwDockWidget_ToolBar::getTitleBarSize()
{
	return ui.Frame->size();
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::lock()
{
	if (!_lock)
	{
		_lock = true;

		//Masquage de la barre de titre
		setMoving(true);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBar::releaseLock()
{
	if (_lock)
	{
		_lock = false;

		//Affichage de la barre de titre
		setMoving(false);
	}
}

//-----------------------------------------------------------------------------
//Recuperation du parent de plus haut niveau
void SwDockWidget_ToolBar::getTopParent()
{
	QWidget * lastParent = NULL;

	if (this->parentWidget())
		this->parentWidget()->installEventFilter(this);

	//Si un top parent existe, on supprime l'ecoute des evenements
	if (_topParent)
		_topParent->removeEventFilter(this);

	_topParent = qobject_cast<QWidget*>(this->parentWidget());
	while(_topParent != NULL)
	{
		_topParent = qobject_cast<QWidget*>(_topParent->parentWidget());
		if(_topParent)
			lastParent = _topParent;
	}

	if(lastParent)
	{
		//Si le parent de plus haut niveau a ete trouve, on ecoute ses evenements
		_topParent = lastParent;
		_topParent->installEventFilter(this);
	}
	
	//Mise a jour de sa position
	if (this->parentWidget())
	{
		QPoint pos = QPoint(this->parentWidget()->mapToGlobal(QPoint(0,0)));
		_mainRect = QRect(pos, this->parentWidget()->size());
		updatePosition();
	}
}