#include "SwDockWidget_ToolBarItem.h"

#include "SwDockWidget_DockWidget.h"

#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

//-----------------------------------------------------------------------------
SwDockWidget_ToolBarItem::SwDockWidget_ToolBarItem(QWidget * dock, QString title, QWidget * toolBar, QSize size)
 : QToolButton(), _widget(dock), _toolBar(toolBar), _tempMovingItem(NULL), _orientation(Qt::Vertical),
	_canMove(false), _isMoving(false), _titleBarSize(size)
{
	if (_widget)
	{
		setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		setCheckable(true);
		setChecked(false);
		//Texte
		setText(title);
		//Icone
		setIcon(dock->windowIcon());
		//Tooltip
		setToolTip(title);
		//Widget masque apres creation
		_widget->hide();
	}

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//Taille de l'icone
	this->setIconSize(QSize(20,20));
}

//-----------------------------------------------------------------------------
SwDockWidget_ToolBarItem::SwDockWidget_ToolBarItem(SwDockWidget_ToolBarItem * item)
 : QToolButton(), _canMove(false), _isMoving(false), _tempMovingItem(NULL)
{
	if (item)
	{
		_titleBarSize = item->getTitleBarSize();
		setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		setCheckable(true);
		setChecked(false);
		//Texte
		setText(item->text());
		//Icone
		setIcon(item->icon());
		//Tooltip
		setToolTip(item->toolTip());
		//Widget
		_widget = item->getWidget();
		_toolBar = item->getToolBar();
	}

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	//Taille de l'icone
	this->setIconSize(QSize(20,20));
}

//-----------------------------------------------------------------------------
SwDockWidget_ToolBarItem::~SwDockWidget_ToolBarItem()
{
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::showWidget()
{
	if (_widget)
	{
		//Si le widget est affiche, on le masque
		if (_widget->isVisible())
		{
			//Masquage avec animation
			QPropertyAnimation * effect = new QPropertyAnimation(_widget, "windowOpacity");
			effect->setDuration(300);
			effect->setStartValue(1);
			effect->setEndValue(0);
			effect->start();
			//Lorsque la fenetre est transparante, on la masque
			connect(effect, SIGNAL(finished()), _widget, SLOT(hide()));
		}
		//Sinon, on l'affiche
		else if (_toolBar)
		{
			QPoint pos = setPosition();

			//Mise a jour de la position
			_widget->move(pos);

			//Fenetre transparante avant affichage, pour utiliser l'effet
			_widget->setWindowOpacity(0);
			_widget->show();
			//Affichage avec animation
			QPropertyAnimation * effect = new QPropertyAnimation(_widget, "windowOpacity");
			effect->setDuration(300);
			effect->setStartValue(0);
			effect->setEndValue(1);
			effect->start();
		}
	}
}

//-----------------------------------------------------------------------------
QPoint SwDockWidget_ToolBarItem::setPosition()
{
	//Recuperation de la taille de l'ecran actif
	QDesktopWidget desktop;
	QRect rect = desktop.screenGeometry (desktop.screenNumber(QCursor::pos()));
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(_widget);
	
	//Ajustement de la position du dock avant ouverture
	QPoint pos = this->pos();
	//Si la toolbar est verticale, ouverture a gauche ou a droite du bouton
	if (_orientation == Qt::Vertical)
	{
		pos = _toolBar->pos() + this->pos() + QPoint(this->width(),0); //Ouverture a droite par defaut
		//Si l'item est situe dans la partie droite de l'ecran, on ouvre la fenetre a gauche
		if (_toolBar->pos().x() > (rect.x() + rect.size().width() / 2) ) //On ajoute la position rect.x() pour gere le cas multi-ecran
		{
			if( dock)
				dock->showShadow(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
			pos.setX(_toolBar->pos().x() + this->pos().x() - _widget->width());
		}
		//Si l'item est situe dans la partie gauche de l'ecran, on ouvre la fenetre a droite
		else
		{
			if( dock)
				dock->showShadow(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
		}
		//Si l'item est trop pret du bord inferieur de l'ecran, on ouvre la fenetre vers le haut
		if (pos.y() + _widget->height() > (rect.y() + rect.height()))
		{
			pos.setY(_toolBar->pos().y() + this->pos().y() + this->height() -_widget->height());
		}
	}
	//Si la toolbar est horizontale, ouverture au dessus ou au dessous du bouton
	else 
	{
		pos = _toolBar->pos() + this->pos() + QPoint(-dock->getShadowSize(), this->height() + _titleBarSize.height()); //Ouverture a droite par defaut
		//Si l'item est situe dans la partie droite de l'ecran, on ouvre la fenetre a gauche
		if (_toolBar->pos().x() > (rect.x() + rect.size().width() / 2) ) //On ajoute la position rect.x() pour gere le cas multi-ecran
		{
			pos.setX(_toolBar->pos().x() + this->pos().x() + this->width() + dock->getShadowSize() - _widget->width());
		}
		dock->showShadow(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
		//Si l'item est trop pret du bord inferieur de l'ecran, on ouvre la fenetre vers le haut
		if (pos.y() + _widget->height() > (rect.y() + rect.height()))
		{
			pos.setY(_toolBar->pos().y() + this->pos().y() -_widget->height());
			if( dock)
				dock->showShadow(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
		}
	}

	return pos;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::hideWidget(bool withEffect)
{
	if (_widget)
	{
		if (isChecked())
			setChecked(false);
		//Masquage avec animation
		QPropertyAnimation * effect = new QPropertyAnimation(_widget, "windowOpacity");
		if (withEffect)
			effect->setDuration(300);
		else
			effect->setDuration(10);
		effect->setStartValue(1);
		effect->setEndValue(0);
		effect->start();
		//Lorsque la fenetre est transparante, on la masque
		connect(effect, SIGNAL(finished()), _widget, SLOT(hide()));
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::setToolBar(QWidget * widget)
{
	if (widget != _toolBar)
		_toolBar = widget;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBarItem::getToolBar()
{
	return _toolBar;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBarItem::getWidget()
{
	return _widget;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBarItem::getMovingItem()
{
	return _tempMovingItem;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::setMovingItem(QWidget * widget)
{
	if (widget != _tempMovingItem)
		_tempMovingItem = widget;
}

//-----------------------------------------------------------------------------
//Gestion du clic
void SwDockWidget_ToolBarItem::mousePressEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		_canMove = true;
		_isMoving = false;
		_clickPos = QCursor::pos();
	}
	QToolButton::mousePressEvent(event);
}

//-----------------------------------------------------------------------------
//Relachement du bouton = fin du clic
void SwDockWidget_ToolBarItem::mouseReleaseEvent( QMouseEvent * event )
{
	if (event->button() == Qt::LeftButton)
	{
		if (!_isMoving)
			showWidget();
		else
		{
			_canMove = false;
			_isMoving = false;
			emit this->stopMoving();
			// Reset cursor
			setCursor (Qt::ArrowCursor);
		}
	}
	QToolButton::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
//Deplacement de la souris avec clic actif = deplacement de la fenetre
void SwDockWidget_ToolBarItem::mouseMoveEvent( QMouseEvent * event )
{
	//Si un deplacement est en cours
	if (_canMove)
	{
		if ((event->pos() - _clickPos).manhattanLength() > QApplication::startDragDistance())
		{
			QPoint cursor = QCursor::pos();

			//Si le curseur est deplace en dehors du bouton, on gere le deplacement
			if (!_isMoving)
			{
				if (event->pos().x() < 0 || event->pos().y() < 0 || event->pos().x() > this->width() || event->pos().y() > this->height())
				{
					_isMoving = true;
					setCursor (Qt::ClosedHandCursor);
				}
			}

			if (_isMoving)
			{
				emit this->isMoving(cursor - _clickPos);
				_clickPos = cursor;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::setOrientation(Qt::Orientation orientation)
{
	if (orientation != _orientation)
		_orientation = orientation;
}

//-----------------------------------------------------------------------------
QSize SwDockWidget_ToolBarItem::getTitleBarSize()
{
	return _titleBarSize;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarItem::updateWidgetPosition()
{
	//Si le widget est ouvert, on met a jour sa position
	if (_widget && _widget->isVisible())
	{
		_widget->move(setPosition());
	}
}