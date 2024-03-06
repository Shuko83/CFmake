/*!
\file _SwModelHostExportSelector.cpp
\date 12/04/2006
\brief  Ihm de selection des entites export�es par le model host
\author  Big
\version 1.0
 */

#include "Main/SwApplication.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
#include "Tools/SwAddress_ToolBox.h"
#include "_SwModelHostExportSelector.h"
#include "_SwExportStreamTreeModel.h"
#include "_SwModelExportedEntitiesModel.h"
#include "_SwModelExportTreeView.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelHostExportSelector::_SwModelHostExportSelector(QWidget *parent,SwComponent_Class * root_element,QList<_SwModelExportedEntity *> * entities_list)
    : QDialog(parent)
{
    setWindowTitle("Select entities to export");

    _root_comp=root_element;
    _entities_list=entities_list;

    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(800,600));

    /* Composant de l'ihm */
    QVBoxLayout * main_layout=new QVBoxLayout(this);
    this->setLayout(main_layout);

    //Ajout de l'arbre
    QSplitter * splitter=new QSplitter(Qt::Horizontal,this);

    _SwModelExportTreeview *view_model=new _SwModelExportTreeview(this);
    _entities_model=new _SwModelExportedEntitiesModel(view_model,_root_comp,_entities_list);
    view_model->setModel(_entities_model);
    view_model->setAcceptDrops(true);
    view_model->setDragDropMode(QAbstractItemView::DropOnly);
    //view_model->setSelectionMode(QAbstractItemView::ExtendedSelection);
    splitter->addWidget(view_model);
    QTreeView *view=new QTreeView(this);
    _SwExportStreamTreeModel * model=new _SwExportStreamTreeModel(view,_root_comp);
    view->setModel(model);
    view->setDragEnabled(true);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    splitter->addWidget(view);
    main_layout->addWidget(splitter);
    //Ajout des boutons
    QHBoxLayout * button_layout=new QHBoxLayout(this);
    main_layout->addLayout(button_layout);
    QPushButton *but=new QPushButton(QString("Valid"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(accept()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignLeft);
    but=new QPushButton(QString("Cancel"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(reject()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignRight);
}
/*! \brief Destructeur */
_SwModelHostExportSelector::~_SwModelHostExportSelector()
{

}
/*! \brief validation des changement */
void _SwModelHostExportSelector::ValidChange() {
    _entities_model->TransformModelToEntities();
}
