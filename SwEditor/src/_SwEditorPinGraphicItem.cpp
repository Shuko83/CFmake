/*!
 \file _SwEditorPinGraphicItem.cpp
 \brief  Class _SwEditorPinGraphicItem gerant la representation graphique d'un pin dans une vue de dessin QT
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
#include <QGraphicsSceneMouseEvent>
#include "_SwEditorPinGraphicItem.h"
#include "_SwEditorGIPinMenu.h"
#include <math.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

#define CL_PORT_SIZE 10
#define CL_END_P_PORT_SIZE 10
#define CL_END_C_PORT_SIZE 12
#define CL_H_END_C_PORT_SIZE CL_END_C_PORT_SIZE/2
#define CL_PI 3.14159
#define CL_GRAB_DISTANCE 10.0
#define CL_GRAB_DISTANCE_SQUARE CL_GRAB_DISTANCE*CL_GRAB_DISTANCE
#define CL_INITIAL_PATH_SEGMENT_SIZE 20
#define CL_CTRL_POINT_SIZE 6.0
/*! \brief Constructeur */
_SwEditorPinGraphicItem::_SwEditorPinGraphicItem(_SwEditorGraphicItem * host,QString name,QString type_name,_SwEditorPinsLayerManager * pl_manager,ISwPins_Manager * source):QGraphicsItem(host) {
    _host=host;
    _comp_host=_host->GetHostComponent();
    _name=name;
    _control_index=-1;
    _type_name=type_name;
    _pl_manager=pl_manager;
    _hosting_service=source;
    _pl_manager->OnPinVisibilityChange.iconnect(*this,&_SwEditorPinGraphicItem::OnPinVisibilityChange);
    _pl_manager->OnPinHighlightChange.iconnect(*this,&_SwEditorPinGraphicItem::OnPinHighlightChange);
    _remote_item=NULL;
    _start_port=QRectF(-QPointF(CL_PORT_SIZE/2.0,CL_PORT_SIZE/2.0),QSize(CL_PORT_SIZE,CL_PORT_SIZE));
    _host->Attach_Listener(this);
    _scene=NULL;
    _start_port_linear_prec_position=0.0;
    _start_port_linear_position=0.0;
    _start_port_position=QPointF(0.0,0.0);
    _is_start_port_editing=false;
    _end_port=QRectF(-QPointF(CL_END_P_PORT_SIZE/2.0,CL_END_P_PORT_SIZE/2.0),QSize(CL_END_P_PORT_SIZE,CL_END_P_PORT_SIZE));
    _end_port_position=QPointF(0.0,0.0);
    SetEndPosition(_end_port_position);
    _connecting_mode=false;
    _is_connection_draw_responsable=false;
    setToolTip(name);
    if (!_pl_manager->GetPinDisplayState(_type_name)) {
        setVisible(false);
    }
    _highlight_flag=false;
}
/*! \brief Destructeur */
_SwEditorPinGraphicItem::~_SwEditorPinGraphicItem() {
    //_host->Detach_Listener(this);
    Q_ASSERT(_remote_item==NULL);
}
/*! \brief Permet de connaitre le rectangle englobant l'item graphique
\return renvoie le rectangle*/
QRectF _SwEditorPinGraphicItem::boundingRect() const {
    QRectF r;

    if (_remote_item!=NULL) {
        r=_routing.GetPath().boundingRect();
    }
    r=_start_port | _end_port.translated(_end_port_position) | _text_area |r;
    if (_is_start_port_editing || _connecting_mode) {
        return r.adjusted(-20.0,-20.0,20.0,20.0);
    }
    return r;

}
/*! \brief Dessine l'item*/
void _SwEditorPinGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen p;
    QString label;

    painter->setRenderHint(QPainter::Antialiasing);

    SetEndPositionDeduceFromRemoteItem();

    //Dessin de la source
    painter->setPen(QColor(10,10,10));
    p=painter->pen();
    p.setWidthF(1.0);
    painter->setPen(p);
    if (_remote_item!=NULL) {
        painter->setBrush(QColor(247,148,29));
    } else {
        painter->setBrush(QColor(255,255,255));
    }
    label=_name;
    if (_is_start_port_editing) {
        painter->setPen(QColor(255,000,000));
        label=_name+"\n["+_type_name+"]";
    } else {
        if (_highlight_flag) {
            painter->setPen(QColor(255,0,0));
            p=painter->pen();
            p.setWidthF(2.0);
            painter->setPen(p);
            label=_name+"\n["+_type_name+"]";
        } else {
            painter->setPen(QColor(128,128,128));
        }
    }
    //Calcul des cnagle de pie
	int startAngle = 0, spanAngle = 0;
    if (_start_port_linear_position==0.0 || _start_port_linear_position==0.25 || _start_port_linear_position==0.50|| _start_port_linear_position==0.75) {
        if (_start_port_linear_position==0.0) startAngle=0;
        if (_start_port_linear_position==0.25) startAngle=-1440;
        if (_start_port_linear_position==0.5) startAngle=2880;
        if (_start_port_linear_position==0.75) startAngle=-4320;
        spanAngle=4320;
    } else {
        if (_start_port_linear_position>0.0 && _start_port_linear_position<0.25) startAngle=0;
        if (_start_port_linear_position>0.25 && _start_port_linear_position<0.5) startAngle=-1440;
        if (_start_port_linear_position>0.5 && _start_port_linear_position<0.75) startAngle=2880;
        if (_start_port_linear_position>0.75 && _start_port_linear_position<1.0) startAngle=-4320;
        spanAngle=2880;
    }

    painter->drawPie(_start_port,startAngle,spanAngle);
    //Dessin du texte
    _text_area.moveCenter(QPointF(0.0,0.0));
    if (_start_port_linear_position>=0.0 && _start_port_linear_position<0.25) {
        _text_area=painter->boundingRect(_text_area,Qt::AlignCenter,label);
        _text_area.translate(0.0,-CL_PORT_SIZE/2.0-_text_area.height()+4.0);
    }
    if (_start_port_linear_position>=0.25 && _start_port_linear_position<0.5) {
        _text_area=painter->boundingRect(_text_area,Qt::AlignCenter,label);
        _text_area.translate(_text_area.width()/2.0+CL_PORT_SIZE/2.0+2.0,0.0);
    }
    if (_start_port_linear_position>=0.5 && _start_port_linear_position<0.75) {
        _text_area=painter->boundingRect(_text_area,Qt::AlignCenter,label);
        _text_area.translate(0.0,CL_PORT_SIZE/2.0+_text_area.height()-4.0);
    }
    if (_start_port_linear_position>=0.75 && _start_port_linear_position<1.0) {
        _text_area=painter->boundingRect(_text_area,Qt::AlignCenter,label);
        _text_area.translate(-_text_area.width()/2.0-CL_PORT_SIZE/2.0-2.0,0.0);
    }
    painter->drawText(_text_area,Qt::AlignCenter,label);
    //Dessin de la connection
    if (_connecting_mode) {
        painter->setPen(QColor(0,0,0));
        QPen pen=painter->pen();
        pen.setStyle(Qt::DotLine);
        pen.setWidthF(2.0);
        painter->setPen(pen);
        painter->drawLine(QPointF(0.0,0.0),_end_port_position);
        if (_routing.GetPath().count()>=2) {
            painter->drawPolyline(_routing.GetPath().data(),_routing.GetPath().count());
        }
        pen.setStyle(Qt::SolidLine);
    }
    if (_remote_item!=NULL && _is_connection_draw_responsable) {
        if (_is_start_port_editing || _remote_item->_is_start_port_editing)
            painter->setPen(QColor(255,100,100));
        else
            painter->setPen(QColor(0,114,188,200));
        QPen pen=painter->pen();
        pen.setWidthF(3.0);
        painter->setPen(pen);
        //painter->drawLine(_symbol_start_port_position,_symbol_end_port_position);
		if (_routing.GetPath().count() >= 2) {
            painter->drawPolyline(_routing.GetPath().data(),_routing.GetPath().count());
        }
        if (_routing.GetControlPoints().count()>=1 && isSelected()) {
            pen.setWidthF(CL_CTRL_POINT_SIZE);
            pen.setColor(QColor(255,255,255));
            painter->setPen(pen);
            painter->drawPoints(_routing.GetControlPoints().data(),_routing.GetControlPoints().count());
            pen.setWidthF(CL_CTRL_POINT_SIZE-2.0);
            pen.setColor(QColor(0,0,0));
            painter->setPen(p);
            painter->drawPoints(_routing.GetControlPoints().data(),_routing.GetControlPoints().count());
            if (_control_index!=-1) {
                pen.setColor(QColor(255,0,0));
                painter->setPen(pen);
                painter->drawPoint(_routing.GetControlPoints()[_control_index]);
            }
        }
    }
    painter->setRenderHint(QPainter::Antialiasing,false);
}
/*! \brief Sur changement de l'item */
QVariant _SwEditorPinGraphicItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change==ItemChildRemovedChange) {
        _host->Detach_Listener(this);
    }
    return QGraphicsItem::itemChange(change,value);
}
/*! \brief Sur pression d'un bouton de la sourie */
void _SwEditorPinGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QList<_SwEditorPinGraphicItem *> * related_connectables_items;
    QRectF r;
    r.setWidth(CL_CTRL_POINT_SIZE);
    r.setHeight(CL_CTRL_POINT_SIZE);


    if (event->button() == Qt::LeftButton && (flags() & ItemIsSelectable)) {
        setSelected(true);
        if (_start_port.contains(event->pos())) {
            if ((event->modifiers() & Qt::ControlModifier) && _remote_item==NULL) {
                _connecting_mode=true;
                _connectables_positions.clear();
                _pl_manager->SetPinToHighlight(_type_name,true);
                related_connectables_items=_pl_manager->GetHighlightedPinsItems();
                for(int i=0;i<related_connectables_items->count();i++)
                    _connectables_positions.push_back((*related_connectables_items)[i]->mapToItem(this,QPointF(0.0,0.0)));
                _end_port_position=_start_port_position;
            } else {
                _is_start_port_editing=true;
                if (_remote_item!=NULL) _remote_item->update();
            }
        }
        if (_remote_item!=NULL && _is_connection_draw_responsable) {
            for (int i=0;i<_routing.GetControlPoints().count();i++) {
                r.moveCenter(_routing.GetControlPoints()[i]);
                if (r.contains(event->pos())) {
                    _control_index=_routing.ChangeControlPoint(i,_routing.GetControlPoints()[i]);
                    i=_routing.GetControlPoints().count();
                }
            }
        }
        if (!(_is_start_port_editing||_connecting_mode||_control_index>=0)) {
            event->ignore();
        } else {
            event->accept();
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
void _SwEditorPinGraphicItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    if ((event->buttons() & Qt::LeftButton) && (flags() & ItemIsMovable)) {
        // Handle ItemIsMovable.
        if (_is_start_port_editing) {
            QPointF newPos(mapToParent(event->pos()) - matrix().map(event->buttonDownPos(Qt::LeftButton)));
            QPointF diff = newPos - pos();
            SetPortPosition(pos() + diff);
            if (_remote_item!=NULL) _remote_item->update();
        }
        if (_connecting_mode) {
            QPointF tmp_point;
            int index_grab = 0;
            bool grab_pos=false;
            for (int i=0;i<_connectables_positions.count();i++) {
                //On regarde si une interface est proche
                tmp_point=_connectables_positions[i]-event->pos();
                qreal d2=tmp_point.x()*tmp_point.x()+tmp_point.y()*tmp_point.y();

                if (d2<CL_GRAB_DISTANCE_SQUARE) {
                    tmp_point=_connectables_positions[i];
                    grab_pos=true;
                    index_grab=i;
                    i=_connectables_positions.count();
                }
            }
            if (grab_pos) {
                SetEndPosition(tmp_point,(*_pl_manager->GetHighlightedPinsItems())[index_grab]);
            } else {
                SetEndPosition(event->pos());
            }
            update();
        }
        if (_control_index!=-1) {
            QPointF new_ctrl_point=event->pos();
            if (event->modifiers() & Qt::ControlModifier) {
                QPointF prec_point=_routing.GetPath()[_control_index/2];
                QPointF next_point=_routing.GetPath()[_control_index/2+2];
                if (abs(prec_point.x()-new_ctrl_point.x())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setX(prec_point.x());
                }
                if (abs(prec_point.y()-new_ctrl_point.y())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setY(prec_point.y());
                }
                if (abs(next_point.x()-new_ctrl_point.x())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setX(next_point.x());
                }
                if (abs(next_point.y()-new_ctrl_point.y())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setY(next_point.y());
                }
            }
            _routing.ChangeControlPoint(_control_index,new_ctrl_point);
            update();
        }
        SignalChangeToListeners(this);
    } else {
        event->ignore();
    }
}
/*! \brief Sur liberation d'un bouton de la sourie */
void _SwEditorPinGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QList<_SwEditorPinGraphicItem *> * related_connectables_items;
    _SwEditorPinGraphicItem * remote_pin_item;

    if (flags() & ItemIsSelectable) {
        if (event->scenePos() == event->buttonDownScenePos(Qt::LeftButton)) {
            setSelected(true);
        }
        if (_connecting_mode ) {
            QPointF tmp_point;
            for (int i=0;i<_connectables_positions.count();i++) {
                //On regarde si une interface est proche
                tmp_point=_connectables_positions[i]-event->pos();
                qreal d2=tmp_point.x()*tmp_point.x()+tmp_point.y()*tmp_point.y();
                if (d2<CL_GRAB_DISTANCE_SQUARE) {
                    related_connectables_items=_pl_manager->GetHighlightedPinsItems();
                    remote_pin_item=(*related_connectables_items)[i];
                    _hosting_service->ConnectRemotePinToLocalPin(_name,remote_pin_item->GetPinName(),remote_pin_item->_hosting_service);
                    i=_connectables_positions.count();
                }
            }
        }
        if (_control_index!=-1) {
            QPointF new_ctrl_point=event->pos();
            if (event->modifiers() & Qt::ControlModifier) {
                QPointF prec_point=_routing.GetPath()[_control_index/2];
                QPointF next_point=_routing.GetPath()[_control_index/2+2];
                if (abs(prec_point.x()-new_ctrl_point.x())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setX(prec_point.x());
                }
                if (abs(prec_point.y()-new_ctrl_point.y())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setY(prec_point.y());
                }
                if (abs(next_point.x()-new_ctrl_point.x())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setX(next_point.x());
                }
                if (abs(next_point.y()-new_ctrl_point.y())<CL_GRAB_DISTANCE) {
                    new_ctrl_point.setY(next_point.y());
                }
            }
            _routing.ChangeControlPoint(_control_index,new_ctrl_point);
        }
        _is_start_port_editing=false;
        _connecting_mode=false;
        _control_index=-1;
        _pl_manager->SetPinToHighlight(_type_name,false);
    }
}
/*! \brief Liberation de l'item */
void _SwEditorPinGraphicItem::Liberate() {
    _pl_manager->OnPinHighlightChange.idisconnect(*this,&_SwEditorPinGraphicItem::OnPinHighlightChange);
    _pl_manager->OnPinVisibilityChange.idisconnect(*this,&_SwEditorPinGraphicItem::OnPinVisibilityChange);
    setVisible(false);
}
/*! \brief Acces au nom du pin */
QString _SwEditorPinGraphicItem::GetPinName() {
    return _name;
}
/*! \brief Acces au services d'interface relatif a cette interface*/
ISwPins_Manager * _SwEditorPinGraphicItem::GetPinManager() {
    return _hosting_service;
}
//-------------------------------------------------------------------
// Interface _ISwEditorGraphicItemListener
//-------------------------------------------------------------------
/*! \brief Sur  changement de l'item representant le composant*/
void _SwEditorPinGraphicItem::OnItemChanged() {
    SetPortLinearPosition(_start_port_linear_position);
    if (_host->isSelected()) {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    } else {
        setFlags(0);
    }
    SignalChangeToListeners(this);
}
//-------------------------------------------------------------------
// Interface _ISwEditorPinGraphicItemListener
//-------------------------------------------------------------------
/*! \brief Sur  changement de l'item representant le pin*/
void _SwEditorPinGraphicItem::OnPinItemChange(_SwEditorPinGraphicItem * source) {
    if (source==this)
        return;
    if (source==NULL) SignalChangeToListeners(this);
    else SignalChangeToListeners(source);
    update();
}

//---------------------------------------------------------------------
// Definition du remote interface item
//---------------------------------------------------------------------
/*! \brief Definition du remote interface item */
void _SwEditorPinGraphicItem::SetRemoteItem(_SwEditorPinGraphicItem * remote_item) {
    if (remote_item==NULL) {
        _is_connection_draw_responsable=false;
        _routing.Reset();
    }
    if (_remote_item!=NULL) _remote_item->Detach_Listener(this);
    if (remote_item!=NULL && remote_item->GetRemoteItem()==NULL)
        _is_connection_draw_responsable=true;
    _remote_item=remote_item;
    if (_remote_item!=NULL) _remote_item->Attach_Listener(this);
    OnPinItemChange(NULL);
}
/*! \brief Acces au remote pin item */
_SwEditorPinGraphicItem * _SwEditorPinGraphicItem::GetRemoteItem() {
    return _remote_item;
}
//---------------------------------------------------------------------
// Gestion des listeners
//---------------------------------------------------------------------
/*! \brief Ajout d'un listener */
void _SwEditorPinGraphicItem::Attach_Listener(_ISwEditorPinGraphicItemListener * listener) {
    QSet<_ISwEditorPinGraphicItemListener *>::const_iterator it;

    it=_observers.find(listener);
    if (it!=_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to attach an already attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)
    }
    _observers.insert(listener);
}
/*! \brief Retrait d'un listener */
void _SwEditorPinGraphicItem::Detach_Listener(_ISwEditorPinGraphicItemListener * listener) {
    QSet<_ISwEditorPinGraphicItemListener *>::iterator it;

    it=_observers.find(listener);
    if (it==_observers.end()) {
        QString msg=QString("In _SwEditorGraphicItem \nFailed to detach an undefined attached listener");
        LAUNCH_SWEXCEPTION("SwEditor",msg)
    }
    _observers.erase(it);
}
/*! \brief Signaler aux listener les changements */
void _SwEditorPinGraphicItem::SignalChangeToListeners(_SwEditorPinGraphicItem * source) {
    QSet<_ISwEditorPinGraphicItemListener *>::iterator it;

    for (it=_observers.begin();it!=_observers.end();it++) {
        (*it)->OnPinItemChange(source);
    }
}
//---------------------------------------------------------------------
// Gestion du positionnement
//---------------------------------------------------------------------
/*! \brief Permet de definir la position linaire du port de l'interface (sur le bord du composant)
\param[in] pos Position linaire (comprise entre 0 et 1)*/
void _SwEditorPinGraphicItem::SetPortLinearPosition(double pos) {
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
double _SwEditorPinGraphicItem::GetPortLinearPosition() {
    return _start_port_linear_position;
}
/*! \brief Permet de connaitre la position linaire du port de l'interface s'approchant le plus
de la position passé en parametres*/
QPointF _SwEditorPinGraphicItem::GetPortNearestValidPosition(const QPointF & pos,double * reminding_linear_position) {
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
void _SwEditorPinGraphicItem::SetPortPosition(const QPointF & pos,bool reprocess_linear_position) {
    QString s;

    if (reprocess_linear_position) {
        _start_port_linear_prec_position=_start_port_linear_position;
        _start_port_position=GetPortNearestValidPosition(pos,&_start_port_linear_position);
    } else {
        _start_port_position=pos;
    }
    setPos(_start_port_position);
    //Size a jour
    if (_host->scene()!=NULL) {
        update();
    }
    SignalChangeToListeners(this);
}
/*! \brief Permet de definir la position du end port*/
void _SwEditorPinGraphicItem::SetEndPosition(const QPointF & pos,_SwEditorPinGraphicItem * potential_remote_item) {
    _end_port_position=pos;

    if (_is_connection_draw_responsable) {
        if (_remote_item==NULL) {
            BuildDrawPath(pos,potential_remote_item);
        } else {
            UpdateDrawPath(pos);
        }
    }

    double x,y;
    double xs,ys;
    double d2,ds2;

    if (pos.x()==0.0 && pos.y()==0.0) {
        _symbol_end_port_position=QPointF(0.0,0.0);
        _symbol_start_port_position=QPointF(0.0,0.0);
    }
    if (pos.y()==0.0) {
        x=pos.x();
        if (x>0.0) {
            x=x-CL_H_END_C_PORT_SIZE;
            xs=CL_H_END_C_PORT_SIZE;
        } else {
            x=x+CL_H_END_C_PORT_SIZE;
            xs=-CL_H_END_C_PORT_SIZE;
        }
        y=0.0;
        ys=0.0;
    } else {
        if (pos.x()==0.0) {
            y=pos.y();
            if (y>0.0) {
                y=y-CL_H_END_C_PORT_SIZE;
                ys=CL_H_END_C_PORT_SIZE;
            } else {
                y=y+CL_H_END_C_PORT_SIZE;
                ys=-CL_H_END_C_PORT_SIZE;
            }
            x=0.0;
            xs=0.0;
        } else {
            d2=pos.x()*pos.x()+pos.y()*pos.y();
            ds2=d2+CL_H_END_C_PORT_SIZE*CL_H_END_C_PORT_SIZE-2*CL_H_END_C_PORT_SIZE*sqrt(d2);
            x=sqrt(ds2/((pos.y()*pos.y())/(pos.x()*pos.x())+1.0));
            xs=sqrt((CL_H_END_C_PORT_SIZE*CL_H_END_C_PORT_SIZE)/((pos.y()*pos.y())/(pos.x()*pos.x())+1.0));
            if (x*pos.x()<0.0) x=-x;
            if (xs*pos.x()<0.0) xs=-xs;
            y=(pos.y()/pos.x())*x;
            ys=(pos.y()/pos.x())*xs;
        }
    }
    _symbol_end_port_position=QPointF(x,y);
    _symbol_start_port_position=QPointF(xs,ys);
}
/*! \brief Permet de definir la position du end port en fonction de l'item distant*/
void _SwEditorPinGraphicItem::SetEndPositionDeduceFromRemoteItem() {
    if (_remote_item==NULL) return;

    QPointF pos=_remote_item->mapToItem(this,QPointF(0.0,0.0));
    SetEndPosition(pos);
}
/*! \brief Permet de resetter la position de end*/
void _SwEditorPinGraphicItem::ResetEndPosition() {
    SetEndPosition(QPointF(0.0,0.0));
}
/*! \brief Permet de connaitre la position finale correspondante*/
QPointF _SwEditorPinGraphicItem::GetEndPosition() {
    return _end_port_position;
}
/*! \brief Permet de definir le dessin du chemin de connexion*/
void _SwEditorPinGraphicItem::BuildDrawPath(const QPointF & pos,_SwEditorPinGraphicItem * potential_remote_item) {

    //Calcul chemin
    _routing.Initialize(QPointF(0.0,0.0),_SwRouting_ToolBox::Undefined,pos,_SwRouting_ToolBox::Undefined);
    _routing.Build();
}
/*! \brief Permet de mettre a jour le dessin du chemin de connexion*/
void _SwEditorPinGraphicItem::UpdateDrawPath(const QPointF & pos) {
    if (_is_connection_draw_responsable && _routing.GetPath().count()==0) {
        BuildDrawPath(pos,_remote_item);
    } else {
        _routing.ChangeEndPoint(pos);
    }
}
/*! \brief Acces au données de routage*/
_SwRouting_ToolBox * _SwEditorPinGraphicItem::GetRouting() {
    return &_routing;
}
//---------------------------------------------------------------------
// Evenement visibilite interface
//---------------------------------------------------------------------
/*! \brief Reception de la visibilite d'un type de pin*/
void _SwEditorPinGraphicItem::OnPinVisibilityChange(const QString * pin_type_name,bool new_visibility) {
    if (*pin_type_name==_type_name) {
        setVisible(new_visibility);
    }
}
/*! \brief Reception de la visibilite d'un type de pin*/
void _SwEditorPinGraphicItem::OnPinHighlightChange(const QString * pin_type_name,bool set_highlight) {
    if (*pin_type_name==_type_name) {
        if (!_connecting_mode && !_is_start_port_editing && set_highlight && _remote_item==NULL) {
            if (set_highlight)
                _pl_manager->RegisterHighlightedPinsItem(this);
            _highlight_flag=set_highlight;
        } else {
            if (!set_highlight)
                _highlight_flag=set_highlight;
        }
        update();
    }

}
//---------------------------------------------------------------------
// Gestion menu
//---------------------------------------------------------------------
/*! \brief Affiche le popup relatif a l'index navigable*/
void _SwEditorPinGraphicItem::ShowPopup(const QPoint &globalpos) {
    QMenu * pl_menu=_pl_manager->GetPinDisplayStateMenu();

    if (_remote_item!=NULL) {
        _SwEditorGIPinMenu * menu=new _SwEditorGIPinMenu(this,_comp_host);
        QAction * pl_menu_action;
        menu->addSeparator();
        pl_menu_action=menu->addMenu(pl_menu);
        menu->exec(globalpos);
        menu->removeAction(pl_menu_action);
        delete menu;
        return;
    }

    pl_menu->exec(globalpos);
    return;
}

