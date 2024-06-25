/**
@file StreamScene.cpp
@brief Scene Stream
@author F.Bighelli
*/

#include "StreamScene.h"
#include "WidgetFactory.h"
#include "IAnchorable.h"
#include "ConnectorGraphicItem.h"
#include "LinkGraphicItem.h"
#include "Arranger.h"
#include "MenuManager.h"
#define G_MARGIN 10.0



//-----------------------------------------------------------------------
StreamScene::StreamScene(QObject * parent) :QGraphicsScene(parent)
{
	_observerEnabled = true;
	_tmpLinkItem = 0;
	_linkTarget = 0;
	_linkSrc = 0;
	_connectorToMove = 0;
	_tmpTargetConnector = 0;

	installEventFilter(this);
	_lineX = new QGraphicsLineItem(-5, 0, 5, 0);
	_lineX->setPen(QColor(Qt::magenta));
	_lineX->setFlag(QGraphicsItem::ItemIsSelectable, false);
	_lineX->setFlag(QGraphicsItem::ItemIsMovable, false);
	_lineX->setZValue(20000);
	addItem(_lineX);

	_lineY = new QGraphicsLineItem(0,-5, 0, 5);
	_lineY->setPen(QColor(Qt::magenta));
	_lineY->setFlag(QGraphicsItem::ItemIsSelectable, false);
	_lineY->setFlag(QGraphicsItem::ItemIsMovable, false);
	_lineY->setZValue(20000);
	addItem(_lineY);
}

//-----------------------------------------------------------------------
void StreamScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw Grid
	{
		painter->save();
		const int gridSize = 100;

		qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
		qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

		QVarLengthArray<QLineF, 100> lines;

		for ( qreal x = left; x < rect.right(); x += gridSize )
			lines.append(QLineF(x, rect.top(), x, rect.bottom()));
		for ( qreal y = top; y < rect.bottom(); y += gridSize )
			lines.append(QLineF(rect.left(), y, rect.right(), y));

		painter->setPen(QPen(QColor("#404040")));
		painter->drawLines(lines.data(), lines.size());
		painter->restore();
	}
}

//-----------------------------------------------------------------------
void StreamScene::setVisibleArea(const QRectF & visibleArea)
{
	_visibleArea = visibleArea;

	/*QRectF rect = _visibleArea;
	rect.setY(rect.y()-1000);
	rect.setX(rect.x()-1000);
	rect.setHeight(rect.height()+2000);
	rect.setWidth(rect.width()+2000);
	setSceneRect(rect);*/

	updateWidgets();
}

//-----------------------------------------------------------------------
IAnchorable * StreamScene::addApplicativeWidget(QWidget *w)
{
	QGraphicsProxyWidget * gw = addWidget(w);
	gw->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	gw->setAcceptDrops(false);
	_wList.push_back(gw);
	gw->setZValue(100.0);
	IAnchorable * anchorable = dynamic_cast<IAnchorable *>(gw->widget());
	if ( anchorable != 0 )
	{
		anchorable->addObserver(this);
	}
	updateWidgets();
	return anchorable;
}

//-----------------------------------------------------------------------
void StreamScene::anchorableAsChanged(QWidget *w)
{
	if ( _observerEnabled )
		updateWidgets();
}

//-----------------------------------------------------------------------
void StreamScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	bool positionDone = false;
	if ( _tmpLinkItem != 0 )
	{
		QList<QGraphicsItem *> litems = items(mouseEvent->scenePos());
		QList<QGraphicsItem *>::iterator it = litems.begin();
		while ( it != litems.end() && !positionDone )
		{
			ConnectorGraphicItem *citem = dynamic_cast<ConnectorGraphicItem *>(*it);
			bool testCible = citem != 0 && citem->isVisible();
			testCible = testCible &&
				((citem->getConnectorType() == PROVIDER && _linkSrc->getConnectorType() == CONSUMER) ||
				(citem->getConnectorType() == PIN && _linkSrc->getConnectorType() == PIN));
			if ( testCible )
			{
				_tmpLinkItem->setTarget(citem);
				_linkTarget = citem;
				positionDone = true;
			}
			it++;
		}
		if ( !positionDone )
		{
			_tmpLinkItem->setTemporaryTarget(mouseEvent->scenePos());
			_linkTarget = 0;
		}
	}
	if ( _connectorToMove != 0 )
	{
		ComponentGraphicItem * parent = (ComponentGraphicItem *) _connectorToMove->parentItem();
		QGraphicsItem * tmpTarget = itemAt(mouseEvent->scenePos(), QTransform());
		if ( tmpTarget != 0 )
		{
			ConnectorGraphicItem * targetConnector = dynamic_cast<ConnectorGraphicItem *>(tmpTarget);
			if ( _tmpTargetConnector != 0 && _tmpTargetConnector != targetConnector )
			{
				_tmpTargetConnector->toggleHover();
				_tmpTargetConnector = 0;
			}
			if ( targetConnector != 0 )
			{
				_tmpTargetConnector = targetConnector;
				_tmpTargetConnector->toggleHover();
				parent->hideConnectorInsertion();
			}
			else
			{
				if ( parent == dynamic_cast<ComponentGraphicItem *>(tmpTarget) )
				{
					QPointF pmv = parent->mapFromScene(mouseEvent->scenePos());
					parent->showConnectorInsertion(pmv);
				}
				else
				{
					parent->hideConnectorInsertion();
				}
			}
		}
		else
		{
			parent->hideConnectorInsertion();
			if ( _tmpTargetConnector != 0 )
			{
				_tmpTargetConnector->toggleHover();
				_tmpTargetConnector = 0;
			}
		}
	}
}

//-----------------------------------------------------------------------
void StreamScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
	if ( mouseEvent->buttons().testFlag(Qt::LeftButton) && !(mouseEvent->modifiers() & Qt::ControlModifier) )
	{
		QGraphicsItem * tmpTarget = itemAt(mouseEvent->scenePos(), QTransform());
		if ( tmpTarget != 0 )
		{
			_linkSrc = dynamic_cast<ConnectorGraphicItem *>(tmpTarget);
			if ( _linkSrc != 0 && _linkSrc->getLinks()->isEmpty() &&
				(_linkSrc->getConnectorType() == CONSUMER ||
				_linkSrc->getConnectorType() == PIN) )
			{
				_tmpLinkItem = new LinkGraphicItem(_linkSrc);
				_tmpLinkItem->setTemporaryTarget(mouseEvent->scenePos());
				addItem(_tmpLinkItem);
				showOnlyCompatiblesItems(_linkSrc);
			}
		}
	}
	else
		if ( mouseEvent->buttons().testFlag(Qt::LeftButton) && (mouseEvent->modifiers() & Qt::ControlModifier) )
		{
			QGraphicsItem * tmpTarget = itemAt(mouseEvent->scenePos(), QTransform());
			if ( tmpTarget != 0 )
			{
				_connectorToMove = dynamic_cast<ConnectorGraphicItem *>(tmpTarget);
				if ( _connectorToMove != 0 )
				{
					QApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
					showOnlyItemsWithSameParent(_connectorToMove);
				}
			}
		}
}

//-----------------------------------------------------------------------
void StreamScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	if ( _tmpLinkItem != 0 )
	{
		showAllItems(_linkSrc);
		if ( _linkTarget != 0 && _linkSrc != 0 )
		{
			onLinkConnectors(_linkSrc, _linkTarget);
		}
		delete _tmpLinkItem;
		_tmpLinkItem = 0;
		_linkSrc = 0;
	}
	if ( _connectorToMove != 0 )
	{
		ComponentGraphicItem * parent = (ComponentGraphicItem *) _connectorToMove->parentItem();
		QGraphicsItem * tmpTarget = itemAt(mouseEvent->scenePos(), QTransform());
		showAllItems(_connectorToMove);
		QApplication::restoreOverrideCursor();
		if ( tmpTarget != 0 )
		{
			ConnectorGraphicItem * targetConnector = dynamic_cast<ConnectorGraphicItem *>(tmpTarget);
			if ( targetConnector != 0 )
			{
				ComponentGraphicItem * p = (ComponentGraphicItem *) _connectorToMove->parentItem();
				QList<ConnectorGraphicItem *> * liste = p->getConnectors();
				int i = liste->indexOf(_connectorToMove);
				int j = liste->indexOf(targetConnector);
				liste->swap(i, j);
				TConnectorPosition cpos = _connectorToMove->getParentPosition();
				_connectorToMove->setParentPosition(targetConnector->getParentPosition());
				targetConnector->setParentPosition(cpos);
				p->updateAttributs();
			}
			else
			{
				if ( parent == dynamic_cast<ComponentGraphicItem *>(tmpTarget) )
				{
					QPointF pmv = parent->mapFromScene(mouseEvent->scenePos());
					parent->showConnectorInsertion(pmv);
					parent->moveConnector(_connectorToMove);
				}
			}
		}
		if ( _tmpTargetConnector != 0 )
		{
			_tmpTargetConnector->toggleHover();
			_tmpTargetConnector = 0;
		}
		parent->hideConnectorInsertion();
		_connectorToMove = 0;
	}
}

//--------------------------------------------------------------------------

//-----------------------------------------------------------------------
void StreamScene::updateWidgets()
{
	_observerEnabled = false;
	QPointF f;
	QList<QGraphicsProxyWidget *>::iterator it = _wList.begin();
	while ( it != _wList.end() )
	{
		QWidget * w = (*it)->widget();
		IAnchorable * anchorable = dynamic_cast<IAnchorable *>(w);
		if ( anchorable != 0 )
		{
			if ( anchorable->isVerticalExtensible() && !anchorable->isHorizontalExtensible() )
			{
				w->resize(w->width(), (int) (_visibleArea.height() - 20.0));
			}
			else if ( anchorable->isHorizontalExtensible() && !anchorable->isVerticalExtensible() )
			{
				w->resize((int) (_visibleArea.width() - 20.0), w->height());
			}
			else if ( anchorable->isHorizontalExtensible() && anchorable->isVerticalExtensible() )
			{
				w->resize((int) (_visibleArea.width() - 20.0), (int) (_visibleArea.height() - 20.0));
			}
			switch ( anchorable->getAnchorPosition() )
			{
				case TOP_LEFT:
					(*it)->setPos(_visibleArea.topLeft() + QPointF(G_MARGIN, G_MARGIN));
					break;
				case TOP_RIGHT:
					f = QPointF(_visibleArea.topRight() + QPointF(-G_MARGIN, G_MARGIN));
					f.setX(f.x() - w->width());
					(*it)->setPos(f);
					break;
				case BOTTOM_LEFT:
					f = QPointF(_visibleArea.bottomLeft() + QPointF(G_MARGIN, -G_MARGIN));
					f.setY(f.y() - w->height());
					(*it)->setPos(f);
					break;
				case BOTTOM_RIGHT:
					f = QPointF(_visibleArea.bottomRight() + QPointF(-G_MARGIN, -G_MARGIN));
					f.setX(f.x() - w->width());
					f.setY(f.y() - w->height());
					(*it)->setPos(f);
					break;
				default:
					break;
			}
		}
		it++;
	}
	update();
	_observerEnabled = true;
}

//-----------------------------------------------------------------------
void StreamScene::showOnlyCompatiblesItems(ConnectorGraphicItem * itemSource)
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	while ( it != litems.end() )
	{
		ConnectorGraphicItem *citem = dynamic_cast<ConnectorGraphicItem *>(*it);
		if ( citem != 0 && citem != itemSource )
		{
			bool testVisible = (itemSource->getConnectorType() == CONSUMER && citem->getConnectorType() == PROVIDER);
			testVisible = testVisible || (itemSource->getConnectorType() == PIN && citem->getConnectorType() == PIN && citem->getLinks()->isEmpty());
			testVisible = testVisible && citem->getModelType() == itemSource->getModelType();
			testVisible = testVisible && citem->parentItem() != itemSource->parentItem();
			if ( testVisible )
			{
				citem->toggleHighlightExtern();
			}
			else
			{
				citem->setVisible(false);
			}
		}
		LinkGraphicItem * litem = dynamic_cast<LinkGraphicItem *>(*it);
		if ( litem != 0 && _tmpLinkItem != litem )
		{
			litem->setVisible(false);
		}
		it++;
	}
}

//-----------------------------------------------------------------------
void StreamScene::showOnlyItemsWithSameParent(ConnectorGraphicItem * itemSource)
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	while ( it != litems.end() )
	{
		ConnectorGraphicItem *citem = dynamic_cast<ConnectorGraphicItem *>(*it);
		if ( citem != 0 && citem != itemSource )
		{
			if ( citem->parentItem() != itemSource->parentItem() )
			{
				citem->setVisible(false);
			}
			else
			{
				citem->toggleHighlightExtern();
			}
		}

		LinkGraphicItem * litem = dynamic_cast<LinkGraphicItem *>(*it);
		if ( litem != 0 )
		{
			litem->setVisible(false);
		}
		it++;
	}
}


//-----------------------------------------------------------------------
void StreamScene::showAllItems(ConnectorGraphicItem * itemSource)
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	while ( it != litems.end() )
	{
		ConnectorGraphicItem *citem = dynamic_cast<ConnectorGraphicItem *>(*it);
		if ( citem != 0 && citem != itemSource )
		{
			if ( !citem->isVisible() )
			{
				citem->setVisible(true);
			}
			else
			{
				citem->toggleHighlightExtern();
			}
		}
		LinkGraphicItem * litem = dynamic_cast<LinkGraphicItem *>(*it);
		if ( litem != 0 )
		{
			litem->setVisible(true);
		}
		it++;
	}

}

//-----------------------------------------------------------------------
void StreamScene::rearrange()
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	QList<ComponentGraphicItem *> lcitems;
	while ( it != litems.end() )
	{
		ComponentGraphicItem *citem = dynamic_cast<ComponentGraphicItem *>(*it);
		if ( citem != 0 )
		{
			lcitems.push_back(citem);
		}
		it++;
	}
	Arranger * arranger = new Arranger();
	arranger->arrange(&lcitems);
}

//-----------------------------------------------------------------------
QList<ComponentGraphicItem *> StreamScene::getMainComponentGraphicItems()
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	QList<ComponentGraphicItem *> lcitems;
	while ( it != litems.end() )
	{
		ComponentGraphicItem *citem = dynamic_cast<ComponentGraphicItem *>(*it);
		if ( citem != 0 && citem->parentItem() == 0 )
		{
			lcitems.push_back(citem);
		}
		it++;
	}
	return lcitems;
}

//-----------------------------------------------------------------------
QList<ComponentGraphicItem *> StreamScene::getAllComponentGraphicItems()
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	QList<ComponentGraphicItem *> lcitems;
	while ( it != litems.end() )
	{
		ComponentGraphicItem *citem = dynamic_cast<ComponentGraphicItem *>(*it);
		if ( citem != 0 )
		{
			lcitems.push_back(citem);
		}
		it++;
	}
	return lcitems;
}

//-----------------------------------------------------------------------
QList<InterestArea *> StreamScene::getAllInterestAreas()
{
	QList<QGraphicsItem *> litems = QGraphicsScene::items();
	QList<QGraphicsItem *>::iterator it = litems.begin();
	QList<InterestArea *> lcitems;
	while ( it != litems.end() )
	{
		InterestArea *citem = dynamic_cast<InterestArea *>(*it);
		if ( citem != 0 )
		{
			lcitems.push_back(citem);
		}
		it++;
	}
	return lcitems;
}

//-----------------------------------------------------------------------
bool StreamScene::eventFilter(QObject *obj, QEvent *event)
{
	if ( event->type() == QEvent::KeyPress )
	{
		QKeyEvent *c = dynamic_cast<QKeyEvent *>(event);
		if ( c && c->key() == Qt::Key_Delete )
		{
			MenuManager::getInstance()->buildContextMenu(QPointF(0, 0));
			MenuManager::getInstance()->onRemove();
			return true;
		}
	}
	return QGraphicsScene::eventFilter(obj, event);
}

