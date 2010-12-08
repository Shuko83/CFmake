/**
@file _SwGuiCompGridLayoutCell.cpp
@brief ComposantGridLayoutCell
@author Big
 */

#include "_SwGuiCompGridLayoutCell.h"
#include "ISwProperty.h" 

#define WIDGET_NAME "Widget%1"
#define LAYOUT_NAME "Layout%1"

/** @brief Constructor */
_SwGuiCompGridLayoutCell::_SwGuiCompGridLayoutCell(int index,ISwProperties * propertiesAccess,ISwInterfaces_Consumer *consumerAccess,bool isLayout):QObject() {
	_isLayout=isLayout;
    QString name;
    if (!_isLayout) {
        name=QString(WIDGET_NAME).arg(index);    
    } else {
        name=QString(LAYOUT_NAME).arg(index);    
    }
    setObjectName(name);
    _propertiesAccess=propertiesAccess;
    _consumerAccess=consumerAccess;
    _isBuild=false;
    _glayout=0;
    _hLayout=0;
    _hWidget=0;
    _alignment=0;
    _row=1;
    _column=1;
    _rowSpan=1;
    _columnSpan=1;
    propertiesAccess->CreatePropertiesForQObject(this,name,true);
    if (_isLayout) {
        consumerAccess->RegisterConsumedInterface<ISwLayout>(name,&_hLayout);  
    } else {
        consumerAccess->RegisterConsumedInterface<ISwWidget>(name,&_hWidget);  
    }

}
/** @brief Destructor */
_SwGuiCompGridLayoutCell::~_SwGuiCompGridLayoutCell() {
    if (_isBuild)
        destroy();
    QList<ISwProperty *> properties=_propertiesAccess->GetProperties();
    for(int i=0;i<properties.count();i++) {
        ISwProperty * p=properties[i];
        if (p->GetRealName().startsWith(objectName())) {
            _propertiesAccess->DestroyProperty(p->GetRealName());    
        }
    }
    _consumerAccess->UnregisterConsumedInterface(objectName());  
}
//Definition du grid layout
void _SwGuiCompGridLayoutCell::setGridLayout(QGridLayout * glayout) {
    if (_glayout==glayout)
        return;
    if (_isBuild)
        destroy();
    _glayout=glayout;
    if (_glayout!=0) {
        build();    
    }
}
//-------------------------------------------------------------------------
//Getter setter property
//-------------------------------------------------------------------------
unsigned int _SwGuiCompGridLayoutCell::getRow() {
    return _row;
}
void _SwGuiCompGridLayoutCell::setRow(unsigned int row){
    _row=row;
    rebuild();
}
unsigned int _SwGuiCompGridLayoutCell::getColumn(){
    return _column;
}
void _SwGuiCompGridLayoutCell::setColumn(unsigned int column){
    _column=column;
    rebuild();
}
unsigned int _SwGuiCompGridLayoutCell::getRowSpan(){
    return _rowSpan;
}
void _SwGuiCompGridLayoutCell::setRowSpan(unsigned int rowSpan){
    _rowSpan=rowSpan;
    rebuild();
}
unsigned int _SwGuiCompGridLayoutCell::getColumnSpan(){
    return _columnSpan;
}
void _SwGuiCompGridLayoutCell::setColumnSpan(unsigned int columnSpan){
    _columnSpan=columnSpan;
    rebuild();
}
Qt::Alignment _SwGuiCompGridLayoutCell::getAlignment(){
    return _alignment;
}
void _SwGuiCompGridLayoutCell::setAlignment(Qt::Alignment alignment){
    _alignment=alignment;
    rebuild();
}
//-------------------------------------------------------------------------
//Fin Getter setter property
//-------------------------------------------------------------------------
/*! \brief evenement avant changement de la disponibilité de l'interface
    \note A Surcharger*/
void _SwGuiCompGridLayoutCell::eventBeforeInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
    if (_isLayout) {
        if (interface_name==objectName() && _hLayout!=0) {
            destroy();
        }
    } else {
        if (interface_name==objectName() && _hWidget!=0) {
            destroy();
        }
    }
}
/*! \brief evenement apres changement de la disponibilité de l'interface
    \note A Surcharger*/
void _SwGuiCompGridLayoutCell::eventAfterInterfaceAvailability(QString interface_name,SwComponent_Class * provider_host) {
    if (_isLayout) {
        if (interface_name==objectName() && _hLayout!=0) {
            build();
        }
    } else {
        if (interface_name==objectName() && _hWidget!=0) {
            build();
        }
    }
}
//Reconstruction
void _SwGuiCompGridLayoutCell::rebuild(){
    if (!_isBuild)
        return;
    destroy();
    build();
}
//Construction
void _SwGuiCompGridLayoutCell::build() {
    if (_glayout==0)
        return;
    if (_isLayout && _hLayout==0)
        return;
    if (!_isLayout && _hWidget==0)
        return;
    qDebug("Build cell");
    if (_isLayout) {
        _glayout->addLayout(&_hLayout->GetLayout(),_row, _column,_rowSpan,_columnSpan,_alignment);    
    } else {
        _glayout->addWidget(&_hWidget->GetWidget(),_row, _column,_rowSpan,_columnSpan,_alignment);    
    }
    _isBuild=true;
}
//Destruction
void _SwGuiCompGridLayoutCell::destroy() {
    if (!_isBuild)
        return;
    qDebug("Destroy cell");
    if (_isLayout) {
        _hLayout->LiberateLayout();    
    } else {
        _hWidget->GetWidget().setParent(0);
    }
    _isBuild=false;
    
}
