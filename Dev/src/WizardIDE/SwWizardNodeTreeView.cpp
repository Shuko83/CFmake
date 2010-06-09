/*!
\file SwWizardNodeTreeView.cpp
\date 12/04/2006
\brief view pour le model de noeuds
\author  Big
\version 1.0
 */
#include <QMetaObject>
#include <QModelIndex>
#include <QMenu>
#include <QMetaMethod>
#include <QAction>
#include "SwWizardNodeTreeView.h" 
#include "SwSimpleComponent_Class.h"

/*! \brief Constructeur */
SwWizardNodeTreeView::SwWizardNodeTreeView(QWidget * parent):QTreeView(parent){
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(OnContextMenuRequested( const QPoint &)));
    setContextMenuPolicy(Qt::CustomContextMenu);
}
/*! \brief Sur demande de menu contextuel */
void SwWizardNodeTreeView::OnContextMenuRequested( const QPoint & pos) {
    QModelIndex index=indexAt(pos);
    if (!index.isValid()) {
        return;
    }
    QPoint globalpos=viewport()->mapToGlobal(pos);
    emit OnMenuRequested(globalpos,index);   
}
