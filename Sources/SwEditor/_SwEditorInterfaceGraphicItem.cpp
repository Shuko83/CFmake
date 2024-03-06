/*!
 \file _SwEditorInterfaceGraphicItem.cpp
 \brief  Class _SwEditorInterfaceGraphicItem gerant la representation graphique d'une interface dans une vue de dessin QT
 \version 1.0
 \date 02-01-2007 11:40:00
 \author F.Bighelli
*/
#include <QtGlobal>
#include <QMenu>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QIcon>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include "Tools/Exception/SwException.h"
#include "Main/SwMacros.h"
#include <QGraphicsSceneMouseEvent>
#include "_SwEditorInterfaceGraphicItem.h"
#include "_SwEditorGIInterfaceMenu.h"
#include <math.h>
#include "Component/Services/ISwInterfaces_Provider.h"
#include "Component/Services/ISwInterfaces_Consumer.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

#define CL_PORT_SIZE 10
#define CL_END_P_PORT_SIZE 10
#define CL_END_C_PORT_SIZE 12
#define CL_H_END_C_PORT_SIZE CL_END_C_PORT_SIZE/2
#define CL_PI 3.14159
#define CL_GRAB_DISTANCE 10.0
#define CL_GRAB_DISTANCE_SQUARE CL_GRAB_DISTANCE*CL_GRAB_DISTANCE
/*! \brief Constructeur */
_SwEditorInterfaceGraphicItem::_SwEditorInterfaceGraphicItem(_SwEditorGraphicItem * host,QString name,QString type_name,bool is_provided,_SwEditorInterfacesLayerManager * il_manager,ISwInterfaces_Service * source):QGraphicsItem(host) {
    _host=host;
    _comp_host=_host->GetHostComponent();
    _name=name;
    _type_name=type_name;
    _il_manager=il_manager;
    _hosting_service=source;
    _il_manager->OnInterfaceVisibilityChange.iconnect(*this,&_SwEditorInterfaceGraphicItem::OnInterfaceVisibilityChange);
    if (is_provided) _il_manager->OnInterfaceHighlightChange.iconnect(*this,&_SwEditorInterfaceGraphicItem::OnInterfaceHighlightChange);
    _is_provided=is_provided;
    _remote_item=NULL;
    _start_port=QRectF(-QPointF(CL_PORT_SIZE/2.0,CL_PORT_SIZE/2.0),QSize(CL_PORT_SIZE,CL_PORT_SIZE));
    _host->Attach_Listener(this);
    _scene=NULL;
    _start_port_linear_prec_position=0.0;
    _start_port_linear_position=0.0;
    _start_port_position=QPointF(0.0,0.0);
    _is_start_port_editing=false;
    if (_is_provided) {
        _end_port=QRectF(-QPointF(CL_END_P_PORT_SIZE/2.0,CL_END_P_PORT_SIZE/2.0),QSize(CL_END_P_PORT_SIZE,CL_END_P_PORT_SIZE));
    } else {
        _end_port=QRectF(-QPointF(CL_END_C_PORT_SIZE/2.0,CL_END_C_PORT_SIZE/2.0),QSize(CL_END_C_PORT_SIZE,CL_END_C_PORT_SIZE));
    }
    _end_port_position=QPointF(-50.0,-50.0);
    SetEndPosition(_end_port_position);
    _is_end_port_editing=false;
    setToolTip(name);
    if (!_il_manager->GetInterfaceDisplayState(_type_name)) {
        setVisible(false);
    }
    _highlight_flag=false;
}
/*! \brief Destructeur */
_SwEditorInterfaceGraphicItem::~_SwEditorInterfaceGraphicItem() {
    //_host->Detach_Listener(this);
    Q_ASSERT(_remote_item==NULL);
}
/*! \brief Permet de connaitre le rectangle englobant l'item graphique
\return renvoie le rectangle*/
QRectF _SwEditorInterfaceGraphicItem::boundingRect() const {
    QRectF r=_start_port | _end_port.translated(_end_port_position) | _text_area;
    if (_is_start_port_editing || _is_end_port_editing) {
        return r.adjusted(-20.0,-20.0,20.0,20.0);
    }
    return r;

}
/*! \brief Dessine l'item*/
void _SwEditorInterfaceGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);
    SetEndPositionDeduceFromRemoteItem();
    painter->setPen(QColor(10,10,10));
    painter->setBrush(QColor(255,255,255));
    if (_is_start_port_editing) {
        painter->setPen(QColor(255,000,000));
    } else {
        if (_is_end_port_editing) {
            painter->setPen(QColor(242,101,34));
        }
    }
    if (_is_provided) {
        painter->drawLine(QPointF(0.0,0.0),_end_port_position);
    } else {
        painter->drawLine(QPointF(0.0,0.0),_symbol_end_port_position);
    }
    painter->drawRect(_start_port);
    if (_is_end_port_editing) {
        painter->setPen(QColor(255,000,000));
    } else {
        if (_is_start_port_editing) {
            painter->setPen(QColor(242,101,34));
        } else {
            painter->setPen(QColor(10,10,10));
        }
    }
    if (_is_provided) {
        if (_highlight_flag) {
            painter->setPen(QColor(255,000,000));
            QPen p=painter->pen();
            p.setWidthF(3.0);
            painter->setPen(p);
            painter->setBrush(QColor(255,242,000));
        }
        painter->drawEllipse(_end_port.translated(_end_port_position));
    } else {
        QPen p=painter->pen();
        p.setWidthF(1.5);
        painter->setPen(p);
        painter->drawArc(_end_port.translated(_end_port_position),_symbol_end_port_angle,180*16);
    }
    QPointF middle_p=QPointF(_end_port_position.x()/2.0,_end_port_position.y()/2.0);
    _text_area.moveCenter(middle_p);
    _text_area=painter->boundingRect(_text_area,Qt::AlignHCenter,_name);
    painter->drawText(_text_area,Qt::AlignHCenter,_name);
    painter->setRenderHint(QPainter::Antialiasing,false);
}
/*! \brief Sur changement de l'item */
QVariant _SwEditorInterfaceGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change==ItemChildRemovedChange) {
        _host->Detach_Listener(this);
    }
    return QGraphicsItem::itemChange(change,value);
}
/*! \brief Sur pression d'un bouton de la sourie */
void _SwEditorInterfaceGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QList<_SwEditorInterfaceGraphicItem *> * related_connectables_items;
    if (event->button() == Qt::LeftButton && (flags() & ItemIsSelectable)) {
        //this->scene()->clearSelection();
        setSelected(true);
        if (_start_port.contains(event->pos())) {
            _is_start_port_editing=true;
        }
        else if (_end_port.translated(_end_port_position).contains(event->pos())) {
            if (_remote_item==NULL) {
                _is_end_port_editing=true;
                if (!_is_provided) {
                    _il_manager->SetInterfaceToHighlight(_type_name,true);
                    related_connectables_items=_il_manager->GetHighlightedInterfaceItems();
                    _connectables_positions.clear();
                    for(int i=0;i<related_connectables_items->count();i++)
                        _connectables_positions.push_back((*related_connectables_items)[i]->mapToItem(this,(*related_connectables_items)[i]->GetEndPosition()));
                }
            }
        }
        if (!(_is_start_port_editing||_is_end_port_editing)) {
            event->ignore();
        }
    }
    if ((event->buttons() & Qt::RightButton) && (_start_port.contains(event->pos()) || (_end_port.translated(_end_port_position).contains(event->pos())))) {
        ShowPopup(event->screenPos());
        event->accept();
        return;
    }
    if (!(flags() & ItemIsMovable)) {
        event->ignore();
    }
}
/*! \brief Sur liberation d'un bouton de la sourie */
void _SwEditorInterfaceGraphicItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if ((event->buttons() & Qt::LeftButton) && (flags() & ItemIsMovable)) {
        // Handle ItemIsMovable.
        if (_is_start_port_editing) {
            QPointF newPos(mapToParent(event->pos()) - matrix().map(event->buttonDownPos(Qt::LeftButton)));
            QPointF diff = newPos - pos();
            SetPortPosition(pos() + diff);
        }
        if (_is_end_port_editing) {
            if (!_is_provided) {
                QPointF tmp_point;
                bool grab_pos=false;
                for (int i=0;i<_connectables_positions.count();i++) {
                    //On regarde si une interface est proche
                    tmp_point=_connectables_positions[i]-event->pos();
                    qreal d2=tmp_point.x()*tmp_point.x()+tmp_point.y()*tmp_point.y();

                    if (d2<CL_GRAB_DISTANCE_SQUARE) {
                        tmp_point=_connectables_positions[i];
                        grab_pos=true;
                        i=_connectables_positions.count();
                    }
                }
                if (grab_pos) {
                    SetEndPosition(tmp_point);
                } else {
                    SetEndPosition(event->pos());
                }
            } else {
                SetEndPosition(event->pos());
            }
            update();
        }
        SignalChangeToListeners();
    } else {
        event->ignore();
    }
}
/*! \brief Sur liberation d'un bouton de la sourie */
void _SwEditorInterfaceGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QList<_SwEditorInterfaceGraphicItem *> * related_connectables_items;
    _SwEditorInterfaceGraphicItem * provider_item;

    if (flags() & ItemIsSelectable) {
        if (event->scenePos() == event->buttonDownScenePos(Qt::LeftButton)) {
            setSelected(true);
        }
        if (_is_end_port_editing && !_is_provided) {
            QPointF tmp_point;
            for (int i=0;i<_connectables_positions.count();i++) {
                //On regarde si une interface est proche
                tmp_point=_connectables_positions[i]-event->pos();
                qreal d2=tmp_point.x()*tmp_point.x()+tmp_point.y()*tmp_point.y();
                if (d2<CL_GRAB_DISTANCE_SQUARE) {
                    ISwInterfaces_Provider * provider;
                    ISwInterfaces_Consumer * consumer;
                    related_connectables_items=_il_manager->GetHighlightedInterfaceItems();
                    provider_item=(*related_connectables_items)[i];
                    provider=dynamic_cast<ISwInterfaces_Provider *>(provider_item->GetInterfaceService());
                    consumer=dynamic_cast<ISwInterfaces_Consumer *>(GetInterfaceService());
                    Q_ASSERT(provider!=NULL && consumer!=NULL);
                    consumer->AttachProvider(provider,_name,(*related_connectables_items)[i]->GetInterfaceName());
                    i=_connectables_positions.count();
                }
            }
        }
        _is_start_port_editing=false;
        _is_end_port_editing=false;
        if (!_is_provided) _il_manager->SetInterfaceToHighlight(_type_name,false);
    }
}
/*! \brief Liberation de l'item */
void _SwEditorInterfaceGraphicItem::Liberate() {
    if (_is_provided) _il_manager->OnInterfaceHighlightChange.idisconnect(*this,&_SwEditorInterfaceGraphicItem::OnInterfaceHighlightChange);
    _il_manager->OnInterfaceVisibilityChange.idisconnect(*this,&_SwEditorInterfaceGraphicItem::OnInterfaceVisibilityChange);
    setVisible(false);
}
/*! \brief Acces au nom de l'interface */
QString _SwEditorInterfaceGraphicItem::GetInterfaceName() {
    return _name;
}
/*! \brief Acces au services d'interface relatif a cette interface*/
ISwInterfaces_Service * _SwEditorInterfaceGraphicItem::GetInterfaceService() {
    return _hosting_service;
}
//-------------------------------------------------------------------
// Interface _ISwEditorGraphicItemListener
//-------------------------------------------------------------------
/*! \brief Sur  changement de l'item representant le composant*/
void _SwEditorInterfaceGraphicItem::OnItemChanged() {
    SetPortLinearPosition(_start_port_linear_position);
    if (_host->isSelected()) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    } else {
        setFlags(0);
    }
    SignalChangeToListeners();
}
//-------------------------------------------------------------------
// Interface _ISwEditorInterfaceGraphicItemListener
//-------------------------------------------------------------------
/*! \brief Sur  changement de l'item representant l'interface*/
void _SwEditorInterfaceGraphicItem::OnInterfaceItemChange() {
    SignalChangeToListeners();
    update();
}

//---------------------------------------------------------------------
// Definition du remote interface item
//---------------------------------------------------------------------
/*! \brief Definition du remote interface item */
void _SwEditorInterfaceGraphicItem::SetRemoteItem(_SwEditorInterfaceGraphicItem * remote_item) {
    if (_remote_item!=NULL) _remote_item->Detach_Listener(this);
    _remote_item=remote_item;
    if (_remote_item!=NULL) _remote_item->Attach_Listener(this);
    OnInterfaceItemChange();
}
/*! \brief Acces au remote interface item */
_SwEditorInterfaceGraphicItem * _SwEditorInterfaceGraphicItem::GetRemoteItem() {
    return _remote_item;
}
//---------------------------------------------------------------------
// Gestion des listeners
//---------------------------------------------------------------------
/*! \brief Ajout d'un listener */
void _SwEditorInterfaceGraphicItem::Attach_Listener(_ISwEditorInterfaceGraphicItemListener * listener) {
    QSet<_ISwEditorInterfaceGraphicItemListener *>::const_iterator it;

    it=_observers.find(listener);
    if (it!=_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to attach an already attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)
    }
    _observers.insert(listener);
}
/*! \brief Retrait d'un listener */
void _SwEditorInterfaceGraphicItem::Detach_Listener(_ISwEditorInterfaceGraphicItemListener * listener) {
    QSet<_ISwEditorInterfaceGraphicItemListener *>::iterator it;

    it=_observers.find(listener);
    if (it==_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to detach an undefined attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)
    }
    _observers.erase(it);
}
/*! \brief Signaler aux listener les changements */
void _SwEditorInterfaceGraphicItem::SignalChangeToListeners() {
    QSet<_ISwEditorInterfaceGraphicItemListener *>::iterator it;

    for (it=_observers.begin();it!=_observers.end();it++) {
        (*it)->OnInterfaceItemChange();
    }
}
//---------------------------------------------------------------------
// Gestion du positionnement
//---------------------------------------------------------------------
/*! \brief Permet de definir la position linaire du port de l'interface (sur le bord du composant)
\param[in] pos Position linaire (comprise entre 0 et 1)*/
void _SwEditorInterfaceGraphicItem::SetPortLinearPosition(double pos) {
    QRectF host_box;
    QPointF internal_start_port;

    if (pos>=1.0 || pos<0.0) pos=fmod(pos,1.0);

    _start_port_linear_prec_position=_start_port_linear_position;
    _start_port_linear_position=pos;
    host_box=_host->connectorsRect();
    if (pos>=0 && pos<0.25) {
        //Barre haute horizontale
        internal_start_port=host_box.topLeft()+QPointF(4.0*pos*host_box.width(),0.0);
    }
    if (pos>=0.25 && pos<0.5) {
        //Barre droite verticale
        internal_start_port=host_box.topRight()+QPointF(0.0,4.0*(pos-0.25)*host_box.height());
    }
    if (pos>=0.5 && pos<0.75) {
        //Barre basse horizontale
        internal_start_port=host_box.bottomRight()-QPointF(4.0*(pos-0.50)*host_box.width(),0.0);
   }
    if (pos>=0.75 && pos<1.0) {
        //Barre gauche verticale
        internal_start_port=host_box.bottomLeft()-QPointF(0.0,4.0*(pos-0.75)*host_box.height());
    }

    SetPortPosition(internal_start_port,false);

}
/*! \brief Permet de connaitre la position linaire du port de l'interface (sur le bord du composant)
\return Position linaire (comprise entre 0 et 1)*/
double _SwEditorInterfaceGraphicItem::GetPortLinearPosition() {
    return _start_port_linear_position;
}
/*! \brief Permet de connaitre la position linaire du port de l'interface s'approchant le plus
de la position passé en parametres*/
QPointF _SwEditorInterfaceGraphicItem::GetPortNearestValidPosition(const QPointF & pos,double * reminding_linear_position) {
    QRectF host_box;

// GRADIANTS DE POSITION
//   1>c>0 0>c>-1
//     |\7|0/|
// c>1 |6\|/1|c<-1
//     -------
//c<-1 |5/|\2|c>1
//     |/4|3\|
//  0>c>-1 1>c>0

    host_box=_host->connectorsRect();

    if (pos.x()<host_box.left()) {
        //Gradiant 6,5
        if (pos.y()<host_box.top()) {
            (*reminding_linear_position)=0.0;
            return host_box.topLeft();
        }
        if (pos.y()>host_box.bottom()) {
            (*reminding_linear_position)=0.75;
            return host_box.bottomLeft();
        }
        (*reminding_linear_position)=(host_box.bottom()-pos.y())/host_box.height()/4.0+0.75;
        return QPointF(host_box.left(),pos.y());
    }
    if (pos.x()>host_box.right()) {
        //Gradiant 1,2
        if (pos.y()<host_box.top()) {
            (*reminding_linear_position)=0.25;
            return host_box.topRight();
        }
        if (pos.y()>host_box.bottom()) {
            (*reminding_linear_position)=0.5;
            return host_box.bottomRight();
        }
        (*reminding_linear_position)=(pos.y()-host_box.top())/host_box.height()/4.0+0.25;
        return QPointF(host_box.right(),pos.y());
    }
    if (pos.y()<host_box.top()) {
        //Gradiant 0,7
        (*reminding_linear_position)=(pos.x()-host_box.left())/host_box.width()/4.0;
        return QPointF(pos.x(),host_box.top());
    }
    if (pos.y()>host_box.bottom()) {
        //Gradiant 4,3
        (*reminding_linear_position)=(host_box.right()-pos.x())/host_box.width()/4.0+0.5;
        return QPointF(pos.x(),host_box.bottom());
    }

    QPointF new_pos;
    QPointF rel_pos=pos-host_box.center();
    if (rel_pos.y()!=0.0) {
        double coef=rel_pos.x()/rel_pos.y();
        if (1.0>=coef && coef>0.0) {
            if (rel_pos.x()>0.0) {
                //Gradiant 3
                new_pos=QPointF(pos.x(),host_box.bottom());
                (*reminding_linear_position)=(host_box.right()-new_pos.x())/host_box.width()/4.0+0.5;
            } else {
                //Gradiant 7
                new_pos=QPointF(pos.x(),host_box.top());
                (*reminding_linear_position)=(new_pos.x()-host_box.left())/host_box.width()/4.0;
            }
        }
        if (coef>1.0) {
            if (rel_pos.x()>0.0) {
                //Gradiant 2
                new_pos=QPointF(host_box.right(),pos.y());
                (*reminding_linear_position)=(new_pos.y()-host_box.top())/host_box.height()/4.0+0.25;
            } else {
                //Gradiant 6
                new_pos=QPointF(host_box.left(),pos.y());
                (*reminding_linear_position)=(host_box.bottom()-new_pos.y())/host_box.height()/4.0+0.75;
            }
        }
        if (coef<=-1.0) {
            if (rel_pos.x()>0.0) {
                //Gradiant 1
                new_pos=QPointF(host_box.right(),pos.y());
                (*reminding_linear_position)=(new_pos.y()-host_box.top())/host_box.height()/4.0+0.25;
            } else {
                //Gradiant 5
                new_pos=QPointF(host_box.left(),pos.y());
                (*reminding_linear_position)=(host_box.bottom()-new_pos.y())/host_box.height()/4.0+0.75;
            }
        }
        if (0.0>=coef && coef>-1.0) {
            if (rel_pos.x()>0.0) {
                //Gradiant 0
                new_pos=QPointF(pos.x(),host_box.top());
                (*reminding_linear_position)=(new_pos.x()-host_box.left())/host_box.width()/4.0;
            } else {
                //Gradiant 4
                new_pos=QPointF(pos.x(),host_box.bottom());
                (*reminding_linear_position)=(host_box.right()-new_pos.x())/host_box.width()/4.0+0.5;
            }
        }
    } else {
        if (rel_pos.x()>0) {
            //Gradiant 5,6
            new_pos=QPointF(host_box.left(),pos.y());
            (*reminding_linear_position)=(host_box.bottom()-new_pos.y())/host_box.height()/4.0+0.75;
        } else {
            //Gradiant 1,2
            new_pos=QPointF(host_box.right(),pos.y());
            (*reminding_linear_position)=(new_pos.y()-host_box.top())/host_box.height()/4.0+0.25;
        }
    }

    return new_pos;
}
/*! \brief Permet de definir la position avec recalculation de la position linaire*/
void _SwEditorInterfaceGraphicItem::SetPortPosition(const QPointF & pos,bool reprocess_linear_position) {
    QString s;

    if (reprocess_linear_position) {
        _start_port_linear_prec_position=_start_port_linear_position;
        _start_port_position=GetPortNearestValidPosition(pos,&_start_port_linear_position);
    } else {
        _start_port_position=pos;
    }
    setPos(_start_port_position);
    //Mise a jour de la end position
    if (_start_port_linear_prec_position!=_start_port_linear_position) {
        QMatrix m;
        m=m.rotate((_start_port_linear_position-_start_port_linear_prec_position)*360.0);
        if (_remote_item==NULL) SetEndPosition(_end_port_position*m);
        else SetEndPositionDeduceFromRemoteItem();
    }
    //Size a jour
    if (_host->scene()!=NULL) {
        update();
    }
    SignalChangeToListeners();
}
/*! \brief Permet de definir la position du end port*/
void _SwEditorInterfaceGraphicItem::SetEndPosition(const QPointF & pos) {
    _end_port_position=pos;

    double x,y;
    double d2,ds2;

    if (pos.y()==0.0) {
        x=pos.x();
        if (x>0.0) x=x-CL_H_END_C_PORT_SIZE;
        else x=x+CL_H_END_C_PORT_SIZE;
        y=0.0;
    } else {
        if (pos.x()==0.0) {
            y=pos.y();
            if (y>0.0) y=y-CL_H_END_C_PORT_SIZE;
            else y=y+CL_H_END_C_PORT_SIZE;
            x=0.0;
        } else {
            d2=pos.x()*pos.x()+pos.y()*pos.y();
            ds2=d2+CL_H_END_C_PORT_SIZE*CL_H_END_C_PORT_SIZE-2*CL_H_END_C_PORT_SIZE*sqrt(d2);
            x=sqrt(ds2/((pos.y()*pos.y())/(pos.x()*pos.x())+1.0));
            if (x*pos.x()<0.0) x=-x;
            y=(pos.y()/pos.x())*x;
        }
    }
    _symbol_end_port_position=QPointF(x,y);

    _symbol_end_port_angle=floor(16.0*(atan2(pos.x(),pos.y())/CL_PI*180.0));
}
/*! \brief Permet de definir la position du end port en fonction de l'item distant*/
void _SwEditorInterfaceGraphicItem::SetEndPositionDeduceFromRemoteItem() {
    if (_remote_item==NULL) return;

    QPointF pos=this->mapFromScene(_remote_item->mapToScene(_remote_item->_end_port_position));
    SetEndPosition(pos);
}
/*! \brief Permet de resetter la position de end*/
void _SwEditorInterfaceGraphicItem::ResetEndPosition() {
    double pos=GetPortLinearPosition();

    SetPortLinearPosition(0.0);
    _end_port_position=QPointF(-50.0,-50.0);
    SetEndPosition(_end_port_position);
    SetPortLinearPosition(pos);
}
/*! \brief Permet de connaitre la position finale correspondante*/
QPointF _SwEditorInterfaceGraphicItem::GetEndPosition() {
    return _end_port_position;
}
//---------------------------------------------------------------------
// Evenement visibilite interface
//---------------------------------------------------------------------
/*! \brief Reception de la visibilite d'une inetrface*/
void _SwEditorInterfaceGraphicItem::OnInterfaceVisibilityChange(const QString * interface_type_name,bool new_visibility) {
    if (*interface_type_name==_type_name) {
        setVisible(new_visibility);
    }
}
/*! \brief Reception de la visibilite d'une inetrface*/
void _SwEditorInterfaceGraphicItem::OnInterfaceHighlightChange(const QString * interface_type_name,bool set_highlight) {
    if (*interface_type_name==_type_name) {
        _highlight_flag=set_highlight;
        if (_highlight_flag) _il_manager->RegisterHighlightedInterfaceItem(this);
        update();
    }

}
//---------------------------------------------------------------------
// Gestion menu
//---------------------------------------------------------------------
/*! \brief Affiche le popup relatif a l'index navigable*/
void _SwEditorInterfaceGraphicItem::ShowPopup(const QPoint &globalpos) {
    QMenu * il_menu=_il_manager->GetInterfaceDisplayStateMenu();

    if (_remote_item!=NULL) {
        _SwEditorGIInterfaceMenu * menu=new _SwEditorGIInterfaceMenu(this,_comp_host);
        QAction * il_menu_action;
        menu->addSeparator();
        il_menu_action=menu->addMenu(il_menu);
        menu->exec(globalpos);
        menu->removeAction(il_menu_action);
        delete menu;
        return;
    }

    il_menu->exec(globalpos);
    return;
}

