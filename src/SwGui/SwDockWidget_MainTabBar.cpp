#include "SwDockWidget_MainTabBar.h"

#include <QtGui/QMouseEvent>
#include <QKeyEvent>

//-----------------------------------------------------------------------------
SwDockWidget_MainTabBar::SwDockWidget_MainTabBar(QWidget * parent)
: QTabBar(parent), _nameEdit(NULL), _name(""), _moving(false)
{

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

	QTabBar::mousePressEvent(e);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton && tabsClosable())
	{
		if (_moving)
			emit stopMovingTabRequested();
		_moving = false;
	}

	QTabBar::mouseReleaseEvent(e);
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::mouseMoveEvent(QMouseEvent * e)
{
	if (!_moving && tabsClosable())
	{
		QRect rect = this->rect();
		//Si le pointeur est sortie de la TabBar, deplacement de l'onglet
		if (e->pos().x() < rect.x() ||
			e->pos().x() > rect.x() + rect.width() ||
			e->pos().y() < rect.y() ||
			e->pos().y() > rect.y() + rect.height())
		{
			//Cas particulier : l'onglet "add" ne peut etre deplace
			if (currentIndex() != count()-1)
			{
				_moving = true;
				QPoint cursor = QCursor::pos();
				//Liberation de l'onglet et initialisation du deplacement
				emit freeTabRequested(currentIndex(), cursor - _clickPos);
				return;
			}
		}
		//Sinon verification d'une reorganisation des onglets
		/*else
		{
			int current = currentIndex();
			for (int i = 0; i < count(); i++)
			{
				QRect rect = tabRect(i);

				if (i != current)
				{
					//Si le pointeur est au dessus d'un autre onglet, on le deplace
					if ((e->pos().x() >= rect.x()) && (e->pos().x() < (rect.x() + rect.width())))
					{
						moveTab(current, i);
						return;
					}
				}
			}
		}*/
	}

	if (_moving)
	{
		QPoint cursor = QCursor::pos();
		setCurrentIndex(0);
		emit moveTabRequested(cursor - _clickPos);
	}
	else
	{
		QTabBar::mouseMoveEvent(e);
	}
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

		//Creation d'une LineEdit pour editer le nom de l'onglet
		int index = currentIndex();
		_name = tabText(index);
		_nameEdit = new QLineEdit(_name, this);
		_nameEdit->setFrame(false);
		_nameEdit->setFocus();
		connectNameEdit();
		//Placement de la LineEdit sur l'onglet
		QRect rect(tabRect(index));
		//Ajustement de la taille et de la position pour que le widget soit a l'interieur de l'onglet
		_nameEdit->move(QPoint(rect.x(), rect.y())/* + QPoint(9,2)*/);
		_nameEdit->resize(rect.size()/* - QSize(12, 5)*/);
		_nameEdit->show();
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
		_nameEdit->resize(rect.size() - QSize(12, 5));
		_nameEdit->move(QPoint(rect.x(), rect.y()) + QPoint(9,2));
	}
	QTabBar::paintEvent(event);
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
		//connect(_nameEdit, SIGNAL(returnPressed()), this, SLOT(renameTab()));
		//connect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(cancelNameEdit()));
		connect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(renameTab()));
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_MainTabBar::disconnectNameEdit()
{
	if (_nameEdit)
	{
		disconnect(_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(renameTabTemporarily(QString)));
		//disconnect(_nameEdit, SIGNAL(returnPressed()), this, SLOT(renameTab()));
		//disconnect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(cancelNameEdit()));
		disconnect(_nameEdit, SIGNAL(editingFinished()), this, SLOT(renameTab()));
	}
}