/**
@file GraphicsResources.cpp
@brief Resources Graphique
@author F.Bighelli
 */

#include "GraphicsResources.h"

#include "Main/SwApplication.h"
#include "Main/SwMacros.h"

using namespace StreamWork::SwCore;

//-----------------------------------------------------------------------
GraphicsResources::GraphicsResources()
{
	_icone_admin=QIcon(":/StreamWorkEditor/settings.png").pixmap(11,14);
	_icone_run=QIcon(":/StreamWorkEditor/mod_run.png").pixmap(11,14);
    _icone_run_disable=QIcon(":/StreamWorkEditor/mod_run_disable.png").pixmap(11,14);
    _icone_replay=QIcon(":/StreamWorkEditor/mod_replay.png").pixmap(11,14);
	_icone_desactive=QIcon(":/StreamWorkEditor/editdelete.png").pixmap(14,14);
	_icone_desactive=QIcon(":/StreamWorkEditor/editdelete.png").pixmap(14,14);
    _view=0;
}

//-----------------------------------------------------------------------
GraphicsResources::~GraphicsResources() 
{

}

//-----------------------------------------------------------------------
GraphicsResources * GraphicsResources::getInstance() 
{
    static GraphicsResources instance;
    return &instance;
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getComponentIcon(StreamWork::SwCore::SwComponent_Class * component) 
{
    QIcon ico=SW_APP->ComponentsBank().GetComponentIcon(component->GetFactoryName(), component->GetFactoryComponentName());
    return ico.pixmap(16,16);
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getRunIcon()
{
    return _icone_run;
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getAdminIcon()
{
	return _icone_admin;
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getRunIconDisabled() 
{
    return _icone_run_disable;
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getReplayIcon() 
{
    return _icone_replay;
}

//-----------------------------------------------------------------------
QPixmap GraphicsResources::getDesactiveIcon() 
{
    return _icone_desactive;
}

//-----------------------------------------------------------------------
QColor GraphicsResources::getInterfaceColor(QString type)
{
    return QColor("#330066");
}


