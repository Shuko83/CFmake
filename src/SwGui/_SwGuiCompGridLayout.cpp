/**
@file _SwGuiCompGridLayout.cpp
@brief ComposantGridLayout
@author Big
 */

#include "_SwGuiCompGridLayout.h"
 

/** @brief Constructor */
_SwGuiCompGridLayout::_SwGuiCompGridLayout():Component() {
    _layout=0;
    _widgets_count=0;
    _layouts_count=0;
    _horizontalSpacing=6;
    _verticalSpacing=0;
    _columnStretch="0";
    _rowStretch="0";
    _leftMargin=0;
    _topMargin=0;
    _rightMargin=0;
    _bottomMargin=0;

}
/** @brief Destructor */
_SwGuiCompGridLayout::~_SwGuiCompGridLayout() {
  //TO DO destruction
    //Desenregistrement des services

    if (_layout!=0) {
        delete _layout;
		_layout = 0;
    }
}
/*! \brief Renvoie le Layout
\return le Layout */
QLayout & _SwGuiCompGridLayout::GetLayout(){
    if (_layout==0) {
        _layout=new QGridLayout();
        _layout->setHorizontalSpacing(_horizontalSpacing);
        _layout->setVerticalSpacing(_verticalSpacing);
        for(int i=0;i<_widgets.count();i++) {
            _widgets[i]->setGridLayout(_layout); 
        }
        for(int i=0;i<_layouts.count();i++) {
            _layouts[i]->setGridLayout(_layout); 
        }
        updateStretchToLayoutStretch();
        updateLayoutStretchToStretch();
        _layout->setContentsMargins(_leftMargin,_topMargin,_rightMargin,_bottomMargin);

    }
    return (*_layout);
}
/*! \brief Liberation du layout (doit etre appele lors de la liberation du layout)
Attention, le layout est (et doit) etre detruit par cette methode*/
void _SwGuiCompGridLayout::LiberateLayout(){
    updateLayoutStretchToStretch();
    for(int i=0;i<_widgets.count();i++) {
        _widgets[i]->setGridLayout(0); 
    }
    for(int i=0;i<_layouts.count();i++) {
        _layouts[i]->setGridLayout(0); 
    }
    delete _layout;
    _layout=NULL;    
}
/*! \brief Initialisation du composant */
void _SwGuiCompGridLayout::initializeComponent() throw(SwException) {
    getIProviderService().RegisterProvidedInterface<ISwLayout>("GridLayout",(ISwLayout *)this);
    getPropertiesService().CreatePropertiesForQObject(this,QString(),true);
}
/*! \brief evenement avant changement de la disponibilité de l'interface
    \note A Surcharger*/
void _SwGuiCompGridLayout::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
    for(int i=0;i<_widgets.count();i++) {
        _widgets[i]->eventBeforeInterfaceAvailability(interface_name,provider_host);    
    }
    for(int i=0;i<_layouts.count();i++) {
        _layouts[i]->eventBeforeInterfaceAvailability(interface_name,provider_host);    
    }
}
/*! \brief evenement apres changement de la disponibilité de l'interface
    \note A Surcharger*/
void _SwGuiCompGridLayout::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
    for(int i=0;i<_widgets.count();i++) {
        _widgets[i]->eventAfterInterfaceAvailability(interface_name,provider_host);    
    }
    for(int i=0;i<_layouts.count();i++) {
        _layouts[i]->eventAfterInterfaceAvailability(interface_name,provider_host);    
    }
    updateStretchToLayoutStretch();
}

//-------------------------------------------------------------------------
//Getter setter property
//-------------------------------------------------------------------------
unsigned int _SwGuiCompGridLayout::getNbWidgets(){
    return _widgets_count;
}
void _SwGuiCompGridLayout::setNbWidgets(unsigned int nbWidgets){
    if (nbWidgets>_widgets_count) {
        while (_widgets.count()!=nbWidgets) {
            _SwGuiCompGridLayoutCell * cell=new _SwGuiCompGridLayoutCell(_widgets.count(),&getPropertiesService(),&getIConsumerService(),false);
            if (_layout!=0) {
                cell->setGridLayout(_layout);   
            }
            _widgets.push_back(cell);
        }
    } else if (nbWidgets<_widgets_count) {
        while (_widgets.count()!=nbWidgets) {
            _SwGuiCompGridLayoutCell * cell=_widgets.back();
            _widgets.pop_back();
            if (_layout!=0) {
                cell->setGridLayout(0);   
            }
            delete cell;
        }
    }
    _widgets_count=nbWidgets;
}
unsigned int _SwGuiCompGridLayout::getNbLayouts(){
    return _layouts_count;
}
void _SwGuiCompGridLayout::setNbLayouts(unsigned int nbLayouts){
    if (nbLayouts>_layouts_count) {
        while (_layouts.count()!=nbLayouts) {
            _SwGuiCompGridLayoutCell * cell=new _SwGuiCompGridLayoutCell(_layouts.count(),&getPropertiesService(),&getIConsumerService(),true);
            if (_layout!=0) {
                cell->setGridLayout(_layout);   
            }
            _layouts.push_back(cell);
        }
    } else if (nbLayouts<_layouts_count) {
        while (_layouts.count()!=nbLayouts) {
            _SwGuiCompGridLayoutCell * cell=_layouts.back();
            _layouts.pop_back();
            if (_layout!=0) {
                cell->setGridLayout(0);   
            }
            delete cell;
        }
    }
    _layouts_count=nbLayouts;
}
int _SwGuiCompGridLayout::getHorizontalSpacing(){
    return _horizontalSpacing;
}
void _SwGuiCompGridLayout::setHorizontalSpacing(unsigned int horizontalSpacing){
    _horizontalSpacing=horizontalSpacing;
    if (_layout!=0) {
        _layout->setHorizontalSpacing(_horizontalSpacing);
    }

}
int _SwGuiCompGridLayout::getVerticalSpacing(){
    return _verticalSpacing;
}
void _SwGuiCompGridLayout::setVerticalSpacing(unsigned int verticalSpacing){
    _verticalSpacing=verticalSpacing;
    if (_layout!=0) {
        _layout->setVerticalSpacing(_verticalSpacing);
    }
}
QString _SwGuiCompGridLayout::getColumnStretch() {
    updateLayoutStretchToStretch();
    return _columnStretch;
}
void _SwGuiCompGridLayout::setColumnStretch(QString columnStretch){
    _columnStretch=columnStretch;
    updateStretchToLayoutStretch();
}
QString _SwGuiCompGridLayout::getRowStretch(){
    updateLayoutStretchToStretch();
    return _rowStretch;
}
void _SwGuiCompGridLayout::setRowStretch(QString rowStretch){
    _rowStretch=rowStretch;
    updateStretchToLayoutStretch();
}
int _SwGuiCompGridLayout::getLeftMargin() {
    return _leftMargin;
}
void _SwGuiCompGridLayout::setLeftMargin(int value){
    _leftMargin=value;
    updateMargins();
}
int _SwGuiCompGridLayout::getTopMargin(){
    return _topMargin;
}
void _SwGuiCompGridLayout::setTopMargin(int value){
    _topMargin=value;
    updateMargins();
}
int _SwGuiCompGridLayout::getRightMargin(){
    return _rightMargin;
}
void _SwGuiCompGridLayout::setRightMargin(int value){
    _rightMargin=value;
    updateMargins();
}
int _SwGuiCompGridLayout::getBottomMargin(){
    return _bottomMargin;
}
void _SwGuiCompGridLayout::setBottomMargin(int value){
    _bottomMargin=value;
    updateMargins();
}

//-------------------------------------------------------------------------
//Fin Getter setter property
//-------------------------------------------------------------------------
void _SwGuiCompGridLayout::updateStretchToLayoutStretch(){
    if (_layout==0) 
        return;
    QStringList tmp=_columnStretch.split(",");
    for(int i=0;i<_layout->columnCount() && i<tmp.size();i++) {
        _layout->setColumnStretch(i,tmp[i].toInt());
    }
    tmp=_rowStretch.split(",");
    for(int i=0;i<_layout->rowCount() && i<tmp.size();i++) {
        _layout->setRowStretch(i,tmp[i].toInt());
    }
}
void _SwGuiCompGridLayout::updateLayoutStretchToStretch(){
    if (_layout==0) 
        return;

    QString tmp="";
    for(int i=0;i<_layout->columnCount();i++) {
        if (i>0) {
            tmp+=",";
        }
        tmp+=QString("%1").arg(_layout->columnStretch(i)); 
    }
    QStringList tmpi=_columnStretch.split(",");
    if (tmpi.size()>_layout->columnCount()) {
        for (int i=_layout->columnCount();i<tmpi.size();i++) {
            if (i>0) {
                tmp+=",";
            }
            tmp+=tmpi[i]; 
        }
    }
    _columnStretch=tmp;
    tmp="";
    for(int i=0;i<_layout->rowCount();i++) {
        if (i>0) {
            tmp+=",";
        }
        tmp+=QString("%1").arg(_layout->rowStretch(i)); 
    }
    tmpi=_rowStretch.split(",");
    if (tmpi.size()>_layout->rowCount()) {
        for (int i=_layout->rowCount();i<tmpi.size();i++) {
            if (i>0) {
                tmp+=",";
            }
            tmp+=tmpi[i]; 
        }
    }
    _rowStretch=tmp;
}
void _SwGuiCompGridLayout::updateMargins() {
    if (_layout==0) 
        return;
    _layout->setContentsMargins(_leftMargin,_topMargin,_rightMargin,_bottomMargin);
}
