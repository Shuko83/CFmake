/**
@file StreamView.cpp
@brief Vue Stream
@author F.Bighelli
*/

#include "StreamView.h"
#include "StreamControler.h"
#include "MainWindow.h"
#include "MenuManager.h"


//-----------------------------------------------------------------------
StreamView::StreamView(QWidget * parent) :QGraphicsView(parent)
{
	//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	setStyleSheet("QGraphicsView { background:#323232 }");
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setDragMode(QGraphicsView::RubberBandDrag);
	setAcceptDrops(true);
	//connect(this,SIGNAL(customContextMenuRequested ( const QPoint & )),
	//        this,SLOT(displayContextualMenu(const QPoint &)));
	//setContextMenuPolicy(Qt::CustomContextMenu);

	test = new QGraphicsRectItem();
	test->setOpacity(0.5);
	//Search management
	QFont font1;
	font1.setFamily(QString::fromUtf8("Segoe UI"));
	font1.setPointSize(15);

	_searchShortCut = new QShortcut(QKeySequence(tr("Ctrl+Shift+F", "Search")), this);
	connect(_searchShortCut, SIGNAL(activated()), this, SLOT(showSearchBox()));

	_scaleFactor = 1.0;

	_searchEdit = new QLineEdit();
	_searchEdit->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
	_searchEdit->hide();
	_searchEdit->setPlaceholderText("search...");
	_searchEdit->setFont(font1);
	_searchEdit->setFixedWidth(250);
	_searchEdit->setStyleSheet("padding-right:30px;");
	_searchEdit->installEventFilter(this);
	_searchEdit->move(mapToGlobal(pos()));

	connect(_searchEdit, SIGNAL(returnPressed()), this, SLOT(doSearch()));
	connect(_searchEdit, SIGNAL(textEdited(const QString &)), this, SLOT(doSearch(const QString &)));


	_controler = 0;
}

//-----------------------------------------------------------------------
void StreamView::setControler(StreamControler * controler)
{
	_controler = controler;
	if ( _controler != 0 )
	{
		((StreamScene *) scene())->setVisibleArea(getVisibleViewportRect());
	}
}

//-----------------------------------------------------------------------
void StreamView::addObserver(IStreamViewObserver * observer)
{
	_observers.push_back(observer);
}

//-----------------------------------------------------------------------
void StreamView::externalUpdateSceneRect()
{
	if ( scene() )
	{
		((StreamScene *) scene())->setVisibleArea(getVisibleViewportRect());
	}
}


//-----------------------------------------------------------------------
void StreamView::resizeEvent(QResizeEvent *event)
{

	_searchEdit->move(mapToGlobal(pos()).x() + width() - _searchEdit->width() - 20, mapToGlobal(pos()).y() - 20);


	if ( scene() )
	{
		QRectF lsceneRect = scene()->sceneRect();
		if ( lsceneRect.width() < (qreal) event->size().width() )
			lsceneRect.setWidth((qreal) event->size().width());
		if ( lsceneRect.height() < (qreal) event->size().height() )
			lsceneRect.setHeight((qreal) event->size().height());
		scene()->setSceneRect(lsceneRect);
		((StreamScene *) scene())->setVisibleArea(getVisibleViewportRect());
	}
	QGraphicsView::resizeEvent(event);
}

//-----------------------------------------------------------------------
void StreamView::dragEnterEvent(QDragEnterEvent * event)
{
	if ( event->mimeData()->hasFormat("application/mod") )
		event->accept();
}

//-----------------------------------------------------------------------
void StreamView::dragMoveEvent(QDragMoveEvent * event)
{
	if ( event->mimeData()->hasFormat("application/mod") )
		event->accept();
}

//-----------------------------------------------------------------------
void StreamView::dragLeaveEvent(QDragLeaveEvent * event)
{
	QGraphicsView::dragLeaveEvent(event);
}

//-----------------------------------------------------------------------
void StreamView::dropEvent(QDropEvent * event)
{
	//qDebug() << event->mimeData()->formats();
	if ( event->mimeData()->hasFormat("application/mod") )
	{
		_controler->addComponent(QString(event->mimeData()->data("application/mod")),
								 mapToScene(event->pos()));
	}
}


//-----------------------------------------------------------------------
void StreamView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy(dx, dy);
	if ( scene() )
	{
		((StreamScene *) scene())->setVisibleArea(getVisibleViewportRect());
	}
}

//-----------------------------------------------------------------------
QPointF StreamView::getLastMousePos()
{
	return _lastMousePos;
}

//-----------------------------------------------------------------------
QRectF StreamView::getVisibleViewportRect()
{
	QPointF topLeft = mapToScene(0, 0);
	QPointF bottomRight = mapToScene(viewport()->width() - 1, viewport()->height() - 1);
	return QRectF(topLeft, bottomRight);
}

//-----------------------------------------------------------------------
void StreamView::displayContextualMenu(const QPoint & pos)
{
	QMenu * menu = _controler->getContextualMenu(mapToScene(pos));
	if ( menu != 0 )
	{
		menu->exec(mapToGlobal(pos));
	}
}

//-----------------------------------------------------------------------
void StreamView::mouseMoveEvent(QMouseEvent * e)
{
	static QPoint _last_pos;
	_lastMousePos = mapToScene(e->pos());
	if ( e->buttons() & Qt::MidButton )
	{
		QScrollBar *hBar = horizontalScrollBar();
		QScrollBar *vBar = verticalScrollBar();
		QPoint delta = e->pos() - _last_pos;
		hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
		vBar->setValue(vBar->value() - delta.y());
		_last_pos = e->pos();
		e->accept();
		return;
	}
	_last_pos = e->pos();
	//Implementation par defaut
	QGraphicsView::mouseMoveEvent(e);
}

//-----------------------------------------------------------------------
void StreamView::mousePressEvent(QMouseEvent * e)
{
	_lastMousePos = mapToScene(e->pos());
	if ( e->buttons() & Qt::MidButton )
	{
		setCursor(QCursor(Qt::PointingHandCursor));

		e->accept();
		return;
	}

	if ( e->buttons().testFlag(Qt::RightButton) )
	{

		//Right click on component without left click
		ComponentGraphicItem* pItemUnderMouse = dynamic_cast<ComponentGraphicItem*>(itemAt(e->pos()));
		if ( pItemUnderMouse )
		{
			if ( !(e->modifiers() & Qt::ControlModifier) )
			{
				scene()->clearSelection();
			}
			pItemUnderMouse->setSelected(true);
		}

		displayContextualMenu(e->pos());
		e->accept();
		return;
	}
	//Implementation par defaut
	QGraphicsView::mousePressEvent(e);


}

//-----------------------------------------------------------------------
void StreamView::mouseReleaseEvent(QMouseEvent * e)
{
	_lastMousePos = mapToScene(e->pos());
	if ( cursor().shape() == Qt::PointingHandCursor )
	{
		setCursor(QCursor(Qt::ArrowCursor));
		e->accept();
		return;
	}
	//Implementation par defaut
	QGraphicsView::mouseReleaseEvent(e);
}

//-----------------------------------------------------------------------
void StreamView::wheelEvent(QWheelEvent *event)
{
	if ( event->modifiers() & Qt::ControlModifier )
	{
		scaleView(pow((double) 2, -event->delta() / 240.0));
		event->accept();
		return;
	}
	QGraphicsView::wheelEvent(event);
}

//-----------------------------------------------------------------------
void StreamView::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if ( factor < 0.3 || factor > 10 )
		return;

	_scaleFactor *= scaleFactor;

	scale(scaleFactor, scaleFactor);

}

//-----------------------------------------------------------------------
void StreamView::paintEvent(QPaintEvent * event)
{
	QGraphicsView::paintEvent(event);
	for ( int i = 0; i < _observers.count(); i++ )
	{
		_observers[i]->viewHasChanged();
	}
}

//-----------------------------------------------------------------------
void StreamView::mouseDoubleClickEvent(QMouseEvent * event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
	if ( scene()->selectedItems().isEmpty() )
	{
		MainWindow *parentMainWindow = dynamic_cast<MainWindow *>(parentWidget());
		if ( parentMainWindow != 0 )
		{
			parentMainWindow->displayStreamSourceOpener();
		}
	}
}

//-----------------------------------------------------------------------
void StreamView::clearAndHideSearchBox()
{
	_searchEdit->hide();
	_searchEdit->setText("");

	//Reset all states
	QMatrix mat = matrix();
	mat.reset();
	setMatrix(mat);
	QTransform trans = transform();
	trans.reset();
	setTransform(trans);

	centerOn(_previousCenter);
	_scaleFactor = _previousScaleFactor;
	scale(_scaleFactor, _scaleFactor);
}

//-----------------------------------------------------------------------
void StreamView::showSearchBox()
{
	_searchEdit->show();
	QTimer::singleShot(0, _searchEdit, SLOT(setFocus()));

	_previousCenter = mapToScene(viewport()->rect()).boundingRect().center();
	_previousScaleFactor = _scaleFactor;
}

//-----------------------------------------------------------------------
void StreamView::doSearch()
{
	foreach(QGraphicsItem * item, scene()->items())
	{
		ComponentGraphicItem * gItem = dynamic_cast<ComponentGraphicItem*>(item);
		if ( gItem &&  gItem->getComponent() )
		{
			if ( gItem->getComponent()->GetName().contains(_searchEdit->text(), Qt::CaseInsensitive) )
			{
				gItem->setSelected(true);
			}
			else
			{
				gItem->setSelected(false);
			}
		}
	}

	//Center and zoom to item
	if ( !scene()->selectedItems().isEmpty() )
	{
		QRectF allItemsBounding;

		foreach(QGraphicsItem * item, scene()->selectedItems())
		{
			QRectF tmp = item->boundingRect();
			tmp.moveTo(item->pos().x() - item->boundingRect().size().width() / 2, item->pos().y());
			allItemsBounding = allItemsBounding.united(tmp);
		}
		QRectF newRect = allItemsBounding;
		newRect.setTopLeft(QPointF(newRect.topLeft().x() - 50, newRect.topLeft().y() - 50));
		newRect.setBottomRight(QPointF(newRect.bottomRight().x() + 50, newRect.bottomRight().y() + 50));
		allItemsBounding = allItemsBounding.united(newRect);

		if ( scene() && !scene()->items().contains(test) )
			scene()->addItem(test);

		test->setRect(allItemsBounding);
		centerOn(allItemsBounding.center());
		fitInView(allItemsBounding, Qt::KeepAspectRatio);
		_scaleFactor = transform().m11();

	}
}

//-----------------------------------------------------------------------
void StreamView::doSearch(const QString &)
{
	doSearch();

}

//-----------------------------------------------------------------------
// void StreamView::keyPressEvent( QKeyEvent *event )
// {
// 	qDebug() << event->key();
// 	if(event->key() == Qt::Key_Delete ||event->key() == Qt::Key_F10)
// 	{
// 		MenuManager::getInstance()->onRemove();
// 	}
// }

//-----------------------------------------------------------------------
bool StreamView::eventFilter(QObject *obj, QEvent *event)
{
	if ( obj == _searchEdit && event->type() == QEvent::Close )
	{
		clearAndHideSearchBox();
		return QGraphicsView::eventFilter(obj, event);
	}

	return QGraphicsView::eventFilter(obj, event);
}

