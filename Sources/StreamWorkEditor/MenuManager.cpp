/**
@file MenuManager.cpp
@brief manager de menu
@author F.Bighelli
 */
#include <QtWidgets>

#include "MenuManager.h"

#include <QColorDialog>
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "Component/Services/ISwInterfaces_Consumer.h"
#include "Component/Services/ISwPins_Manager.h"
#include "Tools/SwTime_ToolBox.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "ISwServiceExtensions.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/** @brief Constructor */
MenuManager::MenuManager():QObject() 
{
	_contextualMenu=new QMenu(0);
    _contextualMenu->setWindowOpacity(0.9);
    _contextualMenu->setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px}");
    _disableSelectionChanged=false;
    _streamControler=0;
    _copyTextColor=TEXT_COLOR;
    _copyBgColor=HEADER_COLOR;
}
/** @brief Destructor */
MenuManager::~MenuManager() 
{
    delete _contextualMenu;
}
/** @brief acces singleton */
MenuManager * MenuManager::getInstance() {
    static MenuManager instance;
    return &instance;
}
/** @brief Definition du controleur */
void MenuManager::setControler(StreamControler * controler) {
    if (controler==_streamControler) {
        return;
    }
    if (_streamControler!=0) {
        _contextualMenu->clear();
        _gwList.clear();
        _lkList.clear();
		_adminList.clear();
		_propertyList.clear();
        _exeList.clear();
        _connectorList.clear();
    }
    _streamControler=controler;
}
/** @brief acces au controleur */
StreamControler * MenuManager::getControler() {
    return _streamControler;
}

/** @brief rebuildMenu() */
void MenuManager::rebuildMenu() {
	_contextualMenu->clear();

    QList<QGraphicsItem *> list=_streamControler->getScene()->selectedItems();
    _gwList.clear();
    _lkList.clear();
    _adminList.clear();
	_propertyList.clear();
	_exeList.clear();
    _iaList.clear();
    _connectorList.clear();
    for(int i=0;i<list.count();i++) 
	{
        ComponentGraphicItem * gcomponent=dynamic_cast<ComponentGraphicItem *>(list[i]);
        if (gcomponent!=0)
		{
            _gwList.push_back(gcomponent);
            ISwExecution_Service * executor= dynamic_cast<ISwExecution_Service *>(gcomponent->getComponent()->QueryService(CG_SW_SERVICE_EXECUTION));
            if(executor!=0) 
			{
                _exeList.push_back(executor);
            }
            ISwAdminSetup * admin= dynamic_cast<ISwAdminSetup *>(gcomponent->getComponent());
            if(admin!=0) 
			{
                _adminList.push_back(admin);
            }

			//AAY Ajout
			StreamWork::SwCore::SwComponent_Class* cp = gcomponent->getComponent();
			if( cp != NULL)
			{
				SwProperties_Class *serv = dynamic_cast<SwProperties_Class *>(cp->QueryService(CG_SW_SERVICE_PROPERTIES));
				if (NULL != serv)
				{
					//parcours les propriétés et cherche si "visible est contenu dans le text"
					QList<ISwProperty*>prop = serv->GetProperties();
					for (ISwProperty *swprop : prop)
					{
						if(swprop && swprop->GetName().contains("visible"))
							_propertyList.push_back(swprop);
					}
				}
			}
        } else 
		{
            LinkGraphicItem *glink=dynamic_cast<LinkGraphicItem *>(list[i]);
            if (glink!=0) 
			{
                _lkList.push_back(glink);
            } 
			else 
			{
                InterestArea * ia=dynamic_cast<InterestArea *>(list[i]);
                if (ia!=0)
				{
                    _iaList.push_back(ia); 
                } else {
                    ConnectorGraphicItem * connectorItem=dynamic_cast<ConnectorGraphicItem *>(list[i]);
                    if (connectorItem!=0) {
                        _connectorList.push_back(connectorItem);
                    }
                }
            }
        }
    }
    buildMenuForContext(_contextualMenu);
}

/** @brief construction du menu en fonction du contexte */
QMenu * MenuManager::buildContextMenu(const QPointF & pos) {
    _menuPosition=pos;

    rebuildMenu();
	return _contextualMenu;
}
/** @brief buildMenuForContext() */
void MenuManager::buildMenuForContext(QMenu * menu) {
    if (_lkList.count()>0) {
        menu->addAction("Disconnect",this,SLOT(onDisconnect()));
    }
    if (_exeList.count()>0) {
        menu->addAction("Start",this,SLOT(onStartExecution()));
        menu->addAction("Stop",this,SLOT(onStopExecution()));
    }
    if (_adminList.count()>0) {
        menu->addAction("Setup",this,SLOT(onSetup()));
    }
    if (_gwList.count()>0) {
        menu->addAction("Activate",this,SLOT(onActivated()));
        menu->addAction("DesActivate",this,SLOT(onDeActivated()));
    }
    if (_gwList.count()==1) {
        menu->addAction("Copy style",this,SLOT(onCopyStyle()));
    }
    if (_gwList.count()>0) {
        menu->addAction("Paste style",this,SLOT(onPasteStyle()));
    }
    if (_gwList.count()>1) {
        menu->addAction("Create new model from selection",this,SLOT(onCreateNewModel()));
    }
    if (_gwList.count()==0 && _lkList.count()==0 && _connectorList.count()==0) {
        menu->addAction("Add interest area",this,SLOT(onAddInterestArea()));
        //menu->addAction("Change background",this,SLOT(onChangeBackGroundColor()));
    }
    if (_gwList.count()>0 || _iaList.count()>0) {
        menu->addAction("Remove",this,SLOT(onRemove()));
    }

	if (_propertyList.count()>0) 
	{
		menu->addSeparator();

		//Gestion de show et du hide
		bool allVisible = true;
		for (ISwProperty* prop : _propertyList)
		{
			if(prop->GetValue().toBool() == false)
				allVisible = false;
		}

		if(allVisible)
			menu->addAction("Hide",this,SLOT(onShow()));
		else
			menu->addAction("Show",this,SLOT(onShow()));
	}
    if (_gwList.count()==0 && _lkList.count()==0 && _connectorList.count()==1) {
        buildMenuForConnector(menu,_connectorList[0]);
    } else {
        _selectedConnector=0;
    }
}
/** @brief sur remove */
void MenuManager::onRemove() 
{
    QList<InterestArea *> ialist=_iaList;
    for(int i=0;i<ialist.count();i++) 
	{
        InterestArea * ia=ialist.at(i);
        ia->setParentItem(0);

		while ( ia->childItems().count() != 0 )
		{
			QGraphicsItem * item = ia->childItems().first();
            if (item)
            {
			    if (! dynamic_cast<QGraphicsTextItem*>(item))
			    {
				    QPointF pt = item->scenePos();
				    item->setParentItem(NULL);
				    item->setPos(pt);
			    }
			    else
			    {
				    item->setParentItem(NULL);
				    _streamControler->getScene()->removeItem(item);
			    }
            }
		}
        _streamControler->getScene()->removeItem(ia);
        delete ialist.at(i);
    }

    QList<SwComponent_Class *> clist;
    for(int i=0;i<_gwList.count();i++) 
	{
        clist.push_back(_gwList[i]->getComponent());
    }
    SwComponent_Class * component;
    for(int i=0;i<_gwList.count();i++) 
	{
        component=_gwList[i]->getComponent();
        while (component->GetParent()!=0 && clist.indexOf(component->GetParent())<0) 
		{
            component=component->GetParent();
        }

        if (clist.indexOf(component->GetParent())>=0) 
		{
            clist.removeAt(clist.indexOf(_gwList[i]->getComponent()));
        }
    }
    for(int i=0;i<clist.count();i++) 
	{
        component=clist[i]->GetParent();
		component->RemoveChild(clist[i]);
    }
    _streamControler->streamControlerChanged();
}
/** @brief sur deconnection */
void MenuManager::onDisconnect() {
    SwComponent_Class * component;
    if (_selectedConnector!=0) {
        _lkList.append(*(_selectedConnector->getLinks()));
    }
    for(int i=0;i<_lkList.count();i++) 
	{
        component=((ComponentGraphicItem *)_lkList[i]->getSource()->parentItem())->getComponent();

		//Bridage de deconnection d'interface sur un composant exécuté
		StreamWork::SwExecution::ISwExecutable_Service * execServS = dynamic_cast<StreamWork::SwExecution::ISwExecutable_Service*>(component->QueryService(CG_SW_SERVICE_EXECUTABLE));
		if( execServS && execServS->isRunning() ) 
		{
			QMessageBox::critical(0,"Forbidden Action","Unable to disconnect interface on running component");
			return;
		}

        if (_lkList[i]->getSource()->getConnectorType()==CONSUMER) 
		{
            ISwInterfaces_Consumer *iconsumer = dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
            iconsumer->DetachProvider(_lkList[i]->getSource()->getName());
        } 
		else if (_lkList[i]->getSource()->getConnectorType()==PIN) 
		{
            ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
            if (pinManager!=0) 
			{
                pinManager->DisconnectPin(_lkList[i]->getSource()->getName());
            }
        }
    }
}
/** @brief sur start execution*/
void MenuManager::onStartExecution() {
    QList<StreamWork::SwExecution::ISwExecution_Service *>::iterator it;
    it=_exeList.begin();
    while (it!=_exeList.end()) {
        (*it)->StartExecution();
        it++;
    }
    _streamControler->getScene()->update();
}
/** @brief sur stop execution*/
void MenuManager::onStopExecution(){
    QList<StreamWork::SwExecution::ISwExecution_Service *>::iterator it;
    it=_exeList.begin();
    while (it!=_exeList.end()) {
        (*it)->AskForStopExecution();
        while (!(*it)->IsExecutionStopped()) {
            SwTime_ToolBox::InternalSleep(10);    
        }
        (*it)->StopExecution();
        it++;
    }
    _streamControler->getScene()->update();
}
/** @brief sur setup */
void MenuManager::onSetup(){
    QList<StreamWork::SwCore::ISwAdminSetup *>::iterator it;
    it=_adminList.begin();
    while (it!=_adminList.end()) {
        (*it)->AdminSetup();
        it++;
    }
    _streamControler->getScene()->update();
}

//-------------------------------------------------------------------------
void MenuManager::onShow()
{
	QList<StreamWork::SwCore::ISwProperty *>::iterator it;
	it=_propertyList.begin();
	while (it!=_propertyList.end()) 
	{
		(*it)->SetValue(!((*it)->GetValue().toBool()));
		it++;
	}
	_streamControler->getScene()->update();
}

/** @brief sur ajout de la zone d'interet */
void MenuManager::onAddInterestArea() {
    InterestArea * ia=new InterestArea(_streamControler);
    ia->setPos(_menuPosition);
    _streamControler->getScene()->addItem(ia);
    _streamControler->streamControlerChanged();
}
/** @brief activated */
void MenuManager::onActivated() {
    for(int i=0;i<_gwList.count();i++) {
        _gwList[i]->getComponent()->setActive(true);
    }
    _streamControler->getScene()->update();
}
/** @brief activated */
void MenuManager::onDeActivated() {
    for(int i=0;i<_gwList.count();i++) {
        _gwList[i]->getComponent()->setActive(false);
    }
    _streamControler->getScene()->update();
}
/** @brief on copy style */
void MenuManager::onCopyStyle() {
    ComponentGraphicItem * cg=_gwList[0];
    _copyTextColor=cg->getTextColor();
    _copyBgColor=cg->getColor();
}
/** @brief on paste style */
void MenuManager::onPasteStyle() {
    for(int i=0;i<_gwList.count();i++) {
        _gwList[i]->setTextColor(_copyTextColor);
        _gwList[i]->setColor(_copyBgColor);
    }
}
/** @brief on change background color */
void MenuManager::onChangeBackGroundColor() {
    QColor c=_streamControler->getView()->backgroundBrush().color();
    c=QColorDialog::getColor(c,0,"Select background color");
    _streamControler->getScene()->setBackgroundBrush(QBrush(c));
    _streamControler->getView()->setBackgroundBrush(QBrush(c));
}
/** @brief construit le menu pour le connector */
void MenuManager::buildMenuForConnector(QMenu * menu,ConnectorGraphicItem * connector) {
    _selectedConnector=connector;
    QList<LinkGraphicItem *> * links=connector->getLinks();
    if (links->size()==1) {
        menu->addAction("Jump to connected",this,SLOT(onJumpToConnected()));
        menu->addAction("Disconnect",this,SLOT(onDisconnect()));
        return;
    }
    if (links->size()==0 && ( _selectedConnector->getConnectorType()==CONSUMER || _selectedConnector->getConnectorType()==PIN)) {
        _connectableItems.clear();
        QMenu *submenu=menu->addMenu(QIcon(),"Connect to");
        QList<QGraphicsItem *> litems=_streamControler->getScene()->items();
        QList<QGraphicsItem *>::iterator it=litems.begin();
        while (it!=litems.end()) {
            ConnectorGraphicItem *citem=dynamic_cast<ConnectorGraphicItem *>(*it);
            if (citem!=0 && citem!=_selectedConnector) {
                bool testConnectable=(_selectedConnector->getConnectorType()==CONSUMER && citem->getConnectorType()==PROVIDER);
                testConnectable = testConnectable || (_selectedConnector->getConnectorType()==PIN && citem->getConnectorType()==PIN && citem->getLinks()->isEmpty());
                testConnectable=testConnectable && citem->getModelType()==_selectedConnector->getModelType();
                testConnectable=testConnectable && citem->parentItem()!=_selectedConnector->parentItem();
                if (testConnectable) {
                    _connectableItems.push_back(citem);
                    ComponentGraphicItem *parentitem=dynamic_cast<ComponentGraphicItem *>(citem->parentItem());
                    QIcon icon=SW_APP->ComponentsBank().GetComponentIcon(parentitem->getComponent()->GetFactoryName(), parentitem->getComponent()->GetFactoryComponentName());
                    QAction * action=submenu->addAction(icon,buildActionNameForConnector(citem),this,SLOT(onConnect()));
                    action->setToolTip(SW_APP->ComponentsBank().GetComponentDescription(parentitem->getComponent()->GetFactoryName(), parentitem->getComponent()->GetFactoryComponentName()));
                } 
            }
            it++;
        }
    }
    if (links->size()==0 && _selectedConnector->getConnectorType()==CONSUMER) {
        ISwServiceExtensions * serviceExtension=dynamic_cast<ISwServiceExtensions *>(SW_APP->QueryService(CG_SW_SERVICE_EXTENSION_MANAGER));
		if (serviceExtension)
		{
			QList<ISwExtension *> availableExtensions = serviceExtension->getExtensionWithType(_selectedConnector->getModelType());
			QMenu *submenu = menu->addMenu(QIcon(), "Create and connect to");
			for (ISwExtension * ext : availableExtensions) {
				QIcon icon = SW_APP->ComponentsBank().GetComponentIcon(ext->getPluginName(), ext->getComponentType());
				QAction * action = submenu->addAction(icon, ext->getComponentType() + "." + ext->getName(), this, SLOT(onCreateAndConnect()));
				action->setToolTip(SW_APP->ComponentsBank().GetComponentDescription(ext->getPluginName(), ext->getComponentType()));
			}
		}
    }

}
/** @brief on jump */
void MenuManager::onJumpToConnected() {
    QList<LinkGraphicItem *> *links=_selectedConnector->getLinks();
    LinkGraphicItem *lk=links->at(0);
    ConnectorGraphicItem *target=(lk->getSource()==_selectedConnector?lk->getTarget():lk->getSource());
	_streamControler->getScene()->clearSelection();
	if(target)
	{
    	_streamControler->getView()->centerOn(target);
    	target->setSelected(true);
	}
}
/** @brief sur Connection */
void MenuManager::onCreateAndConnect() {
    QObject * object=sender();
    QAction * action=dynamic_cast<QAction *>(object);
    if (action!=0) {
        ISwServiceExtensions * serviceExtension=dynamic_cast<ISwServiceExtensions *>(SW_APP->QueryService(CG_SW_SERVICE_EXTENSION_MANAGER));
        QList<ISwExtension *> availableExtensions=serviceExtension->getExtensionWithType(_selectedConnector->getModelType());
		for (ISwExtension * ext : availableExtensions) {
            if ((ext->getComponentType()+"."+ext->getName())==action->text()) {
                QPointF spos=_selectedConnector->mapToScene(0.0,0.0);
                spos.setX(spos.x()+200);

                _streamControler->addComponent(ext->getPluginName(), ext->getComponentType(),spos);
                ComponentGraphicItem * gitem=_streamControler->getLastAddedComponent();
                ConnectorGraphicItem *citem=gitem->getConnector(ext->getName());
                _streamControler->onLinkConnectors(_selectedConnector,citem);
                return;
            }
        }
    }
    

}

/** @brief sur Connection */
void MenuManager::onConnect() {
    QObject * object=sender();
    QAction * action=dynamic_cast<QAction *>(object);
    if (action!=0) {
		for (ConnectorGraphicItem *citem : _connectableItems) {
            if (buildActionNameForConnector(citem)==action->text()) {
                _streamControler->onLinkConnectors(_selectedConnector,citem);
                return;
            }
        }
    }
    
}
/** @brief build action name for citem */
QString MenuManager::buildActionNameForConnector(ConnectorGraphicItem *citem) {
    ComponentGraphicItem *parentitem=dynamic_cast<ComponentGraphicItem *>(citem->parentItem());

    QString name;
    if (citem->getLinks()->size()>0) {
        name=parentitem->getComponent()->GetName()+"."+citem->getName()+" ("+QString("%1").arg(citem->getLinks()->size())+")";
    } else {
        name=parentitem->getComponent()->GetName()+"."+citem->getName();
    }
    return name;
}

/** @brief on create new model */
void MenuManager::onCreateNewModel() {
    bool ok;
    QString modelName= QInputDialog::getText(0, "Enter name of model",
        "Model name:", QLineEdit::Normal,
        "Undefined_Model", &ok);
    if (ok && !modelName.isEmpty()) {
        QList<SwComponent_Class *> clist;
        for(int i=0;i<_gwList.count();i++) {
            clist.push_back(_gwList[i]->getComponent());
        }
        _streamControler->createModelFromSelection(clist,modelName);
    }
}
