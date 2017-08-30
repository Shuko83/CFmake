/**
@file StreamView.h
@brief Vue Stream
@author F.Bighelli
 */

#ifndef _STREAMVIEW_H
#define _STREAMVIEW_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QGLWidget>
#include <QGraphicsView>
#include <QLineEdit>
#include "IStreamViewObserver.h"

class StreamControler;
/**
@class StreamView
@brief Vue Stream
*/
class StreamView : public QGraphicsView{
    Q_OBJECT
public:

	QLineEdit *_searchEdit;

	/** @brief Constructor */
	StreamView(QWidget * parent=0);
    /** @brief Definition du controleur */
    void setControler(StreamControler * controler);
    /** @brief mise a jour externe de la scene */
    void externalUpdateSceneRect();
    /** @brief ajoute un observer */
    void addObserver(IStreamViewObserver * observer);
    /** @brief Definition du rectangle visible de la scene */
    QRectF getVisibleViewportRect();
    /** @brief mouvement du viewport */
    void scrollContentsBy (int dx, int dy );
    /** @brief renvoie la derneire position de la sourie  dans la scene */
    QPointF getLastMousePos();
protected:
    /** @brief evenement de retaillage */
    void resizeEvent(QResizeEvent *event);
    /** @brief evenement d'entree de drag */
    void dragEnterEvent ( QDragEnterEvent * event );
    /** @brief evenement de move de drag */
    void dragMoveEvent ( QDragMoveEvent * event );
    /** @brief evenement de sortie de drag */
    void dragLeaveEvent ( QDragLeaveEvent * event ); 
    /** @brief evenement de drop */
    void dropEvent( QDropEvent * event ); 
    /** @brief evenement de repaint */
    void paintEvent ( QPaintEvent * event ); 
    /** @brief evenement de double click */
    void mouseDoubleClickEvent ( QMouseEvent * event ); 

	bool eventFilter(QObject *obj, QEvent *event);

public slots:
    /** @brief sur demande du menu contextuel */
    void displayContextualMenu(const QPoint & pos);

	void clearAndHideSearchBox();
	void showSearchBox();
	void doSearch();
	void doSearch(const QString &);

protected:
    /** @brief sur click de la souris */
    virtual void mousePressEvent ( QMouseEvent * e );
    /* \brief Evenement sourie en deplacement */
    virtual void mouseMoveEvent ( QMouseEvent * e );
    /* \brief Evenement sourie libere */
    virtual void mouseReleaseEvent ( QMouseEvent * e );
    /* \brief Reception d'un evenement roulette de souris */
    void wheelEvent(QWheelEvent *event);
    /* \brief Retaillage de la vue */
	void scaleView(qreal scaleFactor);
	/* \brief Deplacement de la vue */
	void scrollView(QPoint dp);
private:
    /** @brief Controler hote */
    StreamControler * _controler;
    /** @brief Liste des observers */
    QList<IStreamViewObserver *> _observers;
    /** @brief QPointF last mouse position */
    QPointF _lastMousePos;

	QShortcut *_searchShortCut;
	QPointF _previousCenter;
	qreal _previousScaleFactor;
	qreal _scaleFactor;

	QGraphicsRectItem * test;
};

#endif
