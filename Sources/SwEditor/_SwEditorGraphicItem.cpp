/*!
 \file _SwEditorGraphicItem.h
 \brief Implementation of the Class _SwEditorGraphicItem gerant la representation graphique d'un composant dans une vue de dessin QT 
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/
#include <QMenu>
#include <QLatin1String>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QIcon>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QKeyEvent>
#include "_SwEditorInterfacesGraphicItemManager.h"
#include "_SwEditorPinGraphicItemManager.h"
#include "_SwEditorSelection_Class.h"
#include "_SwEditorGraphicItem.h"
#include <SwApplication.h>
#include <SwMacros.h>
#include <math.h>
#include "_SwEditorGIMenu.h"

#define CL_SW_Z_ORDER_STEP 0.1
#define CL_SW_GITEM_DEFAULT_SIZE 100.0
#define CL_SW_GITEM_DEFAULT_MINIMUM_SIZE 50.0
#define CL_SW_GITEM_DEFAULT_HEADER_HEIGHT 18.0
#define CL_SW_GITEM_DEFAULT_ICON_DIM 16.0
#define CL_SW_GITEM_DEFAULT_HEADER_TEXT_POS 2.0
#define CL_SW_GITEM_DEFAULT_RESIZER 10.0
#define CL_SW_INSIDE_BORDER 2.0
#define CL_SW_SHADOW_SIZE 3.0
#define CL_SW_GRID_SIZE 5.0

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
using namespace StreamWork::SwEditor;


/*! \brief Constructeur */
_SwEditorGraphicItem::_SwEditorGraphicItem(SwComponent_Class * host_comp):QGraphicsItem() {
    //Definition du composant parent
    _host_comp=host_comp;
    QIcon ico=SW_APP->ComponentsBank().GetComponentIcon(_host_comp->GetFactoryComponentName());
    _icone=ico.pixmap(CL_SW_GITEM_DEFAULT_ICON_DIM,CL_SW_GITEM_DEFAULT_ICON_DIM);
    QIcon icor=QIcon(":/SwEditor/mod_run.png");
    _icone_run=icor.pixmap(11,14);
    icor=QIcon(":/SwEditor/mod_run_disable.png");
    _icone_run_disable=icor.pixmap(11,14);
    _navigation=NULL;
    _operations=NULL;
    _il_manager=NULL;
    _interface_managers=NULL;
    _isNavigableIndex=false;
    //Enregistrement des signaux utiles
    _host_comp->OnParentChange.iconnect(*this,&_SwEditorGraphicItem::OnComponentParentChange);
    _host_comp->OnChangeName.iconnect(*this,&_SwEditorGraphicItem::OnComponentNameChange);
    _host_comp->OnChangeDescription.iconnect(*this,&_SwEditorGraphicItem::OnComponentNameChange);
    //Configuration de nous meme
    //setFlag(ItemIsMovable);
    //setFlag(ItemIsSelectable);
    ResizeItem(QRectF(0,0,CL_SW_GITEM_DEFAULT_SIZE,CL_SW_GITEM_DEFAULT_SIZE));
    _shadow_bbox=_bbox.translated(QPointF(CL_SW_SHADOW_SIZE,CL_SW_SHADOW_SIZE));
    _prec_bbox=_bbox|_shadow_bbox;

    //Button non presse
    _pressed_left_button_on_header=false;
    _pressed_left_button_on_resizer=false;
    _itemSelectedChangeToComponentSelected=true;

    //exe_service
    _exe_serv=dynamic_cast<ISwExecution_Service *>(_host_comp->QueryService(CG_SW_SERVICE_EXECUTION));
    _exable_serv=dynamic_cast<ISwExecutable_Service *>(_host_comp->QueryService(CG_SW_SERVICE_EXECUTABLE));
    //Accepte les drops
    setAcceptDrops(true);
}
/*! \brief Initialisation */
void _SwEditorGraphicItem::Initialize(ISwEditorStreamNavigation * nav,ISwEditorStreamOperations *op,_SwEditorInterfacesLayerManager * il_manager,_SwEditorPinsLayerManager * pl_manager) {
    _navigation=nav;
    _operations=op;
    _il_manager=il_manager;
    _pl_manager=pl_manager;
    if (_navigation!=NULL) {
        _navigation->AttachNavigationObserver(this);
    }
    _interface_managers=new _SwEditorInterfacesGraphicItemManager;
    _interface_managers->Initialize(_host_comp,this,_il_manager);
    _pin_managers=new _SwEditorPinGraphicItemManager;
    _pin_managers->Initialize(_host_comp,this,_pl_manager);
}

/*! \brief Destructeur */
_SwEditorGraphicItem::~_SwEditorGraphicItem() {
}
/*! \brief Sur propostion d'un element dragge*/           
void _SwEditorGraphicItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    
    QStringList l=event->mimeData()->formats();
    QString s;
    for(int i=0;i<l.count();i++) {
        s=l[i];
        s="";
    }
    if (_isNavigableIndex && (
        event->mimeData()->hasFormat("application/mod_list") ||
        event->mimeData()->hasFormat("application/mod")) )
        event->setAccepted(true);
    else
        event->setAccepted(false);
}
/*! \brief Sur drop event*/           
void _SwEditorGraphicItem::dropEvent ( QGraphicsSceneDragDropEvent * event ) {
    SwComponent_Class * comp;
    _SwEditorGraphicItem * iitem;
    QPointF drop_pos=event->pos();
    try {
        QStringList comps_list;
        if (event->mimeData()->hasFormat("application/mod")) {
            comps_list.push_back(QString(event->mimeData()->data("application/mod").data()));
        } else {
            QString comps=QString(event->mimeData()->data("application/mod_list").data());
            comps_list=comps.split(";",QString::SkipEmptyParts);
        }
        for(int i=0;i<comps_list.count();i++) {
            _operations->AddChild(comps_list[i]);
            comp=_operations->GetSelectedComponent(0);
            if (comp!=NULL) {
                iitem=dynamic_cast<_SwEditorGraphicItem *>(comp->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
                if (iitem!=NULL) {
                    iitem->setPos(drop_pos);   
                    drop_pos+=QPointF(30,10);
                }
            }       
        }
        if (iitem!=NULL) 
            iitem->AdjustParentItemSize();
    } catch (SwException &e) {
        QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to create new component because\n")+QString(e.what()));        
        return;
    }
}
/*! \brief Sur changement de l'item */
QVariant _SwEditorGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change==QGraphicsItem::ItemSelectedChange && _itemSelectedChangeToComponentSelected) {
        bool selectedState=value.toBool();
        _SwEditorSelection_Class * selection;
        selection=dynamic_cast<_SwEditorSelection_Class *>(_host_comp->GetParent()->QueryService(CG_SW_SERVICE_SELECTION));
        if (selectedState) {
            selection->AddToSelection(_host_comp->GetName());
        } else {
            selection->RemoveFromSelection(_host_comp->GetName());
        }
        SignalChangeToListeners();
    }
    if (change==QGraphicsItem::ItemPositionChange) {
        AdjustParentItemSize();
    }
    //Nothing to do for the moment
    return QGraphicsItem::itemChange(change, value);
}
/*! \brief Sur pression d'un bouton de la sourie */
void _SwEditorGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QList<QGraphicsItem *> items_list;

    if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::AltModifier)) {
        _navigation->GoToChild(_host_comp->GetName());
        event->accept();
        update();
        return;
    }

    if ((event->buttons() & Qt::LeftButton) && isSelected()) {
        QPointF p=event->pos();
        if (_resizer_bbox.contains(event->pos())) {
            _pressed_left_button_on_resizer=true;  
            items_list=childItems();
            _child_bbox=QRectF();
            for (int i=0;i<items_list.count();i++) {
                if (dynamic_cast<_SwEditorGraphicItem *>(items_list[i])!=NULL) 
                    _child_bbox|=items_list[i]->boundingRect();
            }
            event->accept();
            update();
            return;
        }
    }
    if ((event->buttons() & Qt::LeftButton) && parentItem()!=NULL && !_isNavigableIndex/*&& isSelected()*/) {
        QPointF p=event->pos();
        if (_header_bbox.contains(event->pos())) {
            _pressed_left_button_on_header=true;
            QGraphicsItem::mousePressEvent(event);
            return;
        }
    }
    if ((event->buttons() & Qt::RightButton) && _isNavigableIndex) {   
        ShowPopup(event->screenPos());
        event->accept();
        return;
    }
    _SwEditorGraphicItem * pitem=dynamic_cast<_SwEditorGraphicItem *>(this->parentItem());
    if ((event->buttons() & Qt::RightButton) && pitem!=NULL && pitem->_isNavigableIndex) {  
        pitem->ShowPopup(event->screenPos());
        event->accept();
        return;
    }
        
    event->ignore();    
}
/*! \brief Sur liberation d'un bouton de la sourie */
void _SwEditorGraphicItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if (_pressed_left_button_on_resizer /*&& isSelected()*/) {
        QPointF rp=event->pos()-event->lastPos();
        if (!rp.isNull()) {
            qreal w,h;
            w=_bbox.width();
            h=_bbox.height();
            w+=rp.x();
            h+=rp.y();
            if (w<CL_SW_GITEM_DEFAULT_MINIMUM_SIZE) w=CL_SW_GITEM_DEFAULT_MINIMUM_SIZE;
            if (h<CL_SW_GITEM_DEFAULT_MINIMUM_SIZE) h=CL_SW_GITEM_DEFAULT_MINIMUM_SIZE;
            QRectF r=QRectF(_bbox.topLeft(),QSize(w,h));
            QRectF rc=_child_bbox.adjusted(-CL_SW_INSIDE_BORDER,-CL_SW_INSIDE_BORDER-CL_SW_GITEM_DEFAULT_HEADER_HEIGHT,CL_SW_INSIDE_BORDER,CL_SW_INSIDE_BORDER);
            if (_child_bbox.isEmpty() || r.contains(rc)) {
                ResizeItem(r,true);
                update();
                event->accept();
                return;
            }
        }
    } 

    if (_pressed_left_button_on_header && parentItem()!=NULL/*&& isSelected()*/) {
        QPointF rp=event->pos()-event->lastPos();
        if (!rp.isNull()) {    
            QGraphicsItem::mouseMoveEvent(event);
            //Test grille
            if (event->modifiers() & Qt::ControlModifier) {
                QPointF l_pos=pos();
                l_pos.setX(floor(l_pos.x()/CL_SW_GRID_SIZE)*CL_SW_GRID_SIZE);
                l_pos.setY(floor(l_pos.y()/CL_SW_GRID_SIZE)*CL_SW_GRID_SIZE);
                setPos(l_pos);
            }
            SignalChangeToListeners();
            return;
        }
    }
    event->ignore();
}
/*! \brief Sur liberation d'un bouton de la sourie */
void _SwEditorGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (_pressed_left_button_on_resizer /*&& isSelected()*/) {
        _pressed_left_button_on_resizer=false;
        event->accept();
        update();
        return;
    }
    if (_pressed_left_button_on_header && parentItem()!=NULL/*&& isSelected()*/) {
        _pressed_left_button_on_header=false;
        QGraphicsItem::mouseReleaseEvent(event);
        //Test grille
        if (event->modifiers() & Qt::ControlModifier) {
            QPointF l_pos=pos();
            l_pos.setX(floor(l_pos.x()/CL_SW_GRID_SIZE)*CL_SW_GRID_SIZE);
            l_pos.setY(floor(l_pos.y()/CL_SW_GRID_SIZE)*CL_SW_GRID_SIZE);
            setPos(l_pos);
        }
        SignalChangeToListeners();
        return;
    }
    event->ignore();
}
/*! \brief resize de l'item */
void _SwEditorGraphicItem::ResizeItem(QRectF rect,bool propagate_to_parent) {
    _header_bbox=QRectF(rect.topLeft().x(),rect.topLeft().y(),rect.width(),CL_SW_GITEM_DEFAULT_HEADER_HEIGHT);
    _bbox=rect;
    _shadow_bbox=_bbox.translated(QPointF(CL_SW_SHADOW_SIZE,CL_SW_SHADOW_SIZE));
    _resizer_bbox=QRectF(rect.bottomRight().x()-CL_SW_GITEM_DEFAULT_RESIZER-1,rect.bottomRight().y()-CL_SW_GITEM_DEFAULT_RESIZER-1,CL_SW_GITEM_DEFAULT_RESIZER,CL_SW_GITEM_DEFAULT_RESIZER);
    if (propagate_to_parent) {
        AdjustParentItemSize();
    } 
    //Si c'est le parent, mise a jour de la vue scene
    QGraphicsItem *parent=parentItem();
    if (parent==NULL && scene()!=NULL) {
        QRectF bbox_scene=_bbox.adjusted(0.0,0.0,20.0,20.0);
        scene()->setSceneRect(bbox_scene);
    }
    SignalChangeToListeners();
    update();
}
/*! \brief ajustement de l'item parent pour qu'il contienne toujours l'item */
void _SwEditorGraphicItem::AdjustParentItemSize() {
    QGraphicsItem *parent=parentItem();
    ISwEditorGraphicItem * sw_item;


    QRectF box_in_parent=_bbox;
    box_in_parent.translate(this->pos());
    box_in_parent.adjust(-CL_SW_INSIDE_BORDER,-CL_SW_INSIDE_BORDER-CL_SW_GITEM_DEFAULT_HEADER_HEIGHT,CL_SW_INSIDE_BORDER,CL_SW_INSIDE_BORDER);
    if (parent!=NULL && !parent->boundingRect().contains(box_in_parent)) {
        sw_item=dynamic_cast<ISwEditorGraphicItem *>(parent);
        if (sw_item!=NULL) {
            //Definition de nouvelle taille du parent
            QRectF parent_bounding_box=parent->boundingRect();
            parent_bounding_box.adjust(0,0,-CL_SW_SHADOW_SIZE,-CL_SW_SHADOW_SIZE);
            QRectF new_parent_bounding_box=parent_bounding_box | box_in_parent;
            sw_item->ResizeItem(new_parent_bounding_box,true);
        }
    }
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwEditorGraphicItem::Liberate() {
    if (_interface_managers==NULL)
        return;
    _itemSelectedChangeToComponentSelected=false;
    setVisible(false);
    _interface_managers->Liberate();
    delete _interface_managers;
    _interface_managers=NULL;
    _pin_managers->Liberate();
    delete _pin_managers;
    _pin_managers=NULL;
    if (scene()!=NULL) {
        scene()->removeItem(this);    
    }
    if (_navigation!=NULL) {
        _navigation->DetachNavigationObserver(this);
        _navigation=NULL;
    }
    _host_comp->OnParentChange.idisconnect(*this,&_SwEditorGraphicItem::OnComponentParentChange);
    _host_comp->OnChangeName.idisconnect(*this,&_SwEditorGraphicItem::OnComponentNameChange);
    //delete this;//setParentItem(NULL);
    setVisible(false);
}
/*! \brief Permet de connaitre le rectangle englobant l'item graphique
\return renvoie le rectangle*/
QRectF _SwEditorGraphicItem::boundingRect() const {
    QRectF r;

    r=_bbox|_shadow_bbox;
    if (_prec_bbox.contains(r))
        return _prec_bbox;
    return r;
}
/*! \brief Permet de connaitre la zone de positionnement des connecteurs d'interfaces
\return renvoie le rectangle*/
QRectF _SwEditorGraphicItem::connectorsRect() const {
    return _bbox;
}
/*! \brief Dessine l'item*/
void _SwEditorGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    
    painter->setPen(QColor(0,0,0,0));  
    painter->setBrush(QColor(40,40,40,128));     
    painter->drawRect(_shadow_bbox);
    painter->setPen(QPen(Qt::black, 0));  
    painter->setBrush(QColor(200,200,200));     
    painter->drawRect(_bbox);
    if (_host_comp->GetParent()==_navigation->GetCurrentComponent()) {
        if (isSelected()) {
            painter->setBrush(QColor(0,173,239));
        } else {
            painter->setBrush(QColor(58,110,165));
        }
    } else {
        if (_isNavigableIndex) {
            painter->setBrush(QColor(0,74,128));    
       } else {
            painter->setBrush(QColor(128,128,128));     
        }
    }
    painter->setPen(QPen(Qt::black, 0));  
    painter->drawRect(_header_bbox);
    QPointF pixmap_pos=_header_bbox.topLeft()+QPointF(1,1);
    painter->drawPixmap(pixmap_pos,_icone);
    painter->setBrush(QBrush());
    QString s;
    if (_isNavigableIndex) painter->setPen(QPen(Qt::white, 0)); 

    if (_exe_serv!=NULL || _exable_serv!=NULL) {
        if ((_exe_serv!=NULL && !_exe_serv->IsExecutionStopped()) ||
            (_exe_serv==NULL && _exable_serv!=NULL && _exable_serv->isRunning())){
            s=QString("[Run]%1").arg(_host_comp->GetName());
            painter->drawPixmap(pixmap_pos.x()+_icone.width()+3,pixmap_pos.y()+1,_icone_run);
            painter->drawText(_header_bbox.translated(QPointF(_icone.width()+3+_icone_run.width()+2,2))&_header_bbox,Qt::AlignLeft,s);
        } else {
            s=QString("%1").arg(_host_comp->GetName());
            painter->drawPixmap(pixmap_pos.x()+_icone.width()+3,pixmap_pos.y()+1,_icone_run_disable);
            painter->drawText(_header_bbox.translated(QPointF(_icone.width()+3+_icone_run.width()+2,2))&_header_bbox,Qt::AlignLeft,s);
        }
    }else {
        s=QString("%1").arg(_host_comp->GetName());
        painter->drawText(_header_bbox.translated(QPointF(_icone.width()+3,2))&_header_bbox,Qt::AlignLeft,s);
    }
    //painter->drawText(_header_bbox.translated(QPointF(_icone.width()+3,2))&_header_bbox,Qt::AlignLeft,_host_comp->GetName());
    painter->setPen(QPen(Qt::black, 0));  
    painter->drawRect(_bbox);
    if (flags() & QGraphicsItem::ItemIsSelectable) {
        if (_pressed_left_button_on_resizer) {
            painter->setBrush(QColor(255,242,0));
        } else {
            if (isSelected()) {
                painter->setBrush(QColor(247,148,29));
            } else {
                painter->setBrush(QColor(123,46,0));            
            }
        }
        painter->setPen(QPen());
        painter->drawRect(_resizer_bbox);
    }
    _prec_bbox=_bbox|_shadow_bbox;
}
//---------------------------------------------------------------------
// Interface ISwEditorGraphicItem
//---------------------------------------------------------------------
/*! \brief Fournit l'item graphique qt correspondant
\return Renvoie un pointeur sur l'item graphique qt correspondant */
QGraphicsItem * _SwEditorGraphicItem::GetRelatedItem(){
    return this;
}
/*! \brief autorise ou non la mise a jour de la selection par itemChange */
void _SwEditorGraphicItem::EnableSelectionUpdateByItemChange(bool enable) {
    _itemSelectedChangeToComponentSelected=enable;    
}
/*! \brief Acces a l'interface manager */
ISwEditorInterfacesGraphicItemManager * _SwEditorGraphicItem::GetInterfacesManager() {
    return _interface_managers;
}
/*! \brief Acces au pin manager */
ISwEditorPinGraphicItemManager * _SwEditorGraphicItem::GetPinsManager() {
    return _pin_managers;
}
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _SwEditorGraphicItem::GetHostComponent(){
    return _host_comp;
}           
//---------------------------------------------------------------------
// Interface ISwPersistent
//---------------------------------------------------------------------
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE "gitem"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_WIDTH "width"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_HEIGHT "height"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_X "x"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_Y "y"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PX "px"
#define CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PY "py"
/*! \brief methode permettant de charger des donnees */
void _SwEditorGraphicItem::Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager){
    double px,py,pwidth,pheight;
    bool result=true;

    //To do load
    QDomElement item_node = elt.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE));
    if (item_node.isNull()) {
        return;
    }
    if (item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_WIDTH) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_HEIGHT) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_X) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_Y) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PX) &&
        item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PY) ) {
        px=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_X).toDouble(&result);
        if (!result) return;
        py=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_Y).toDouble(&result);
        if (!result) return;
        pwidth=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_WIDTH).toDouble(&result);
        if (!result) return;
        pheight=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_HEIGHT).toDouble(&result);
        if (!result) return;
        ResizeItem(QRect(px,py,pwidth,pheight));
        px=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PX).toDouble(&result);
        if (!result) return;
        py=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PY).toDouble(&result);
        if (!result) return;
        setPos(px,py);
        _prec_bbox=_bbox|_shadow_bbox;
    }
    //Chargement du gestionnaire des items d'interfaces
    _interface_managers_history_index=_interface_managers->Load(elt);
    if (_interface_managers_history_index!=0)
        finalizer_manager.RegisterFinalization(_interface_managers_history_index,this);
    //Mise a jour dessin
    _pin_managers_history_index=_pin_managers->Load(elt);
    if (_pin_managers_history_index!=0)
        finalizer_manager.RegisterFinalization(_pin_managers_history_index,this);
    update();
}

/*! \brief methode permettant de sauver des donnees */
void _SwEditorGraphicItem::Save(QXmlStreamWriter& writer)
{
	writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE);
	//taille
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_WIDTH, QString::number(_bbox.width()));
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_HEIGHT, QString::number(_bbox.height()));
	//position
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_X, QString::number(_bbox.topLeft().x()));
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_Y, QString::number(_bbox.topLeft().y()));
	//centre
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PX, QString::number(x()));
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GITEM_NODE_PY, QString::number(y()));
	//Ajout du neoud interface
	writer.writeEndElement();
	//Sauvegarde du gestionnaire des items d'interfaces
	_interface_managers->Save(writer);
	//Sauvegarde du gestionnaire des items de pins
	_pin_managers->Save(writer);
}

//---------------------------------------------------------------------
// Interface ISwFinalizer uses historic index pour differencier Interface et Pin et finaliser l'aspect graphique (draw);
//---------------------------------------------------------------------            
    /*! \brief Finalise le lien graphique pour Interface ou Pin
	\return false si la finalisation n'a pas eu lieu et true si ok*/
bool _SwEditorGraphicItem::Finalize(quint64 historic_index) {
    if (_interface_managers_history_index==historic_index) {
        _interface_managers->Finalize();
        return true;
    }
    if (_pin_managers_history_index==historic_index) {
        _pin_managers->Finalize();
        return true;
    }
    return false;
}
//---------------------------------------------------------------------
// Sur changement du parent
//---------------------------------------------------------------------
/*! \brief sur changement du parent du composant*/
void _SwEditorGraphicItem::OnComponentParentChange() {
    SwComponent_Class *host_comp_parent=_host_comp->GetParent();
    ISwEditorGraphicItem * parent_gitem;
    QGraphicsScene * sce;

    if (parentItem()!=NULL && host_comp_parent==NULL) {
        setParentItem(NULL);
        if (scene()!=NULL) {
            scene()->removeItem(this);    
        }
        return;
    }
    parent_gitem=dynamic_cast<ISwEditorGraphicItem *>(host_comp_parent->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
    if (parent_gitem==NULL) {
        //setParentItem(NULL);
        if (scene()!=NULL) {
            scene()->removeItem(this);    
        }
        return;
    } 
    //Atttachement a l'item parent
    this->setParentItem(parent_gitem->GetRelatedItem());
    //Enregistrement au niveau de la scene si elle est defini
    sce=parent_gitem->GetRelatedItem()->scene();
    //Q_ASSERT(sce!=NULL);
    if (sce!=NULL) {
       sce->addItem(this);    
    }
    setVisible(true);
    //Mise a jour
    update();
}
//---------------------------------------------------------------------
// Sur changement du nom
//---------------------------------------------------------------------
/*! \brief sur changement du nom du composant*/
void _SwEditorGraphicItem::OnComponentNameChange() {
    QString s;

    s=_host_comp->GetName()+":"+_host_comp->GetFactoryComponentName();
    if (!_host_comp->GetDescription().isEmpty()) {
        s=s+"\nDesc:"+_host_comp->GetDescription();
    }
    setToolTip(s);
    update();
}
//---------------------------------------------------------------------
// Sur mise a jour
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwEditorGraphicItem::Update(StreamWork::SwCore::ISwObservable* sender) {
    _SwEditorSelection_Class * selection;

    if (_isNavigableIndex) {
        if (_host_comp!=_navigation->GetCurrentComponent()) {
            //this n'est plus le nouvel index de validation
            _isNavigableIndex=false;
            setZValue(0.0);
            //Enregistrement de l'item en observer de la selection
            selection=dynamic_cast<_SwEditorSelection_Class *>(_host_comp->QueryService(CG_SW_SERVICE_SELECTION));
            selection->DetachSelectionObserver(this);
            //Mise a jour des selections
            SetSelectionModeOff();
        } else {
            //Mise a jour des selections  suivant a commande stry select all
            SetSelectionModeOn();
        }
    } else {
        if (_host_comp==_navigation->GetCurrentComponent()) {
            //this est le nouvel index de validation
            _isNavigableIndex=true;
            setZValue(1.0);
            //Mise a jour des selections  suivant a commande stry select all
            SetSelectionModeOn();
            //Enregistrement de l'item en observer de la selection
            selection=dynamic_cast<_SwEditorSelection_Class *>(_host_comp->QueryService(CG_SW_SERVICE_SELECTION));
            selection->AttachSelectionObserver(this);
        }
    }
    update();
}
/*! \brief Active le capacite de selection des items dont le composant est selectionnable*/
void _SwEditorGraphicItem::SetSelectionModeOn() {
    _SwEditorSelection_Class * selection;
    SwComponent_Class * child;
    ISwEditorGraphicItem * child_graphic_item;
    int nb,i;

    selection=dynamic_cast<_SwEditorSelection_Class *>(_host_comp->QueryService(CG_SW_SERVICE_SELECTION));
    //Pour tous les composants enfants selectionne, on rend le graphic item selectionnable et selectionne
    nb=selection->GetSelectedComponentNumber();
    for(i=0;i<nb;i++) {
        child=selection->GetSelectedComponent(i);
        child_graphic_item=dynamic_cast<ISwEditorGraphicItem *>(child->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
        if (child_graphic_item!=NULL) {
            child_graphic_item->EnableSelectionUpdateByItemChange(false);
            child_graphic_item->GetRelatedItem()->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            child_graphic_item->GetRelatedItem()->setSelected(true);
            child_graphic_item->EnableSelectionUpdateByItemChange(true);
            child_graphic_item->GetRelatedItem()->setZValue(1.0); 
        }
    }
    //Pour tous les composants enfants non selectionne, on rend le graphic item selectionnable et selectionne
    nb=selection->GetUnselectedComponentNumber();
    for(i=0;i<nb;i++) {
        child=selection->GetUnselectedComponent(i);
        child_graphic_item=dynamic_cast<ISwEditorGraphicItem *>(child->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
        if (child_graphic_item!=NULL) {
            child_graphic_item->EnableSelectionUpdateByItemChange(false);
            child_graphic_item->GetRelatedItem()->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            child_graphic_item->GetRelatedItem()->setSelected(false);
            child_graphic_item->EnableSelectionUpdateByItemChange(true);
            child_graphic_item->GetRelatedItem()->setZValue(0.0); 
        }
   }
}
/*! \brief Desactive le capacite de selection des items dont le composant n'est selectionnable*/
void _SwEditorGraphicItem::SetSelectionModeOff() {
    _SwEditorSelection_Class * selection;
    SwComponent_Class * child;
    ISwEditorGraphicItem * child_graphic_item;
    int nb,i;

    selection=dynamic_cast<_SwEditorSelection_Class *>(_host_comp->QueryService(CG_SW_SERVICE_SELECTION));
    //Pour tous les composants enfants selectionne, on rend le graphic item non selectionnable et non selectionne
    nb=selection->GetSelectedComponentNumber();
    for(i=0;i<nb;i++) {
        child=selection->GetSelectedComponent(i);
        child_graphic_item=dynamic_cast<ISwEditorGraphicItem *>(child->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
        if (child_graphic_item!=NULL) {
            child_graphic_item->EnableSelectionUpdateByItemChange(false);
            child_graphic_item->GetRelatedItem()->setSelected(false);
            child_graphic_item->GetRelatedItem()->setFlags(0);
            child_graphic_item->EnableSelectionUpdateByItemChange(true);
            child_graphic_item->GetRelatedItem()->setZValue(0.0); 
        }
    }
    //Pour tous les composants enfants non selectionne, on rend le graphic item non selectionnable
    nb=selection->GetUnselectedComponentNumber();
    for(i=0;i<nb;i++) {
        child=selection->GetUnselectedComponent(i);
        child_graphic_item=dynamic_cast<ISwEditorGraphicItem *>(child->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
        if (child_graphic_item!=NULL) {
            child_graphic_item->EnableSelectionUpdateByItemChange(false);
            child_graphic_item->GetRelatedItem()->setFlags(0);
            child_graphic_item->EnableSelectionUpdateByItemChange(true);
            child_graphic_item->GetRelatedItem()->setZValue(0.0); 
        }
    }
}
//---------------------------------------------------------------------
// Gestion des listeners
//---------------------------------------------------------------------            
/*! \brief Ajout d'un listener */
void _SwEditorGraphicItem::Attach_Listener(_ISwEditorGraphicItemListener * listener) {
    QSet<_ISwEditorGraphicItemListener *>::const_iterator it;

    it=_observers.find(listener);
    if (it!=_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to attach an already attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)        
    }
    _observers.insert(listener);
}
/*! \brief Retrait d'un listener */
void _SwEditorGraphicItem::Detach_Listener(_ISwEditorGraphicItemListener * listener) {
    QSet<_ISwEditorGraphicItemListener *>::iterator it;

    it=_observers.find(listener);
    if (it==_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to detach an undefined attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)        
    }
    _observers.erase(it);
}
/*! \brief Signaler aux listener les changements */
void _SwEditorGraphicItem::SignalChangeToListeners() {
    QSet<_ISwEditorGraphicItemListener *>::iterator it;    

    for (it=_observers.begin();it!=_observers.end();it++) {
        (*it)->OnItemChanged();
    }
}
//---------------------------------------------------------------------
// Gestion menu
//---------------------------------------------------------------------
/*! \brief Affiche le popup relatif a l'index navigable*/
void _SwEditorGraphicItem::ShowPopup(const QPoint &globalpos) {
    _SwEditorGIMenu * menu=new _SwEditorGIMenu(_operations,_navigation);      
    QMenu * il_menu=_il_manager->GetInterfaceDisplayStateMenu();
    QMenu * pl_menu=_pl_manager->GetPinDisplayStateMenu();
    QAction * il_menu_action;
    QAction * pl_menu_action;

    //Popup
    if (il_menu!=NULL) {
        menu->addSeparator();
        il_menu_action=menu->addMenu(il_menu);
    }
    if (pl_menu!=NULL) {
        menu->addSeparator();
        pl_menu_action=menu->addMenu(pl_menu);
    }
    menu->exec(globalpos);
    if (il_menu!=NULL)
        menu->removeAction(il_menu_action);
    update();
    delete menu;
}

