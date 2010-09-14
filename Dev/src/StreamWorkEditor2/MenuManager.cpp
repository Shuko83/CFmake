/**
@file MenuManager.cpp
@brief manager de menu
@author F.Bighelli
 */

#include "MenuManager.h"

#include <SwApplication.h>
#include <SwMacros.h>
#include <ISwInterfaces_Consumer.h>
#include <ISwPins_Manager.h>
#include <SwTime_ToolBox.h>

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

static MenuManager * _instance=0;


/** @brief Constructor */
MenuManager::MenuManager():QObject() {
	_menu=new QMenu(0);
    _menu->setTitle("Edition");
	_contextualMenu=new QMenu(0);
    _contextualMenu->setWindowOpacity(0.9);
    _contextualMenu->setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px}");
    _contextualMenuSwap=new QMenu(0);
    _contextualMenuSwap->setWindowOpacity(0.9);
    _contextualMenuSwap->setStyleSheet("* { border: 1px solid gray; background: #ccc; font-size:10px}");
    _disableSelectionChanged=false;
    _streamControler=0;
    menuNeedBeRebuild=true;

}
/** @brief Destructor */
MenuManager::~MenuManager() {
    delete _menu;
    delete _contextualMenu;
    delete _contextualMenuSwap;
}
/** @brief acces singleton */
MenuManager * MenuManager::getInstance() {
    if (_instance==0) {
        _instance=new MenuManager();
    }
    return _instance;
}
/** @brief Definition du controleur */
void MenuManager::setControler(StreamControler * controler) {
    if (controler==_streamControler) {
        return;
    }
    if (_streamControler!=0) {
        _contextualMenu->clear();
        _contextualMenuSwap->clear();
        _gwList.clear();
        _lkList.clear();
        _adminList.clear();
        _exeList.clear();
    }
    _streamControler=controler;
    _menu->clear();
    if (_streamControler!=0) {
        selectionChanged();
    }
}
/** @brief selection changed */
void MenuManager::selectionChanged() {
    menuNeedBeRebuild=true;
}
/** @brief rebuildMenu() */
void MenuManager::rebuildMenu() {
    QList<QGraphicsItem *> list=_streamControler->getScene()->selectedItems();
    _gwList.clear();
    _lkList.clear();
    _adminList.clear();
    _exeList.clear();
    _iaList.clear();
    for(int i=0;i<list.count();i++) {
        ComponentGraphicItem * gcomponent=dynamic_cast<ComponentGraphicItem *>(list[i]);
        if (gcomponent!=0) {
            _gwList.push_back(gcomponent);
            ISwExecution_Service * executor=
                dynamic_cast<ISwExecution_Service *>(gcomponent->getComponent()->QueryService(CG_SW_SERVICE_EXECUTION));
            if(executor!=0) {
                _exeList.push_back(executor);
            }
            ISwAdminSetup * admin=
                dynamic_cast<ISwAdminSetup *>(gcomponent->getComponent());
            if(admin!=0) {
                _adminList.push_back(admin);
            }
        } else {
            LinkGraphicItem *glink=dynamic_cast<LinkGraphicItem *>(list[i]);
            if (glink!=0) {
                _lkList.push_back(glink);
            } else {
                InterestArea * ia=dynamic_cast<InterestArea *>(list[i]);
                if (ia!=0) {
                    _iaList.push_back(ia); 
                }
            }
        }
    }
    if (_gwList.count()>0 || _iaList.count()>0) {
        _contextualMenu->addAction("Remove",this,SLOT(onRemove()));
    }
    if (_lkList.count()>0) {
        _contextualMenu->addAction("Disconnect",this,SLOT(onDisconnect()));
    }
    if (_exeList.count()>0) {
        _contextualMenu->addAction("Start",this,SLOT(onStartExecution()));
        _contextualMenu->addAction("Stop",this,SLOT(onStopExecution()));
    }
    if (_adminList.count()>0) {
        _contextualMenu->addAction("Setup",this,SLOT(onSetup()));
    }
    _contextualMenu->addAction("Add interest area",this,SLOT(onAddInterestArea()));
    menuNeedBeRebuild=false;
}

/** @brief construction du menu en fonction du contexte */
QMenu * MenuManager::buildContextMenu(const QPointF & pos) {
    if (menuNeedBeRebuild) {
        rebuildMenu();
    }
    _menuPosition=pos;
    if (!_contextualMenu->isEmpty()) {
        QMenu * tmp=_contextualMenuSwap;
        _contextualMenuSwap=_contextualMenu;
        _contextualMenu=tmp;
        _contextualMenu->clear();
    } 
    if (_gwList.count()>0 || _iaList.count()>0) {
        _contextualMenu->addAction("Remove",this,SLOT(onRemove()));
    }
    if (_lkList.count()>0) {
        _contextualMenu->addAction("Disconnect",this,SLOT(onDisconnect()));
    }
    if (_exeList.count()>0) {
        _contextualMenu->addAction("Start",this,SLOT(onStartExecution()));
        _contextualMenu->addAction("Stop",this,SLOT(onStopExecution()));
    }
    if (_adminList.count()>0) {
        _contextualMenu->addAction("Setup",this,SLOT(onSetup()));
    }
    if (_gwList.count()>0) {
        _contextualMenu->addAction("Activate",this,SLOT(onActivated()));
        _contextualMenu->addAction("DesActivate",this,SLOT(onDeActivated()));
    }

    _contextualMenu->addAction("Add interest area",this,SLOT(onAddInterestArea()));
    return _contextualMenu;
}
/** @brief sur remove */
void MenuManager::onRemove() {
    QList<InterestArea *> ialist=_iaList;
    for(int i=0;i<ialist.count();i++) {
        InterestArea * ia=ialist.at(i);
        ia->setParentItem(0);
        _streamControler->getScene()->removeItem(ia);
        delete ialist.at(i);
    }

    QList<SwComponent_Class *> clist;
    for(int i=0;i<_gwList.count();i++) {
        clist.push_back(_gwList[i]->getComponent());
    }
    SwComponent_Class * component;
    for(int i=0;i<_gwList.count();i++) {
        component=_gwList[i]->getComponent();
        while (component->GetParent()!=0 && clist.indexOf(component->GetParent())<0) {
            component=component->GetParent();
        }
        if (clist.indexOf(component->GetParent())>=0) {
            clist.removeAt(clist.indexOf(_gwList[i]->getComponent()));
        }
    }
    for(int i=0;i<clist.count();i++) {
        component=clist[i]->GetParent();
        component->RemoveChild(clist[i]);
    }
    _streamControler->streamControlerChanged();
}
/** @brief sur deconnection */
void MenuManager::onDisconnect() {
    SwComponent_Class * component;
    for(int i=0;i<_lkList.count();i++) {
        component=((ComponentGraphicItem *)_lkList[i]->getSource()->parentItem())->getComponent();
        if (_lkList[i]->getSource()->getConnectorType()==CONSUMER) {
            ISwInterfaces_Consumer *iconsumer=
                dynamic_cast<ISwInterfaces_Consumer *>(component->QueryService(CG_SW_SERVICE_INTERFACES_CONSUMER));
            iconsumer->DetachProvider(_lkList[i]->getSource()->getName());
        } else if (_lkList[i]->getSource()->getConnectorType()==PIN) {
            ISwPins_Manager *pinManager=dynamic_cast<ISwPins_Manager *>(component->QueryService(CG_SW_SERVICE_PINS_MANAGER));
            if (pinManager!=0) {
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
