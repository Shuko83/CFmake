/**
@file QInterestAreaTreeModel.cpp
@brief QModel pour tree view relative au interest area
@author F.Bighelli
 */

#include "QInterestAreaTreeModel.h"
#include "SwApplication.h"
#include "SwMacros.h"
 
using namespace StreamWork::SwCore;

bool caseInsensitiveLessThan(const QInterestAreaTreeModel::Item * s1, const QInterestAreaTreeModel::Item * s2)
{
    return s1->getText().toLower() < s2->getText().toLower();
}

/** @brief Constructor */
QInterestAreaTreeModel::QInterestAreaTreeModel(QObject * parent):QAbstractItemModel(parent) {
	_controler=0;
}

QInterestAreaTreeModel::~QInterestAreaTreeModel()
{
    qDeleteAll(_items);
}

/** @brief definition du controler */
void QInterestAreaTreeModel::setStreamControler(StreamControler * controler) {
    if(_controler!=0) {
        _controler->removeStreamControlerObserver(this);
    }
    _controler=controler;
    if (_controler!=0)
        _controler->addStreamControlerObserver(this);
    controlerHasChanged();
}
//-------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags QInterestAreaTreeModel::flags ( const QModelIndex & index ) const{
    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
int QInterestAreaTreeModel::columnCount ( const QModelIndex & parent  ) const{
    return 1;
}			
 /*! \brief Renvoie le nombre de ligne pour un parent donné */
int QInterestAreaTreeModel::rowCount ( const QModelIndex & parent  ) const{
    if (parent.isValid()) {
        Item * item=(Item *)parent.internalPointer();
        return item->getChilds().count();
    } else {
        return _items.count();
    }
}
/*! \brief Renvoie les données d'entete */
QVariant QInterestAreaTreeModel::headerData ( int section, 
                                             Qt::Orientation orientation, 
                                             int role  ) const{
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Interest Area"));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant QInterestAreaTreeModel::data (const QModelIndex & index,
                                       int role ) const{
    Item * item=(Item *)index.internalPointer();
    if (role == Qt::DisplayRole) {
        return QVariant(item->getText());
    }
    if (role == Qt::DecorationRole  ) {
        return QVariant(item->getIcon());
    }
    return QVariant();
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex QInterestAreaTreeModel::index (int row,
                                           int column,
                                           const QModelIndex & parent  ) const{
    if (parent.isValid()) {
        Item * item=(Item *)parent.internalPointer();
        return createIndex(row,column,item->getChilds().at(row));
    }
    if (_items.count()==0)
        return QModelIndex();
    return createIndex(row,column,_items.at(row));
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex QInterestAreaTreeModel::parent ( const QModelIndex & index ) const{
    if (!index.isValid()) {
        return QModelIndex();
    }
    Item * item=(Item *)index.internalPointer();
    Item * parent=item->getParent();
    if (parent==0) {
        return QModelIndex();
    }
    Item * parentParent=parent->getParent();
    if (parentParent==0) {
        if (_items.count()==0)
            return QModelIndex();
        return createIndex(_items.indexOf(parent),0,parent);
    }
    return createIndex(parentParent->getChilds().indexOf(parent),0,parent);
}
//--------------------------------------------------------------------------
// events gui
//--------------------------------------------------------------------------
/** @brief sur selection d'un item */ 
void QInterestAreaTreeModel::onSelectItem(const QModelIndex & index) {
    if (!index.isValid()) {
        return;
    }
    Item * item=(Item *)index.internalPointer();
    QGraphicsItem * gitem=item->getGraphicsItem();
    _controler->getView()->centerOn(gitem);
}
//-------------------------------------------------------------
// Interface IStreamControlerObserver
//------------------------------------------------------------
void QInterestAreaTreeModel::controlerHasChanged(){
	beginResetModel();

    qDeleteAll(_items);
    _items.clear();

    if( _controler!=0) {
        QList<InterestArea *> ias=_controler->getScene()->getAllInterestAreas();
        for(int i=0;i<ias.count();i++) {
            _items.push_back(new Item(ias.at(i)));
        }
        qSort(_items.begin(), _items.end(), caseInsensitiveLessThan);
    }
	endResetModel();
}
//-------------------------------------------------------------
// Item
//------------------------------------------------------------
QInterestAreaTreeModel::Item::Item(InterestArea * ia,Item * parent) {
    _parent=parent;
    _ia=ia;
    _cg=0;
    QList<ComponentGraphicItem *> list=_ia->getRelatedComponents();
    for(int i=0;i<list.count();i++) {
        _childs.push_back(new Item(list.at(i),this));
    }
    qSort(_childs.begin(), _childs.end(), caseInsensitiveLessThan);
    QPixmap p(16,16);
    QPainter painter(&p);
    painter.setPen(QColor(Qt::black));
    painter.setBrush(_ia->getColor());
    QRect r=p.rect();
    r.adjust(0,0,-1,-1);
    painter.drawRect(r);
    _ico=QIcon(p);
}
QInterestAreaTreeModel::Item::Item(ComponentGraphicItem * cg,Item * parent){
    _parent=parent;
    _ia=0;
    _cg=cg;
}
QInterestAreaTreeModel::Item::~Item(){
    qDeleteAll(_childs);
}
bool QInterestAreaTreeModel::Item::isIA(){
    return _ia!=0;
}
QInterestAreaTreeModel::Item * QInterestAreaTreeModel::Item::getParent(){
    return _parent;
}
QList<QInterestAreaTreeModel::Item *> & QInterestAreaTreeModel::Item::getChilds(){
    return _childs;
}
QString QInterestAreaTreeModel::Item::getText() const {
    if (_ia!=0) {
        return _ia->getText();
    } 
    return _cg->getComponent()->GetName();
}
QGraphicsItem * QInterestAreaTreeModel::Item::getGraphicsItem() {
    if (_ia!=0) {
        return _ia;
    } 
    return _cg;

}
QIcon QInterestAreaTreeModel::Item::getIcon() {
    if (_ia!=0)
        return _ico;
    return SW_APP->ComponentsBank().GetComponentIcon(_cg->getComponent()->GetFactoryName(), _cg->getComponent()->GetFactoryComponentName());
}
