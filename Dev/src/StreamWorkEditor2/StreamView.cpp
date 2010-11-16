/**
@file StreamView.cpp
@brief Vue Stream
@author F.Bighelli
 */

#include "StreamView.h"
#include "StreamControler.h"
 

/** @brief Constructor */
StreamView::StreamView(QWidget * parent):QGraphicsView(parent) {
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setStyleSheet("QGraphicsView { background:black }");
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true);
    //connect(this,SIGNAL(customContextMenuRequested ( const QPoint & )),
    //        this,SLOT(displayContextualMenu(const QPoint &)));
    //setContextMenuPolicy(Qt::CustomContextMenu);
    _controler=0;
}
/** @brief Definition du controleur */
void StreamView::setControler(StreamControler * controler) {
    _controler=controler;
    if (_controler!=0) {
        ((StreamScene *)scene())->setVisibleArea(getVisibleViewportRect());
    }
}
/** @brief ajoute un observer */
void StreamView::addObserver(IStreamViewObserver * observer) {
    _observers.push_back(observer);
}
/** @brief mise a jour externe de la scene */
void StreamView::externalUpdateSceneRect(){
    if (scene()) {
        ((StreamScene *)scene())->setVisibleArea(getVisibleViewportRect());
    }
}

/** @brief evenement de retaillage */
void StreamView::resizeEvent(QResizeEvent *event){
    if (scene()) {
        QRectF lsceneRect=scene()->sceneRect();
        if (lsceneRect.width()<(qreal)event->size().width())
            lsceneRect.setWidth((qreal)event->size().width());
        if (lsceneRect.height()<(qreal)event->size().height())
            lsceneRect.setHeight((qreal)event->size().height());
        scene()->setSceneRect(lsceneRect);
        ((StreamScene *)scene())->setVisibleArea(getVisibleViewportRect());
    }
    QGraphicsView::resizeEvent(event);
}
/** @brief evenement d'entree de drag */
void StreamView::dragEnterEvent ( QDragEnterEvent * event ) {
    if (event->mimeData()->hasFormat("application/mod"))
        event->accept();
}
/** @brief evenement de move de drag */
void StreamView::dragMoveEvent ( QDragMoveEvent * event ) {
    if (event->mimeData()->hasFormat("application/mod"))
        event->accept();
}
/** @brief evenement de sortie de drag */
void StreamView::dragLeaveEvent ( QDragLeaveEvent * event ) {
    QGraphicsView::dragLeaveEvent(event);
}
/** @brief evenement de drop */
void StreamView::dropEvent( QDropEvent * event ) {
    qDebug() << event->mimeData()->formats();
    if (event->mimeData()->hasFormat("application/mod")) {
        _controler->addComponent(QString(event->mimeData()->data("application/mod")),
                                 mapToScene(event->pos()));
    }
}

/** @brief mouvement du viewport */
void StreamView::scrollContentsBy (int dx, int dy ) {
    QGraphicsView::scrollContentsBy(dx,dy);
    if (scene()) {
        ((StreamScene *)scene())->setVisibleArea(getVisibleViewportRect());
    }
}
/** @brief renvoie la derneire position de la sourie  dans la scene */
QPointF StreamView::getLastMousePos() {
    return _lastMousePos;
}
/** @brief Definition du rectangle visible de la scene */
QRectF StreamView::getVisibleViewportRect() {
    QPointF topLeft     = mapToScene(0,0);
    QPointF bottomRight = mapToScene( viewport()->width() - 1, viewport()->height() - 1 );
    return QRectF( topLeft, bottomRight );
}
/** @brief sur demande du menu contextuel */
void StreamView::displayContextualMenu(const QPoint & pos) {
    QMenu * menu = _controler->getContextualMenu(mapToScene(pos));
    if (menu!=0) {
        menu->exec(mapToGlobal(pos));
    }
}
/* \brief Evenement sourie en deplacement */
void StreamView::mouseMoveEvent ( QMouseEvent * e ) {
    static QPoint _last_pos;
    _lastMousePos=mapToScene(e->pos());
    if (e->buttons() & Qt::MidButton) {
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = e->pos() - _last_pos;
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());
        _last_pos=e->pos();
        e->accept();
        return;
    }
    _last_pos=e->pos();
    //Implementation par defaut
    QGraphicsView::mouseMoveEvent(e);
}
/* \brief Evenement sourie presse */
void StreamView::mousePressEvent ( QMouseEvent * e ){
    _lastMousePos=mapToScene(e->pos());
    if (e->buttons() & Qt::MidButton) {
        setCursor(QCursor(Qt::PointingHandCursor));
        
        e->accept();
        return;
    }
    if (e->buttons().testFlag(Qt::RightButton)) {
        displayContextualMenu(e->pos());
        e->accept();
        return;
    }
    //Implementation par defaut
    QGraphicsView::mousePressEvent(e);


}
/* \brief Evenement sourie libere */
void StreamView::mouseReleaseEvent ( QMouseEvent * e ){
    _lastMousePos=mapToScene(e->pos());
    if (cursor().shape()==Qt::PointingHandCursor) {
        setCursor(QCursor(Qt::ArrowCursor));
        e->accept();
        return;
    }
    //Implementation par defaut
    QGraphicsView::mouseReleaseEvent(e);
}
/* \brief Reception d'un evenement roulette de souris */
/* \brief Reception d'un evenement roulette de souris */
void StreamView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        scaleView(pow((double)2, -event->delta() / 240.0));
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
}
/* \brief Retaillage de la vue */
void StreamView::scaleView(qreal scaleFactor) {
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 10)
        return;

    scale(scaleFactor, scaleFactor);
    
}
/** @brief evenement de repaint */
void StreamView::paintEvent ( QPaintEvent * event ) {
    QGraphicsView::paintEvent(event);
    for(int i=0;i<_observers.count();i++) {
        _observers[i]->viewHasChanged();
    }
}

