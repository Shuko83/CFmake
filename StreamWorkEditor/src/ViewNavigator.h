/**
@file ViewNavigator.h
@brief Navigateur de la vue
@author F.Bighelli
 */

#ifndef _VIEWNAVIGATOR_H
#define _VIEWNAVIGATOR_H

#include <QtWidgets>

#include "IStreamViewObserver.h"
#include "StreamView.h"
/**
@class ViewNavigator
@brief Navigateur de la vue
*/
class ViewNavigator : public QWidget, public IStreamViewObserver{
    Q_OBJECT
public:
	/** @brief Constructor */
	ViewNavigator(StreamView *sv,QWidget * parent = 0);
    //--------------------------------------------------------------------------
    // IStreamViewObserver
    //--------------------------------------------------------------------------
    virtual void viewHasChanged();

protected:
    /** @breif event de dessin */
    void paintEvent(QPaintEvent *event);
protected:
    /** @brief sur click de la souris */
    virtual void mousePressEvent ( QMouseEvent * e );
    /* \brief Evenement sourie en deplacement */
    virtual void mouseMoveEvent ( QMouseEvent * e );
    /* \brief Evenement sourie libere */
    virtual void mouseReleaseEvent ( QMouseEvent * e );
private:
    /** @breif view */
    StreamView * _streamView;
    /** @brief Point de demarrage */
    QPointF pstart;
    /** @brief dernier rectangle de la vue avant mouvement */
    QRectF _rView;
    /** @brief dernier rectangle de la vue dans la nav */
    QRectF _rViewNav;
    /** @brief deplacement de la vue en cours */
    bool _movingInProgress;
    /** @brief gestion des scrollbar */
    int _hvalue;
    int _vvalue;
};

#endif
