/*!
\file _SwEditorPinGraphicItemManager.cpp
\date 02/01/2007
\brief implementation du service de gestion des items graphiques représentant les interfaces
\author  Big
\version 1.0
 */
#include <SwApplication.h>
#include <SwMacros.h>
#include <QGraphicsScene>
#include "_SwEditorPinGraphicItemManager.h"
#include "ISwEditorGraphicItem.h"
#include "_SwEditorPinGraphicItem.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

#define CL_LINEAR_DISPLACEMENT 0.05

/*! \brief Constructeur */
_SwEditorPinGraphicItemManager::_SwEditorPinGraphicItemManager() {
    _host_comp=NULL;
    _host_item=NULL;
    _pins_manager=NULL;
    _current_linear_position=0.0;
}
/*! \brief Destructeur */
_SwEditorPinGraphicItemManager::~_SwEditorPinGraphicItemManager() {
    _pins_manager=dynamic_cast<ISwPins_Manager *>(_host_comp->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    if (_pins_manager!=NULL) _pins_manager->UnregisterListener(this);
}
/*! \brief Initialisation */
void _SwEditorPinGraphicItemManager::Initialize(SwComponent_Class * host_comp,_SwEditorGraphicItem * host_item,_SwEditorPinsLayerManager * pl_manager) {
    QString si;

    _host_comp=host_comp;
    _host_item=host_item;
    _pl_manager=pl_manager;

    //Definition de l'index par defaut
    _history_index=SW_APP->GetHistoricCpt();

    _pins_manager=dynamic_cast<ISwPins_Manager *>(_host_comp->QueryService(CG_SW_SERVICE_PINS_MANAGER));
    //On se mets en ecoute des gestionnaires d'interfaces
    if (_pins_manager!=NULL) _pins_manager->RegisterListener(this);
    else return;

    QList<SwPin *> liste_pin=_pins_manager->GetPinList();

    for (int i=0;i<liste_pin.count();i++) {
        OnAddPin(liste_pin[i]);
    }

}
/*! \brief Acces a l'index d'historique */
quint64 _SwEditorPinGraphicItemManager::GetHistoryIndex() {
    return _history_index;
}
//---------------------------------------------------------------------
// Interface ISwEditorPinGraphicItemManager
//---------------------------------------------------------------------
/*! \brief Renvoie l'item graphique relatif a une interface fournit */
_SwEditorPinGraphicItem * _SwEditorPinGraphicItemManager::GetItemForPin(QString name) {
    QMap<QString,_SwEditorPinGraphicItem *>::const_iterator it;

    it=_pin_gitems.find(name);
    if (it!=_pin_gitems.end()) {
        return (*it);
    }
    return NULL;
}
//---------------------------------------------------------------------
// Interface ISwPins_ManagerListener
//---------------------------------------------------------------------
/*! \brief Sur ajout d'un nouveau pin*/
void _SwEditorPinGraphicItemManager::OnAddPin(SwPin * pin) {
    QMap<QString,_SwEditorPinGraphicItem *>::const_iterator it;
    _SwEditorPinGraphicItem * created_item;
    //Creation d'un nouvel gitem pour l'interface
    //qDebug("_SwEditorPinGraphicItemManager::OnAddPin>");
    it=_pin_gitems.find(pin->GetName());
    if (it!=_pin_gitems.end()) {
        return;
    }
    created_item=new _SwEditorPinGraphicItem(_host_item,pin->GetName(),pin->GetType(),_pl_manager,_pins_manager);
    created_item->SetPortLinearPosition(_current_linear_position);
    if (_host_item->scene()!=NULL) {
        _host_item->scene()->addItem(created_item);
    }
    created_item->update();
    _current_linear_position+=CL_LINEAR_DISPLACEMENT;
    if (_current_linear_position>1.0) _current_linear_position-=1.0;
    _pin_gitems.insert(pin->GetName(),created_item);
    //qDebug("OnAddPin %s of component %s",pin->GetName().toLatin1().data(),_host_comp->GetName().toLatin1().data());
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();

}
/*! \brief Sur suppression d'un pin */
void _SwEditorPinGraphicItemManager::OnRemovePin(SwPin * pin) {
    //Suppression d'un gitem existant pour l'interface
    QMap<QString,_SwEditorPinGraphicItem *>::iterator it;
    _SwEditorPinGraphicItem * item;
    //Creation d'un nouvel gitem pour l'interface
    //qDebug("_SwEditorPinGraphicItemManager::OnRemovePin>");
    it=_pin_gitems.find(pin->GetName());
    if (it!=_pin_gitems.end()) {
        item=(*it);
        item->Liberate();
        //if (item->scene()!=NULL)
        //    item->scene()->removeItem(item);
        //delete item; //delete par le parent
        _pin_gitems.erase(it);
        //qDebug("OnRemovePin %s of component %s",pin->GetName().toLatin1().data(),_host_comp->GetName().toLatin1().data());
    } else {
        return;
    }
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
/*! \brief Sur connection de pins */
void _SwEditorPinGraphicItemManager::OnConnectPin(SwPin * local_pin,SwPin * remote_pin) {
    QMap<QString,_SwEditorPinGraphicItem *>::iterator it;
    ISwEditorGraphicItem * remote_comp_item;
    ISwEditorPinGraphicItemManager * remote_manager;
    _SwEditorPinGraphicItem * remote_item;
    //Linkage d'un gitem relatif a l'item source (consumer only)
    //qDebug("_SwEditorPinGraphicItemManager::OnConnectPin>");

    //recherche de l'item interface consumer
    it=_pin_gitems.find(local_pin->GetName());
    if (it==_pin_gitems.end()) {
        return;
    }

    //recherche de l'item interface producteur
    remote_comp_item=dynamic_cast<ISwEditorGraphicItem *>(remote_pin->GetManager()->GetHostComponent()->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
    remote_manager=remote_comp_item->GetPinsManager();
    remote_item=remote_manager->GetItemForPin(remote_pin->GetName());
    (*it)->SetRemoteItem(remote_item);
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
/*! \brief Sur deconnexion d'un pin*/
void _SwEditorPinGraphicItemManager::OnDisconnectPin(SwPin * local_pin,SwPin * remote_pin) {
    QMap<QString,_SwEditorPinGraphicItem *>::iterator it;
    //Delinkage d'un gitem relatif a l'item source (consumer only)
    //qDebug("_SwEditorPinGraphicItemManager::OnDisconnectPin>");
    //recherche de l'item interface consumer
    it=_pin_gitems.find(local_pin->GetName());
    if (it==_pin_gitems.end()) {
        return;
    }

    (*it)->SetRemoteItem(NULL);
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Nettoyage du manager*/
void _SwEditorPinGraphicItemManager::Liberate() {
    Q_ASSERT(_pin_gitems.count()==0);
}
//---------------------------------------------------------------------
// Persistent aspect
//---------------------------------------------------------------------
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE "gpitems"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE_HISTORY "history"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE "gpitem"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NAME_NODE "name"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NBPOINTS_NODE "nbpoints"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE_PORT_LIN_POS "ppos"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE "point"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_X "x"
#define CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_Y "y"
/*! \brief methode permettant de charger des donnees */
quint64 _SwEditorPinGraphicItemManager::Load(QDomElement & elt) {
    quint64 index_to_return;
    bool result=true;
    double ppos,x,y;
    int nb_points;
    QPointF epos;
    QString iname;
    _TmpGPData * h_data;


    //Load item principal
    QDomElement item_node = elt.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE));
    if (item_node.isNull() || !item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE_HISTORY)) {
        return 0;
    }
    //Load index de finalization
    index_to_return=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE_HISTORY).toULongLong(&result);
    if (!result)
        return 0;
    //Load des items info
    QDomElement subitem_node = item_node.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE));
    while (!subitem_node.isNull()) {
        //Enregistrement des données
        if (subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NAME_NODE) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE_PORT_LIN_POS)) {
            iname=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NAME_NODE);
            ppos=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE_PORT_LIN_POS).toDouble(&result);
            nb_points=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NBPOINTS_NODE).toDouble(&result);
            h_data=new _TmpGPData(ppos);
            _tmp_items.insert(iname,h_data);
            if (nb_points>2) {
                QDomElement point_node = subitem_node.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE));
                while (!point_node.isNull()) {
                    x=point_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_X).toDouble(&result);
                    y=point_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_Y).toDouble(&result);
                    h_data->p.push_back(QPointF(x,y));
                    point_node = point_node.nextSiblingElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE));
                }
            }
        }
        subitem_node = subitem_node.nextSiblingElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE));
    }
    return index_to_return;
}

/*! \brief methode permettant de sauver des donnees */
#define SHORT_DOUBLE(d) QString("%1").arg(d,0,'g',6)
void _SwEditorPinGraphicItemManager::Save(QXmlStreamWriter& writer)
{
	writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE);
	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEMS_NODE_HISTORY, QString::number(_history_index));
	for (auto it = _pin_gitems.begin(); it != _pin_gitems.end(); it++)
	{
		writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE);

		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NAME_NODE, it.key());
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NODE_PORT_LIN_POS, SHORT_DOUBLE(it.value()->GetPortLinearPosition()));
		_SwRouting_ToolBox * rt = it.value()->GetRouting();
		if (rt->GetPath().count() > 2)
		{
			writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NBPOINTS_NODE, QString::number(rt->GetPath().count()));
			for (int i = 0; i < rt->GetPath().count(); i++)
			{
				writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE);
				writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_X, SHORT_DOUBLE(rt->GetPath()[i].x()));
				writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_PATH_POINT_NODE_Y, SHORT_DOUBLE(rt->GetPath()[i].y()));
				writer.writeEndElement();
			}
		}
		else
		{
			writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GPITEM_NBPOINTS_NODE, QString::number(-1));
		}
		writer.writeEndElement();
	}
	writer.writeEndElement();
}

/*! \brief Finalisation */
void _SwEditorPinGraphicItemManager::Finalize() {
    QMap<QString,_TmpGPData *>::iterator it;
    QMap<QString,_SwEditorPinGraphicItem *>::iterator itr;
    _TmpGPData * p_data;
    //affectation des données temporaires
    for (it=_tmp_items.begin();it!=_tmp_items.end();it++) {
        QString s=it.key();
        itr=_pin_gitems.find(it.key());
        if (itr!=_pin_gitems.end()) {
            itr.value()->SetPortLinearPosition(it.value()->ppos);
            itr.value()->GetRouting()->SetPath(it.value()->p);
        }
        p_data=it.value();
        delete p_data;
    }
    _tmp_items.clear();
}

