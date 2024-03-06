#include "SwDockWidget_Overlay.h"
#include "SwDockWidget_DockWidget.h"

#include <QPainter>
#include <QPen>

//-----------------------------------------------------------------------------
SwDockWidget_Overlay::SwDockWidget_Overlay(QWidget *parent, QWidget * mainWidget, QColor color)
: QWidget(parent), _color(color)
{
	_back = NULL;
	_top = NULL;
	_mainWidget = NULL;
	setPalette(Qt::transparent);
	this->hide();
	setMainWidget(mainWidget);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
}

//-----------------------------------------------------------------------------
SwDockWidget_Overlay::~SwDockWidget_Overlay()
{

}

//-----------------------------------------------------------------------------
void SwDockWidget_Overlay::setColor(QColor color)
{
	_color = color;
}

//-----------------------------------------------------------------------------
void SwDockWidget_Overlay::paintEvent(QPaintEvent * /*event*/)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	//Affichage d'un rectangle partiellement transparent
	painter.fillRect(0, 0, width(), height(), _color);
}

//-----------------------------------------------------------------------------
//Enregistrement des widgets sur lesquels afficher le rectangle
void SwDockWidget_Overlay::setWidgets(QWidget * back, QWidget * top)
{
	//Widget sur lequel va etre positionne le DockWidget
	if (back)
		_back = back;
	//DockWidget a positionner
	if (top)
		_top = top;
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_Overlay::getDockWidget()
{
	return _top;
}

//-----------------------------------------------------------------------------
//Enregistrement du widget principal (aire complete qui contient tous les DockWidget)
void SwDockWidget_Overlay::setMainWidget(QWidget * mainWidget)
{
	if (mainWidget)
		_mainWidget = mainWidget;
}

//-----------------------------------------------------------------------------
//Affichage du rectangle de pre-positionnement
void SwDockWidget_Overlay::showOn(Qt::DockWidgetArea area, bool toMain)
{
	QPoint pos;
	QSize size;
	QWidget * back = NULL;
	this->setParent(0);
	//Si affichage sur le widget principal
	if (toMain)
		back = _mainWidget;
	//Si affichage sur le widget
	else
		back = _back;

	if (back)
	{
		this->setParent(back);
		QSize topSize = _top->size();
		SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(_top);
		if (dock)
			topSize = dock->getRawSize();

		switch(area)
		{
			//Positionnement a gauche
			case Qt::LeftDockWidgetArea:
				//Si le dock widget peut etre mis entierement dans le main widget
				if ( back->minimumSizeHint().width() + topSize.width() < back->width() )
					size = QSize(topSize.width(), back->height());
				//Sinon, si le dock widget redimensionne a son minimum peut etre mis dans le main widget redimensionne a son minimum
				else if ( back->minimumSizeHint().width() + _top->minimumSizeHint().width() < back->width() )
					size = QSize(back->width() - back->minimumSizeHint().width(), back->height());
				//Sinon affichage de la plus petite taille possible du dock widget
				else
					size = QSize(_top->minimumSizeHint().width(), back->height());

				pos = QPoint(0, 0);
				break;

			//Positionnement a droite
			case Qt::RightDockWidgetArea:
				//Si le dock widget peut etre mis entierement dans le main widget
				if ( back->minimumSizeHint().width() + topSize.width() < back->width() )
					size = QSize(topSize.width(), back->height());
				//Sinon, si le dock widget redimensionne a son minimum peut etre mis dans le main widget redimensionne a son minimum
				else if ( back->minimumSizeHint().width() + _top->minimumSizeHint().width() < back->width() )
					size = QSize(back->width() - back->minimumSizeHint().width(), back->height());
				//Sinon affichage de la plus petite taille possible du dock widget
				else
					size = QSize(_top->minimumSizeHint().width(), back->height());

				pos = QPoint(qMax(back->width()-size.width(),0), 0);
				break;

			//Positionnement en haut
			case Qt::TopDockWidgetArea:
				//Si le dock widget peut etre mis entierement dans le main widget
				if ( back->minimumSizeHint().height() + topSize.height() < back->height() )
					size = QSize(back->width(), topSize.height());
				//Sinon, si le dock widget redimensionne a son minimum peut etre mis dans le main widget redimensionne a son minimum
				else if ( back->minimumSizeHint().height() + _top->minimumSizeHint().height() < back->height() )
					size = QSize(back->width(), back->height() - back->minimumSizeHint().height());
				//Sinon affichage de la plus petite taille possible du dock widget
				else
					size = QSize(back->width(), _top->minimumSizeHint().height());

				pos = QPoint(0, 0);
				break;

			//Positionnement en bas
			case Qt::BottomDockWidgetArea:
				//Si le dock widget peut etre mis entierement dans le main widget
				if ( back->minimumSizeHint().height() + topSize.height() < back->height() )
					size = QSize(back->width(), topSize.height());
				//Sinon, si le dock widget redimensionne a son minimum peut etre mis dans le main widget redimensionne a son minimum
				else if ( back->minimumSizeHint().height() + _top->minimumSizeHint().height() < back->height() )
					size = QSize(back->width(), back->height() - back->minimumSizeHint().height());
				//Sinon affichage de la plus petite taille possible du dock widget
				else
					size = QSize(back->width(), _top->minimumSizeHint().height());

				pos = QPoint(0, qMax(back->height()-size.height(),0));
				break;

			//Onglet
			case Qt::AllDockWidgetAreas:
				pos = QPoint(0, 0);
				size = QSize(back->width(), back->height());
				break;
		}

		//Mise a jour de la position et de la taille du rectangle de pre-positionnement
		this->move(pos);
		this->resize(size);

		//Affichage
		show();
	}
}
