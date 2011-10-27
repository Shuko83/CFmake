/**
@file VerticalMultiAnchorableContainer.cpp
@brief Container vertical d'ancrable
@author F.Bighelli
 */

#include "VerticalMultiAnchorableContainer.h"
 

/** @brief Constructor */
VerticalMultiAnchorableContainer::VerticalMultiAnchorableContainer(QWidget * parent,TAnchorPosition aPosition):
    QWidget(parent),
    _aPosition(aPosition) {
    layout=new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setMinimumSize(22,22);
    resize(QSize(22, 22));
}
/** @brief ajout d'un widget */
void VerticalMultiAnchorableContainer::addWidget(QWidget * w) {
    w->setParent(this);
    layout->addWidget(w);
    IAnchorable * a=dynamic_cast<IAnchorable *>(w);
    if (a!=0) {
        a->addObserver(this);
    }
}

//--------------------------------------------------------------------------
// IAnchorable
//--------------------------------------------------------------------------
/** @brief Position d'ancrage */
TAnchorPosition VerticalMultiAnchorableContainer::getAnchorPosition(){
    return _aPosition;
}
/** @brief Est extensible verticalement */
bool VerticalMultiAnchorableContainer::isVerticalExtensible(){
    bool isExtensible=false;
    const QObjectList &liste=children();
    for (int i=0;liste.count();i++) {
        if (dynamic_cast<QWidget*>(liste[i])!=0) {
            IAnchorable * a=dynamic_cast<IAnchorable *>(liste[i]);
            if (a!=0) {
                isExtensible=isExtensible || a->isVerticalExtensible(); 
            }
        }
    }
    return isExtensible;
}
/** @brief Est extensible horizontalement */
bool VerticalMultiAnchorableContainer::isHorizontalExtensible(){
    bool isExtensible=false;
    const QObjectList &liste=children();
    for (int i=0;liste.count();i++) {
        if (dynamic_cast<QWidget*>(liste[i])!=0) {
            IAnchorable * a=dynamic_cast<IAnchorable *>(liste[i]);
            if (a!=0) {
                isExtensible=isExtensible || a->isHorizontalExtensible(); 
            }
        }
    }
    return isExtensible;
}
/** @brief ajout d'observer */
void VerticalMultiAnchorableContainer::addObserver(IAnchorableObserver * o){
    _observers.push_back(o);
}
/** @brief suppression d'observer */
void VerticalMultiAnchorableContainer::removeObserver(IAnchorableObserver * o){
    int i=_observers.indexOf(o);
    if (i>0) {
        _observers.erase(_observers.begin()+i);
    }
}

//--------------------------------------------------------------------------
// IAnchorableObserver
//--------------------------------------------------------------------------
/*@brief indique que l'entité a changé */
void VerticalMultiAnchorableContainer::anchorableAsChanged(QWidget */*w*/){
    notifyObservers();
}

/** @brief notification des observers */
void VerticalMultiAnchorableContainer::notifyObservers(){
    QList<IAnchorableObserver *>::iterator it;
    it=_observers.begin();
    while (it!=_observers.end()) {
        (*it)->anchorableAsChanged(this);
        it++;
    }

}