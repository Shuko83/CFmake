/**
@file ViewNavigator.cpp
@brief Navigateur de la vue
@author F.Bighelli
 */

#include "ViewNavigator.h"

#include <QtGlobal>

/** @brief Constructor */
ViewNavigator::ViewNavigator(StreamView *sv,QWidget * parent):QWidget(parent) {
	_streamView=sv;
    _streamView->addObserver(this);
    setAutoFillBackground(false);
    resize(200,200);
    _movingInProgress=false;
    setMouseTracking(true);
}
//--------------------------------------------------------------------------
// IStreamViewObserver
//--------------------------------------------------------------------------
void ViewNavigator::viewHasChanged() {
    repaint();
}
/** @breif event de dessin */
void ViewNavigator::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);

    QGraphicsScene * scene=_streamView->scene();
    if (scene==0) {
        return;
    }
    QRectF sceneRect=scene->sceneRect();
    QSizeF ssize=sceneRect.size();
    ssize.scale(QSizeF(width(),height()),Qt::KeepAspectRatio);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0));
    painter.drawRect(QRectF(0,0,ssize.width(),ssize.height()));

    QPoint topleft=_streamView->mapFromScene(sceneRect.topLeft());
    QPoint bottomRight=_streamView->mapFromScene(sceneRect.bottomRight());
    _streamView->render(&painter,QRectF(0,0,width(),height()),QRect(topleft,bottomRight));

    QRectF rview=_streamView->getVisibleViewportRect();

    QRectF rviewnav;
    rviewnav.setLeft((rview.left()-sceneRect.left())*ssize.width()/sceneRect.width());
    rviewnav.setRight((rview.right()-sceneRect.left())*ssize.width()/sceneRect.width());
    rviewnav.setTop((rview.top()-sceneRect.top())*ssize.height()/sceneRect.height());
    rviewnav.setBottom((rview.bottom()-sceneRect.top())*ssize.height()/sceneRect.height());
    painter.setPen(QColor(Qt::red));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rviewnav);
    _rViewNav=rviewnav;

}
/** @brief sur click de la souris */
void ViewNavigator::mousePressEvent ( QMouseEvent * e ) {
    if (!_movingInProgress) {
		if (_rViewNav.contains(e->localPos())) {
            setCursor(Qt::SizeAllCursor);   
            _movingInProgress=true;
            pstart=e->pos();
            _rView=_streamView->getVisibleViewportRect();
            _hvalue=_streamView->horizontalScrollBar()->sliderPosition();
            _vvalue=_streamView->verticalScrollBar()->sliderPosition();
        } 
    }
    
}
/* \brief Evenement sourie en deplacement */
void ViewNavigator::mouseMoveEvent ( QMouseEvent * e ){
    if (!_movingInProgress) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
		if (_rViewNav.contains(e->localPos())) {
#else
		if (_rViewNav.contains(e->pos())) {
#endif 
            setCursor(Qt::SizeAllCursor);    
        } else {
            setCursor(Qt::ArrowCursor); 
        }
    } else {
        QPointF pcurrent=e->pos()-pstart;
        QRectF sceneRect=_streamView->scene()->sceneRect();
        QSizeF ssize=sceneRect.size();
        ssize.setWidth(ssize.width()-_streamView->horizontalScrollBar()->pageStep());
        ssize.setHeight(ssize.height()-_streamView->verticalScrollBar()->pageStep());
        ssize.scale(QSizeF(width(),height()),Qt::KeepAspectRatio);
        int hmin=_streamView->horizontalScrollBar()->minimum();
        int hmax=_streamView->horizontalScrollBar()->maximum();
        int hnewvalue=_hvalue+pcurrent.x()*(hmax-hmin+_streamView->horizontalScrollBar()->pageStep())/ssize.width();
        if (hnewvalue<hmin) hnewvalue=hmin;
        if (hnewvalue>hmax) hnewvalue=hmax;
        _streamView->horizontalScrollBar()->setSliderPosition(hnewvalue);
        int vmin=_streamView->verticalScrollBar()->minimum();
        int vmax=_streamView->verticalScrollBar()->maximum();
        int vnewvalue=_vvalue+pcurrent.y()*(vmax-vmin+_streamView->verticalScrollBar()->pageStep())/ssize.height();
        if (vnewvalue<vmin) vnewvalue=vmin;
        if (vnewvalue>vmax) vnewvalue=vmax;
        _streamView->verticalScrollBar()->setSliderPosition(vnewvalue);
    }
}
/* \brief Evenement sourie libere */
void ViewNavigator::mouseReleaseEvent ( QMouseEvent * /*e*/ ){
    if (_movingInProgress) {
        setCursor(Qt::ArrowCursor); 
        _movingInProgress=false;

    }
}

