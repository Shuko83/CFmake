/*!
\file _SwModelHostExportSelector.cpp
\date 12/04/2006
\brief  Ihm de selection des entites export�es par le model host
\author  Big
\version 1.0
 */

#include <SwApplication.h>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
#include <SwAddress_ToolBox.h>
#include "_SwConfigurationSelector.h"
#include "_SwConfigurationStreamTreeModel.h"
#include "_SwConfigurationEntitiesModel.h"
#include "_SwConfigurationTreeView.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwConfigurationSelector::_SwConfigurationSelector(QWidget *parent,SwComponent_Class * root_element,QList<_SwConfigurationExportedEntity *> * entities_list)
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

    _SwConfigurationTreeView *view_model=new _SwConfigurationTreeView(this);
    _entities_model=new _SwConfigurationEntitiesModel(view_model,_root_comp,_entities_list);
    view_model->setModel(_entities_model);
    view_model->setAcceptDrops(true);
    view_model->setDragDropMode(QAbstractItemView::DropOnly);
    //view_model->setSelectionMode(QAbstractItemView::ExtendedSelection);
    splitter->addWidget(view_model);
    QTreeView *view=new QTreeView(this);
    _SwConfigurationStreamTreeModel * model=new _SwConfigurationStreamTreeModel(view,_root_comp);
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
_SwConfigurationSelector::~_SwConfigurationSelector()
{

}
/*! \brief validation des changement */
void _SwConfigurationSelector::ValidChange() {
    _entities_model->TransformModelToEntities();
}
