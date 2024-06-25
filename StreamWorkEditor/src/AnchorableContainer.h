/**
@file AnchorableContainer.h
@brief Widgte container ancrable
@author F.Bighelli
 */

#ifndef _ANCHORABLECONTAINER_H
#define _ANCHORABLECONTAINER_H

#include <QtCore>
#include "IAnchorable.h"

/**
@class AnchorableContainer
@brief Widgte container ancrable
*/
class AnchorableContainer : public QWidget, public IAnchorable {
    Q_OBJECT
public:
	/** @brief Constructor */
	AnchorableContainer(QWidget * parent,TAnchorPosition aPosition,QIcon icon,QString title);
    /** @brief definition du widgetCentral */
    void setCentralWidget(QWidget * w,bool vExt,bool hExt);
    //--------------------------------------------------------------------------
    // IAnchorable
    //--------------------------------------------------------------------------
    /** @brief Position d'ancrage */
    virtual TAnchorPosition getAnchorPosition();
    /** @brief Est extensible verticalement */
    virtual bool isVerticalExtensible();
    /** @brief Est extensible horizontalement */
    virtual bool isHorizontalExtensible();
    /** @brief ajout d'observer */
    virtual void addObserver(IAnchorableObserver * o);
    /** @brief suppression d'observer */
    virtual void removeObserver(IAnchorableObserver * o);

    /** @brief montrer l'anchorable expended */
    virtual void showExpended();

protected:
    /** @brief notification des observers */
    void notifyObservers();
    /** @brief event acces */
    virtual void enterEvent ( QEvent * event );
    /** @brief event acces */
    virtual void leaveEvent ( QEvent * event );

public slots:
    /** @brief sur expension */
    void onExpand(bool);
    /** @brief sur reduction */
    void onReduce();
private:
    /** @brief Position d'ancrage */
    TAnchorPosition _aPosition;
    /** @brief Capacité d'extension */
    bool _isHorizontalExtensible;
    bool _isVerticalExtensible;
    /** @brief Lister des observers */
    QList<IAnchorableObserver *> _observers;
    /** @brief layout */
    QVBoxLayout * layout;
    /** @brief Icon */
    QIcon _icon;
    /** @brief label d'entete */
    QLabel * _labelHeader;
    /** @brief button */
    QPushButton * _iconButton;
    /** @brief hosted widget */
    QWidget * _widget;
    /** @brief Timer de fermeture */
    QTimer * _closeTimer;

};

#endif
