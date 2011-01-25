/**
@file MenuManager.cpp
@brief manager de menu
@author F.Bighelli
 */

#include "MenuManager.h"

#include <QColorDialog>
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
    _copyTextColor=TEXT_COLOR;
    _copyBgColor=HEADER_COLOR;


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
    buildMenuForContext(_contextualMenu);
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
    buildMenuForContext(_contextualMenu);
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
    if (_gwList.count()==0 && _lkList.count()==0) {
        menu->addAction("Add interest area",this,SLOT(onAddInterestArea()));
        menu->addAction("Change background",this,SLOT(onChangeBackGroundColor()));
    }
    if (_gwList.count()>0 || _iaList.count()>0) {
        menu->addAction("Remove",this,SLOT(onRemove()));
    }
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
