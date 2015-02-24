/**
@file VerticalMultiAnchorableContainer.h
@brief Container vertical d'ancrable
@author F.Bighelli
 */

#ifndef _VERTICALMULTIANCHORABLECONTAINER_H
#define _VERTICALMULTIANCHORABLECONTAINER_H

#include <QtCore>
#include "IAnchorable.h"

/**
@class VerticalMultiAnchorableContainer
@brief Container vertical d'ancrable
*/
class VerticalMultiAnchorableContainer : public QWidget, public IAnchorable , public IAnchorableObserver{
    Q_OBJECT
public:
	/** @brief Constructor */
	VerticalMultiAnchorableContainer(QWidget * parent,TAnchorPosition aPosition);
    /** @brief ajout d'un widget */
    void addWidget(QWidget * w);
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

    //--------------------------------------------------------------------------
    // IAnchorableObserver
    //--------------------------------------------------------------------------
    /*@brief indique que l'entité a changé */
    virtual void anchorableAsChanged(QWidget *w);

protected:
    /** @brief notification des observers */
    void notifyObservers();

private:
    /** @brief Position d'ancrage */
    TAnchorPosition _aPosition;
    /** @brief Lister des observers */
    QList<IAnchorableObserver *> _observers;
    /** @brief layout */
    QVBoxLayout * layout;

};

#endif
