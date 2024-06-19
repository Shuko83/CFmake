/*!
\file _SwEditorInterfacesGraphicItemManager.cpp
\date 02/01/2007
\brief implementation du service de gestion des items graphiques représentant les interfaces
\author  Big
\version 1.0
 */
#include <SwApplication.h>
#include <SwMacros.h>
#include <QGraphicsScene>
#include "_SwEditorInterfacesGraphicItemManager.h"
#include "ISwEditorGraphicItem.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

#define CL_LINEAR_DISPLACEMENT 0.05

/*! \brief Constructeur */
_SwEditorInterfacesGraphicItemManager::_SwEditorInterfacesGraphicItemManager() {
    _host_comp=NULL;
    _host_item=NULL;
    _provider_service=NULL;
    _consumer_service=NULL;
    _provider=NULL;
    _consumer=NULL;
    _current_linear_position=0.0;
}
/*! \brief Destructeur */
_SwEditorInterfacesGraphicItemManager::~_SwEditorInterfacesGraphicItemManager() {
    _provider_service=dynamic_cast<ISwInterfaces_Service *>(_host_comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    _consumer_service=dynamic_cast<ISwInterfaces_Service *>(_host_comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    if (_provider_service!=NULL) _provider_service->DetachInterfacesServices_Listener(this);
    if (_consumer_service!=NULL) _consumer_service->DetachInterfacesServices_Listener(this);
}
/*! \brief Initialisation */
void _SwEditorInterfacesGraphicItemManager::Initialize(SwComponent_Class * host_comp,_SwEditorGraphicItem * host_item,_SwEditorInterfacesLayerManager * il_manager) {
    QString si;

    _host_comp=host_comp;
    _host_item=host_item;
    _il_manager=il_manager;

    //Definition de l'index par defaut
    _history_index=SW_APP->GetHistoricCpt();

    _provider_service=dynamic_cast<ISwInterfaces_Service *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    _consumer_service=dynamic_cast<ISwInterfaces_Service *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
    //On se mets en ecoute des gestionnaires d'interfaces
    if (_provider_service!=NULL) _provider_service->AttachInterfacesServices_Listener(this);
    if (_consumer_service!=NULL) _consumer_service->AttachInterfacesServices_Listener(this);

    //Recuperation des interfaces deja existantes
    ISwInterfaces_Provider * _provider=dynamic_cast<ISwInterfaces_Provider *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_PROVIDER));
    ISwInterfaces_Consumer * _consumer=dynamic_cast<ISwInterfaces_Consumer *>(host_comp->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));

    if (_provider!=NULL) {
        si=_provider->GetFirstInterface();
        while (!si.isEmpty()) {
            OnAddInterface(_provider_service,si);
            si=_provider->GetNextInterface();
        }
    }
    if (_consumer!=NULL) {
        si=_consumer->GetFirstInterface(NULL,NULL,NULL);
        while (!si.isEmpty()) {
            OnAddInterface(_consumer_service,si);
            si=_consumer->GetNextInterface(NULL,NULL,NULL);
        }
    }
}
/*! \brief Acces a l'index d'historique */
quint64 _SwEditorInterfacesGraphicItemManager::GetHistoryIndex() {
    return _history_index;
}
//---------------------------------------------------------------------
// Interface ISwEditorInterfacesGraphicItemManager
//---------------------------------------------------------------------
/*! \brief Renvoie l'item graphique relatif a une interface fournit */
_SwEditorInterfaceGraphicItem * _SwEditorInterfacesGraphicItemManager::GetItemForProvidedInterface(QString name) {
    QMap<QString,_SwEditorInterfaceGraphicItem *>::const_iterator it;

    it=_provided_gitems.find(name);
    if (it!=_provided_gitems.end()) {
        return (*it);
    }
    return NULL;
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ServicesListener
//---------------------------------------------------------------------
/*! \brief Sur ajout d'une nouvelle interface */
void _SwEditorInterfacesGraphicItemManager::OnAddInterface(ISwInterfaces_Service * source,QString interface_name) {
    QMap<QString,_SwEditorInterfaceGraphicItem *>::const_iterator it;
    QMap<QString,_SwEditorInterfaceGraphicItem *> * map_to_use;
    _SwEditorInterfaceGraphicItem * created_item;
    //Creation d'un nouvel gitem pour l'interface
    //qDebug("_SwEditorInterfacesGraphicItemManager::OnAddInterface>");
    if (source==_provider_service) {
        map_to_use=&_provided_gitems;
    } else {
        map_to_use=&_consumed_gitems;
    }
    it=map_to_use->find(interface_name);
    if (it!=map_to_use->end()) {
        return;
    }
    created_item=new _SwEditorInterfaceGraphicItem(_host_item,interface_name,source->GetInterfaceType(interface_name),source==_provider_service?true:false,_il_manager,source);
    created_item->SetPortLinearPosition(_current_linear_position);
    if (_host_item->scene()!=NULL) {
        _host_item->scene()->addItem(created_item);
    }
    created_item->update();
    _current_linear_position+=CL_LINEAR_DISPLACEMENT;
    if (_current_linear_position>1.0) _current_linear_position-=1.0;
    map_to_use->insert(interface_name,created_item);
    //qDebug("OnAddInterface %s of component %s",interface_name.toLatin1().data(),_host_comp->GetName().toLatin1().data());
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();

}
/*! \brief Sur suppression d'une  interface */
void _SwEditorInterfacesGraphicItemManager::OnRemoveInterface(ISwInterfaces_Service * source,QString interface_name) {
    //Suppression d'un gitem existant pour l'interface
    QMap<QString,_SwEditorInterfaceGraphicItem *>::iterator it;
    QMap<QString,_SwEditorInterfaceGraphicItem *> * map_to_use;
    _SwEditorInterfaceGraphicItem * item;
    //Creation d'un nouvel gitem pour l'interface
    //qDebug("_SwEditorInterfacesGraphicItemManager::OnRemoveInterface>");
    if (source==_provider_service) {
        map_to_use=&_provided_gitems;
    } else {
        map_to_use=&_consumed_gitems;
    }
    it=map_to_use->find(interface_name);
    if (it!=map_to_use->end()) {
        item=(*it);
        item->Liberate();
        //if (item->scene()!=NULL)
        //    item->scene()->removeItem(item);
        //delete item; //delete par le parent
        map_to_use->erase(it);
        //qDebug("OnRemoveInterface %s of component %s",interface_name.toLatin1().data(),_host_comp->GetName().toLatin1().data());
    } else {
        return;
    }
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
/*! \brief Sur connection d'une interface */
void _SwEditorInterfacesGraphicItemManager::OnConnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name) {
    QMap<QString,_SwEditorInterfaceGraphicItem *>::iterator it;
    ISwEditorGraphicItem * remote_comp_item;
    ISwEditorInterfacesGraphicItemManager * remote_manager;
    _SwEditorInterfaceGraphicItem * remote_item;
    //Linkage d'un gitem relatif a l'item source (consumer only)
    //qDebug("_SwEditorInterfacesGraphicItemManager::OnConnectInterface>");
    if (source==_provider_service) {
        qDebug("provider... nothing to do");
        return;
    }

    //recherche de l'item interface consumer
    it=_consumed_gitems.find(interface_name);
    if (it==_consumed_gitems.end()) {
        return;
    }

    //recherche de l'item interface producteur
    remote_comp_item=dynamic_cast<ISwEditorGraphicItem *>(remote_source->GetHostComponent()->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM));
    remote_manager=remote_comp_item->GetInterfacesManager();
    remote_item=remote_manager->GetItemForProvidedInterface(remote_interface_name);
    (*it)->SetRemoteItem(remote_item);
    //qDebug("OnConnectInterface %s of component %s to %s of component %s",interface_name.toLatin1().data(),_host_comp->GetName().toLatin1().data(),remote_interface_name.toLatin1().data(),remote_source->GetHostComponent()->GetName().toLatin1().data());
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
/*! \brief Sur deconnection d'une interface */
void _SwEditorInterfacesGraphicItemManager::OnDisconnectInterface(ISwInterfaces_Service * source,QString interface_name,ISwInterfaces_Service * remote_source,QString remote_interface_name) {
    QMap<QString,_SwEditorInterfaceGraphicItem *>::iterator it;
    //Delinkage d'un gitem relatif a l'item source (consumer only)
    //qDebug("_SwEditorInterfacesGraphicItemManager::OnDisconnectInterface>");
    if (source==_provider_service) {
        qDebug("provider... nothing to do");
        return;
    }
    //recherche de l'item interface consumer
    it=_consumed_gitems.find(interface_name);
    if (it==_consumed_gitems.end()) {
        return;
    }

    (*it)->SetRemoteItem(NULL);
    //qDebug("OnConnectInterface %s of component %s to %s of component %s",interface_name.toLatin1().data(),_host_comp->GetName().toLatin1().data(),remote_interface_name.toLatin1().data(),remote_source->GetHostComponent()->GetName().toLatin1().data());
    //Mise a jour de l'index d'historique
    _history_index=SW_APP->GetHistoricCpt();
}
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------
/*! \brief Nettoyage du manager*/
void _SwEditorInterfacesGraphicItemManager::Liberate() {
    Q_ASSERT(_consumed_gitems.count()==0);
    Q_ASSERT(_provided_gitems.count()==0);
}
//---------------------------------------------------------------------
// Persistent aspect
//---------------------------------------------------------------------
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE "giitems"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE_HISTORY "history"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE "name"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_PRO_NODE "giitem_pro"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_CON_NODE "giitem_con"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS "ppos"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X "px"
#define CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y "py"
/*! \brief methode permettant de charger des donnees */
quint64 _SwEditorInterfacesGraphicItemManager::Load(QDomElement & elt) {
    quint64 index_to_return;
    bool result=true;
    double ppos;
    QPointF epos;
    QString iname;

    //Load item principal
    QDomElement item_node = elt.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE));
    if (item_node.isNull() || !item_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE_HISTORY)) {
        return 0;
    }
    //Load index de finalization
    index_to_return=item_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE_HISTORY).toULongLong(&result);
    if (!result)
        return 0;
    //Load des items info provider
    QDomElement subitem_node = item_node.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_PRO_NODE));
    while (!subitem_node.isNull()) {
        //Enregistrement des données
        if (subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y)) {
            iname=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE);
            ppos=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS).toDouble(&result);
            epos.setX(subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X).toDouble(&result));
            epos.setY(subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y).toDouble(&result));
            _provided_tmp_items.insert(iname,new _TmpGIData(ppos,epos));
        }
        subitem_node = subitem_node.nextSiblingElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_PRO_NODE));
    }
    //Load des items info consumer
    subitem_node = item_node.firstChildElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_CON_NODE));
    while (!subitem_node.isNull()) {
        //Enregistrement des données
        if (subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X) &&
            subitem_node.hasAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y)) {
            iname=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE).toLatin1();
            ppos=subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS).toDouble(&result);
            epos.setX(subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X).toDouble(&result));
            epos.setY(subitem_node.attribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y).toDouble(&result));
            _consumed_tmp_items.insert(iname,new _TmpGIData(ppos,epos));
        }
        subitem_node = subitem_node.nextSiblingElement(QString(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_PRO_NODE));
    }
    return index_to_return;
}

/*! \brief methode permettant de sauver des donnees */
#define SHORT_DOUBLE(d) QString("%1").arg(d,0,'g',6)
void StreamWork::SwEditor::_SwEditorInterfacesGraphicItemManager::Save(QXmlStreamWriter& writer)
{
	writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE);

	writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEMS_NODE_HISTORY, QString::number(_history_index));
	//Ajout des description de chaque interface item
	for (auto it = _provided_gitems.begin(); it != _provided_gitems.end(); it++)
	{
		writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_PRO_NODE);
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE, it.key());
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS, SHORT_DOUBLE(it.value()->GetPortLinearPosition()));
		QPointF f = it.value()->GetEndPosition();
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X, SHORT_DOUBLE(f.x()));
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y, SHORT_DOUBLE(f.y()));
		writer.writeEndElement();
	}
	for (auto it = _consumed_gitems.begin(); it != _consumed_gitems.end(); it++)
	{
		writer.writeStartElement(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_CON_NODE);
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NAME_NODE, it.key());
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_PORT_LIN_POS, SHORT_DOUBLE(it.value()->GetPortLinearPosition()));
		QPointF f = it.value()->GetEndPosition();
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_X, SHORT_DOUBLE(f.x()));
		writer.writeAttribute(CL_SW_XML_DRAW_COMP_EDITOR_GIITEM_NODE_END_POS_Y, SHORT_DOUBLE(f.y()));
		writer.writeEndElement();
	}
	writer.writeEndElement();
}

/*! \brief Finalisation */
void _SwEditorInterfacesGraphicItemManager::Finalize() {
    QMap<QString,_TmpGIData *>::iterator it;
    QMap<QString,_SwEditorInterfaceGraphicItem *>::iterator itr;
    _TmpGIData * p_data;
    //affectation des données temporaires
    for (it=_provided_tmp_items.begin();it!=_provided_tmp_items.end();it++) {
        QString s=it.key();
        itr=_provided_gitems.find(it.key());
        if (itr!=_provided_gitems.end()) {
            itr.value()->SetPortLinearPosition(it.value()->ppos);
            itr.value()->SetEndPosition(it.value()->epos);
        }
        p_data=it.value();
        delete p_data;
    }
    for (it=_consumed_tmp_items.begin();it!=_consumed_tmp_items.end();it++) {
        itr=_consumed_gitems.find(it.key());
        if (itr!=_consumed_gitems.end()) {
            itr.value()->SetPortLinearPosition(it.value()->ppos);
            if (itr.value()->GetRemoteItem()==NULL) {
                itr.value()->SetEndPosition(it.value()->epos);
            }
        }
        p_data=it.value();
        delete p_data;
    }
    _provided_tmp_items.clear();
    _consumed_tmp_items.clear();

}

