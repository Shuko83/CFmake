#include "SwDockWidget_ToolBarWindow.h"

#include "SwDockWidget_ToolBar.h"
#include "SwDockWidget_ToolBarItem.h"
#include "SwDockWidget_DockWidget.h"

#include <QMessageBox>

#define OVERLAY_COLOR			QColor(124,189,255,255)

//-----------------------------------------------------------------------------
SwDockWidget_ToolBarWindow::SwDockWidget_ToolBarWindow(QWidget * parent)
 : QWidget(parent), _isMovingToolBarItem(false), _tbOverlay(NULL), _lock(false), _mainWidget(NULL)
{
	//Creation de la zone de pre-positionnement
	_tbOverlay = new SwDockWidget_Overlay(this, NULL, OVERLAY_COLOR);
}

//-----------------------------------------------------------------------------
SwDockWidget_ToolBarWindow::~SwDockWidget_ToolBarWindow()
{
}



//-----------------------------------------------------------------------------
// Gestion des SwDockWidget_ToolBar
//-----------------------------------------------------------------------------
//Reduction du SwDockWidget_DockWidget dans une SwDockWidget_ToolBar
void SwDockWidget_ToolBarWindow::reduceInToolBar()
{
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(this->sender());
	if (dock)
	{
		//Recuperation de la position
		QPoint pos = dock->pos();
		//Creation d'une toolbar
		SwDockWidget_ToolBar * tb = new SwDockWidget_ToolBar(_mainWidget);
		//Creation d'un bouton pour le dock actif
		addInToolBar(dock, tb);
		//Mise a jour de la position de la toolbar
		tb->move(pos + QPoint(10,10));
		//Enregistrement de la toolbar
		_listToolBar.push_back(tb);
		connectSignals(tb);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::addInToolBar(QWidget * widget, QWidget * toolbar)
{
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
	SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(toolbar);

	if (widget && tb)
	{
		//Creation d'un bouton pour le dock actif
		SwDockWidget_ToolBarItem * tbi = new SwDockWidget_ToolBarItem(dock, dock->getTitle(), tb, tb->getTitleBarSize());
		//Mise a jour de l'etat du dock
		dock->setInToolBar(true, tbi);
		//Ajout du bouton dans la toolbar
		tb->addItem(tbi);
		//Ecoute des evenements
		connect(tbi, SIGNAL(isMoving(QPoint)), this, SLOT(moveToolBarItem(QPoint)));
		connect(tbi, SIGNAL(stopMoving()), this, SLOT(stopMovingToolBarItem()));
	}
}

//-----------------------------------------------------------------------------
//Liberation du SwDockWidget_DockWidget dans une SwDockWidget_ToolBar
void SwDockWidget_ToolBarWindow::releaseFromToolBar(QWidget * widget)
{
	SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget *>(widget);

	if (dock == NULL)
		dock = qobject_cast<SwDockWidget_DockWidget*>(this->sender());

	if (dock)
	{
		//Recuperation du bouton
		SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(dock->getToolBarItem());
		if (item && item->isVisible())
		{
			SwDockWidget_ToolBar * toolBar = qobject_cast<SwDockWidget_ToolBar*>(item->getToolBar());
			if (toolBar)
			{
				//Suppression du bouton
				toolBar->removeItem(item);
				item->setParent(0);
				//Si necessaire, suppression de la toolbar associee
				if (toolBar->isEmpty())
				{
					_listToolBar.removeOne(toolBar);
					delete toolBar;
				}
				//Mise a jour de l'etat du dock
				dock->setInToolBar(false);
			}
		}
		dock->setToolBarItem(0);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::moveToolBarItem(QPoint pos)
{
	SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(this->sender());
	if (item && !_lock)
	{
		//Si l'element est le seul de la toolbar, on deplace la toolbar
		SwDockWidget_ToolBar * toolbar = qobject_cast<SwDockWidget_ToolBar*>(item->getToolBar());
		if (toolbar->count() == 1)
		{
			//Suppression du shadowitem precedent
			if (_tbOverlay)
			{
				_tbOverlay->setParent(this); //Recuperation du toolbar
				_tbOverlay->hide();
			}

			//Deplacement de la toolbar
			toolbar->setMoving(true);
			toolbar->setWindowOpacity(0.4);
			toolbar->move(toolbar->pos() + pos);

			//Si l'element est au-dessus d'une autre toolbar, on cree un element provisoire de placement
			SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(getToolBarUnderCursor(toolbar));
			if (tb)
				tb->showOverlay(QCursor::pos(), _tbOverlay);
		}
		//Sinon, on cree une nouvelle toolbar
		else
		{
			item->getWidget()->hide();
			QPoint newPos = item->pos() + toolbar->pos();
			SwDockWidget_ToolBar * newTb = new SwDockWidget_ToolBar(_mainWidget);
			//Mise a jour de la position de la toolbar
			newTb->move(newPos + pos);
			//Ajout du bouton dans la toolbar
			toolbar->removeItem(item);

			//Creation d'un nouvel item permettant de conserver les event de deplacement
			//Le premier item recupere les informations de deplacement, le deuxieme est affiche et deplace
			SwDockWidget_ToolBarItem * newItem = new SwDockWidget_ToolBarItem(item);
			newTb->addItem(newItem);
			newItem->setToolBar(newTb);
			item->setToolBar(newTb);
			item->setMovingItem(newItem);
			item->resize(0,0);
			//Ecoute des evenements
			connect(newItem, SIGNAL(isMoving(QPoint)), this, SLOT(moveToolBarItem(QPoint)));
			connect(newItem, SIGNAL(stopMoving()), this, SLOT(stopMovingToolBarItem()));
			
			//Mise a jour de l'etat du dock
			SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(item->getWidget());
			if (dock)
			{
				dock->setToolBarItem(newItem);
			}

			_listToolBar.push_back(newTb);
			connectSignals(newTb);
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::stopMovingToolBarItem()
{
	SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(this->sender());
	if (item && !_lock)
	{
		if (_tbOverlay)
		{
			_tbOverlay->setParent(this); //Recuperation du toolbar
			_tbOverlay->hide();
		}

		//Mise a jour du curseur
		item->setCursor (Qt::ArrowCursor);
		
		//Si l'element est au-dessus d'une autre toolbar, on le place dedans
		SwDockWidget_ToolBar * toolbar = qobject_cast<SwDockWidget_ToolBar*>(item->getToolBar());
		toolbar->setWindowOpacity(1);
		SwDockWidget_ToolBar * newToolbar = qobject_cast<SwDockWidget_ToolBar*>(getToolBarUnderCursor(toolbar));
		if (newToolbar)
		{
			SwDockWidget_ToolBarItem * mItem = qobject_cast<SwDockWidget_ToolBarItem*>(item->getMovingItem());
			if (mItem)
			{
				item = mItem;
			}

			newToolbar->addItem(item, QCursor::pos());
			item->hideWidget(false);
			item->setToolBar(newToolbar);
			item->setCursor (Qt::ArrowCursor);
			item->setMovingItem(0);
			
			//Suppression de l'ancienne toolbar
			_listToolBar.removeOne(toolbar);
			delete toolbar;
		}
		else
		{
			//Suppression de l'element provisoire de deplacement si necessaire
			SwDockWidget_ToolBarItem * mItem = qobject_cast<SwDockWidget_ToolBarItem*>(item->getMovingItem());
			toolbar->setMoving(false);
			if (mItem)
			{
				item->hide();
				toolbar->removeItem(item);
				item = 0;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::moveToolBar(/*QPoint pos*/)
{
	SwDockWidget_ToolBar * toolbar = qobject_cast<SwDockWidget_ToolBar*>(this->sender());
	
	//Si l'element est le seul de la toolbar, on deplace la toolbar
	if (toolbar && toolbar->count() == 1 && !_lock)
	{
		//Suppression du shadowitem precedent
		if (_tbOverlay)
		{
			_tbOverlay->setParent(this); //Recuperation du toolbar
			_tbOverlay->hide();
		}

		//Si l'element est au-dessus d'une autre toolbar, on cree un element provisoire de placement
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(getToolBarUnderCursor(toolbar));
		if (tb)
			tb->showOverlay(QCursor::pos(), _tbOverlay);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::stopMovingToolBar()
{
	SwDockWidget_ToolBar * toolbar = qobject_cast<SwDockWidget_ToolBar*>(this->sender());
	if (toolbar && toolbar->count() == 1 && !_lock)
	{
		if (_tbOverlay)
		{
			_tbOverlay->setParent(this); //Recuperation du toolbar
			_tbOverlay->hide();
		}
		
		//Si l'element est au-dessus d'une autre toolbar, on le place dedans
		SwDockWidget_ToolBar * newToolbar = qobject_cast<SwDockWidget_ToolBar*>(getToolBarUnderCursor(toolbar));
		if (newToolbar)
		{
			SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(toolbar->itemAt(0));
			if (item)
			{
				newToolbar->addItem(item, QCursor::pos());
				item->hideWidget(false);
				item->setToolBar(newToolbar);
				
				//Suppression de l'ancienne toolbar
				_listToolBar.removeOne(toolbar);
				toolbar->deleteLater();
			}
		}
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBarWindow::getToolBarUnderCursor(QWidget * activeToolBar)
{
	QPoint pos = QCursor::pos();

	foreach (QObject * obj, _listToolBar)
	{
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
		if (tb && tb != activeToolBar)
		{
			if (QRect(tb->pos(), tb->size()).contains(pos))
				return tb;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
QList<QObject*> SwDockWidget_ToolBarWindow::getListToolBar()
{
	return _listToolBar;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::saveToolBar(QDomDocument doc, QDomElement dom)
{
	//Parcours des toolbar
	foreach(QObject * obj, _listToolBar)
	{
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
		if (tb)
		{
			//Classe
			QDomElement noeud = doc.createElement("SwDockWidget_ToolBar");

			//Sauvegarde de la position de la toolbar
			QDomElement pos = doc.createElement("pos");
			pos.setAttribute("x", tb->pos().x());
			pos.setAttribute("y", tb->pos().y());
			noeud.appendChild(pos);

			//Sauvegarde de l'orientation de la toolbar
			noeud.setAttribute("orientation", tb->getOrientation());

			//Sauvegarde des parametres d'aimantation de la toolbar
			noeud.setAttribute("aimantation", tb->getStuckPosition());
			QDomElement dist = doc.createElement("dist");
			dist.setAttribute("x", tb->getDist().x());
			dist.setAttribute("y", tb->getDist().y());
			noeud.appendChild(dist);

			//Verrouillage
			noeud.setAttribute("locked", QString::number(this->locked()));

			//Sauvegarde des elements qui constituent la toolbar
			for(int i = 0; i < tb->count(); i++)
			{
				SwDockWidget_ToolBarItem * item = qobject_cast<SwDockWidget_ToolBarItem*>(tb->itemAt(i));
				if (item)
				{
					writeWidgetParameters(doc, noeud, item->getWidget());
				}
			}

			dom.appendChild(noeud);
		}
	}
}

//-----------------------------------------------------------------------------
QWidget * SwDockWidget_ToolBarWindow::loadToolBar(QDomNode node)
{
	if (!node.isNull())
	{
		QDomElement e = node.toElement();
		QString name = e.nodeName();
		
		//Si toolbar
		if (!name.compare("SwDockWidget_ToolBar"))
		{
			SwDockWidget_ToolBar * tb = new SwDockWidget_ToolBar(_mainWidget);
			int nbDocks = 0;

			//Orientation
			Qt::Orientation o = (Qt::Orientation)e.attribute("orientation").toInt();
			tb->setOrientation(o);

			//Aimantation
			Qt::WindowFrameSection section = (Qt::WindowFrameSection)e.attribute("aimantation").toInt();
			//Verrouillage
			bool locked = e.attribute("locked").toInt();

			//Lecture des parametres
			QDomNodeList list = node.childNodes();
			for (int i = 0; i < list.count(); i++)
			{
				QDomNode attrNode = list.item(i);
				e = attrNode.toElement();

				//Widget
				if (!e.nodeName().compare("SwDockWidget_DockWidget"))
				{
					QWidget * widget = readWidgetParameters(attrNode);
					if (widget)
					{
						SwDockWidget_DockWidget * dock = qobject_cast<SwDockWidget_DockWidget*>(widget);
						if (dock)
						{
							addInToolBar(dock, tb);
							nbDocks++;
						}
					}
				}
				//Position
				else if (!e.nodeName().compare("pos"))
				{
					QPoint pos;
					pos.setX(e.attribute("x").toInt());
					pos.setY(e.attribute("y").toInt());
					tb->move(pos);
				}
				//Distance du bord
				else if (!e.nodeName().compare("dist"))
				{
					QPoint dist;
					dist.setX(e.attribute("x").toInt());
					dist.setY(e.attribute("y").toInt());
					tb->setDist(dist);
				}
			}

			//Aimantation
			tb->setStuckPosition(section);

			//Verrouillage
			if (locked)
				tb->lock();

			//Si la toolbar ne contient aucun dock, on la supprime
			if (nbDocks == 0)
			{
				delete tb;
			}
			else
			{
				//Enregistrement de la toolbar
				_listToolBar.push_back(tb);
				connectSignals(tb);

				return tb;
			}
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::closeToolBar()
{
	//Demande de confirmation avant fermeture
	QMessageBox msgBox;
	msgBox.setText("Are you sure you want to close the SwDockWidget_ToolBar?");
	msgBox.setStandardButtons(QMessageBox::Yes |QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int ret = msgBox.exec();
	if (ret == QMessageBox::No)
		return;

	SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(this->sender());

	if (_listToolBar.contains(tb))
	{
		//Recuperation des docks contenus dans la toolbar
		QList<QWidget*> list = tb->getListWidget();
		foreach (QWidget * widget, list)
		{
			widget->close();
		}

		_listToolBar.removeOne(tb);
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::connectSignals(QWidget * toolbar)
{
	if (toolbar)
	{
		connect(toolbar, SIGNAL(closeToolBarAsked()), this, SLOT(closeToolBar()));
		connect(toolbar, SIGNAL(isMoving()), this, SLOT(moveToolBar()));
		connect(toolbar, SIGNAL(stopMoving()), this, SLOT(stopMovingToolBar()));
	}
}

//-----------------------------------------------------------------------------
//Verrouillage de la disposition des docks
void SwDockWidget_ToolBarWindow::lock()
{
	if (!_lock)
	{
		_lock = true;
		foreach(QObject * obj, _listToolBar)
		{
			SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
			if (tb)
			{
				tb->lock();
			}
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::releaseLock()
{
	if (_lock)
	{
		_lock = false;
		foreach(QObject * obj, _listToolBar)
		{
			SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
			if (tb)
			{
				tb->releaseLock();
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool SwDockWidget_ToolBarWindow::locked()
{
	return _lock;
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::setLock(bool state)
{
	if (_lock != state)
	{
		_lock = state;
		if (state)
			lock();
		else
			releaseLock();
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::closeToolbars()
{
	foreach(QObject * obj, _listToolBar)
	{
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
		if (tb)
		{
			tb->close();
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::updateMainWidget()
{
	foreach(QObject * obj, _listToolBar)
	{
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
		if (tb)
		{
			tb->setParent(_mainWidget);
			tb->show(); //Widget masque par le changement de parent, doit etre re-affiche
		}
	}
}

//-----------------------------------------------------------------------------
void SwDockWidget_ToolBarWindow::updateToolBarMainRect()
{
	foreach(QObject * obj, _listToolBar)
	{
		SwDockWidget_ToolBar * tb = qobject_cast<SwDockWidget_ToolBar*>(obj);
		if (tb)
		{
			tb->updateMainRect();
		}
	}
}