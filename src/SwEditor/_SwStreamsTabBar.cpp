/*!
 \file _SwStreamsTabBar.h
 \brief Implementation of the Class _SwStreamsTabBar qui surcharge une tabbar pour la gestion des streams
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/


#include "_SwStreamsTabBar.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;


_SwStreamsTabBar::_SwStreamsTabBar(QWidget *parent)
	: QTabBar(parent)
{
    _manager=NULL;
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(OnIndexChanged(int)));
    _rebuild_enabled=false;
    this->setDisabled(true);
}

_SwStreamsTabBar::~_SwStreamsTabBar()
{
}
/*! \brief Attach un stream manager */
void _SwStreamsTabBar::AttachStreamsManager(ISwEditorStreamsManager * manager) {
    _manager=manager;
    _manager->AttachStreamManagerObserver(this);
    _rebuild_enabled=true;
    this->setDisabled(false);
    BuildWidget();
}
/*! \briefdetach un stream manager */
void _SwStreamsTabBar::DetachStreamsManager() {
    _rebuild_enabled=false;
    this->setDisabled(true);
    _manager->DetachStreamManagerObserver(this);   
}
//---------------------------------------------------------------------
// Autres
//---------------------------------------------------------------------
/*! \brief(Re)Construction du widget */
void _SwStreamsTabBar::BuildWidget() {
    if (!_rebuild_enabled)
        return;
    _rebuild_enabled=false; //Eviter la reentrance
    if (_manager==NULL)
        return;
    if (_manager->GetStreamsCount()<this->count()) {
        while (this->count()>_manager->GetStreamsCount()) this->removeTab(this->count()-1);
    }
    for(int i=0;i<_manager->GetStreamsCount();i++) {
        if (i<this->count())
            this->setTabText(i,_manager->GetStreamDescription(i).toString());
        else 
            this->addTab(_manager->GetStreamDescription(i).toString());
    }
    this->setCurrentIndex(_manager->GetCurrentStream());
    _rebuild_enabled=true;
}
/*! \brief sur changement de l'index*/
void _SwStreamsTabBar::OnIndexChanged(int index) {
    _rebuild_enabled=false;
    _manager->SetCurrentStream(index);
    _rebuild_enabled=true;
}
//---------------------------------------------------------------------
// Interface ISwObserver
//---------------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwStreamsTabBar::Update() {
    BuildWidget();    
}

