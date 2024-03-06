/*!
 \file _SwEditorQGraphicsView.h
 \brief  implementation QGraphicsView pour l'editeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QWheelEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QPoint>
#include <QGraphicsItem>

#include <math.h>

#include "_SwEditorQGraphicsView.h"

//using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;


_SwEditorQGraphicsView::_SwEditorQGraphicsView( QGraphicsScene * scene, QWidget * parent)
	: QGraphicsView(scene,parent)
{
    _help="<h2>View Controls:</h2>\n";
    _help+="<h5>Zoom</h5>Ctrl+WheelMouse\n";
    _help+="<h5>Pan</h5>Ctrl+WheelButton\n";
    _help+="<h5>Operations</h5>RightClick\n";
    _help+="<h5>Component editing</h5>Alt+Click";
    setDragMode(QGraphicsView::RubberBandDrag);
    setCacheMode(QGraphicsView::CacheBackground);
    //setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setViewportUpdateMode(FullViewportUpdate);
    _internalEvent=false;
}

_SwEditorQGraphicsView::~_SwEditorQGraphicsView()
{
}
/** mise a jour de la scene */
void _SwEditorQGraphicsView::sceneUpdated() {
    connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
}
/* \brief sur changement de selection */
void _SwEditorQGraphicsView::onSelectionChanged() {
    if (!_internalEvent) {
        QList<QGraphicsItem* > gitems=scene()->selectedItems();
        if (gitems.count()>0) {
            ensureVisible(gitems.at(0));
        }
    }
}
/* \brief Evenement pression touche */
void _SwEditorQGraphicsView::keyPressEvent ( QKeyEvent * event ) {
    if (event->matches(QKeySequence::HelpContents)) {
        QMessageBox::information(this,"View Controls",_help); 
        event->accept();
        return;
    }
    //Implementation par defaut
    QGraphicsView::keyPressEvent(event);
}
/* \brief Evenement sourie en deplacement */
void _SwEditorQGraphicsView::mouseMoveEvent ( QMouseEvent * e ) {

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
void _SwEditorQGraphicsView::mousePressEvent ( QMouseEvent * e ){
    _internalEvent=true;

    if (e->buttons() & Qt::MidButton) {
        setCursor(QCursor(Qt::PointingHandCursor));
        
        e->accept();
        return;
    }
    //Implementation par defaut
    QGraphicsView::mousePressEvent(e);

    _internalEvent=false;

}
/* \brief Evenement sourie libere */
void _SwEditorQGraphicsView::mouseReleaseEvent ( QMouseEvent * e ){
    _internalEvent=true;
    if (cursor().shape()==Qt::PointingHandCursor) {
        setCursor(QCursor(Qt::ArrowCursor));
        e->accept();
        return;
    }
    //Implementation par defaut
    QGraphicsView::mouseReleaseEvent(e);
    _internalEvent=false;
}
/* \brief Reception d'un evenement roulette de souris */
/* \brief Reception d'un evenement roulette de souris */
void _SwEditorQGraphicsView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        scaleView(pow((double)2, -event->delta() / 240.0));
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
}
/* \brief Retaillage de la vue */
void _SwEditorQGraphicsView::scaleView(qreal scaleFactor) {
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 5)
        return;

    scale(scaleFactor, scaleFactor);
    
}

