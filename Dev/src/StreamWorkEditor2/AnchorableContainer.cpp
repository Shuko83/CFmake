/**
@file AnchorableContainer.cpp
@brief Widgte container ancrable
@author F.Bighelli
 */

#include "AnchorableContainer.h"
 

/** @brief Constructor */
AnchorableContainer::AnchorableContainer(QWidget * parent,
                                         TAnchorPosition aPosition,
                                         QIcon icon,QString title):
    QWidget(parent),
    _aPosition(aPosition),
    _icon(icon){
    _widget=0;
    _isHorizontalExtensible=false;
    _isVerticalExtensible=false;
    _iconButton=new QPushButton(icon,"",this);
    _iconButton->setFlat(true);
    connect(_iconButton,SIGNAL(clicked (bool)),this,SLOT(onExpand(bool)));
    _labelHeader=new QLabel(title,this);
    _labelHeader->setVisible(false);
    _labelHeader->setAlignment(Qt::AlignHCenter);
    layout=new QVBoxLayout(this);
    layout->addWidget(_iconButton);
    layout->addWidget(_labelHeader);
    layout->setMargin(2);
    layout->setSpacing(2);
    resize(QSize(22, 22));
    _closeTimer=new QTimer(this);
    _closeTimer->setSingleShot(true);
    connect(_closeTimer,SIGNAL(timeout ()),this,SLOT(onReduce()));

}
/** @brief definition du widgetCentral */
void AnchorableContainer::setCentralWidget(QWidget * w,bool vExt,bool hExt) {
    _widget=w;
    _widget->setVisible(false);
    layout->addWidget(_widget);
    _isHorizontalExtensible=hExt;
    _isVerticalExtensible=vExt;
}

//--------------------------------------------------------------------------
// IAnchorable
//--------------------------------------------------------------------------
/** @brief Position d'ancrage */
TAnchorPosition AnchorableContainer::getAnchorPosition() {
    return _aPosition;
}
/** @brief Est extensible verticalement */
bool AnchorableContainer::isVerticalExtensible(){
    return _iconButton->isVisible()?false:_isVerticalExtensible;
}
/** @brief Est extensible horizontalement */
bool AnchorableContainer::isHorizontalExtensible(){
    return _iconButton->isVisible()?false:_isHorizontalExtensible;
}
/** @brief ajout d'observer */
void AnchorableContainer::addObserver(IAnchorableObserver *o){
    _observers.push_back(o);
}
/** @brief suppression d'observer */
void AnchorableContainer::removeObserver(IAnchorableObserver *o){
    int i=_observers.indexOf(o);
    if (i>0) {
        _observers.erase(_observers.begin()+i);
    }
}
/** @brief montrer l'anchorable expended */
void AnchorableContainer::showExpended() {
    onExpand(true);
}
/** @brief notification des observers */
void AnchorableContainer::notifyObservers() {
    QList<IAnchorableObserver *>::iterator it;
    it=_observers.begin();
    while (it!=_observers.end()) {
        (*it)->anchorableAsChanged(this);
        it++;
    }
}    
/** @brief event acces */
void AnchorableContainer::enterEvent ( QEvent * event ) {
    if (_closeTimer->isActive()) {
        _closeTimer->stop();
    }
}
/** @brief event acces */
void AnchorableContainer::leaveEvent ( QEvent * event ){
    if (!_iconButton->isVisible()) {
        _closeTimer->start(2000); 
    }
}

/** @brief sur expension */
void AnchorableContainer::onExpand(bool) {
    if (_widget!=0) {
        _iconButton->setVisible(false);
        _labelHeader->setVisible(true);
        _widget->setVisible(true);
        notifyObservers();
    }
}
/** @brief sur reduction */
void AnchorableContainer::onReduce() {
    _labelHeader->setVisible(false);
    _widget->setVisible(false);
    _iconButton->setVisible(true);
    resize(QSize(22, 22));
    notifyObservers();
}
