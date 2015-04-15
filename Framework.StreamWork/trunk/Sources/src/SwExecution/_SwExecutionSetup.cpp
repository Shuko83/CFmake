/*!
\file _SwExecutionSetup.cpp
\date 12/04/2006
\brief Ihm de configuration de l'execution
\author  Big
\version 1.0
 */

#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>
#include <SwAddress_ToolBox.h>
#include "_SwExecutorStreamTreeModel.h"
#include "_SwExecutorListEditor.h"
#include "_SwExecutionSetup.h"

/*! \brief Constructeur */
_SwExecutionSetup::_SwExecutionSetup(QWidget *parent,SwComponent_Class * root_component,QList<QString> * current_list,QList<_SwExecutionMode> * current_mode_list)
    : QDialog(parent)
{
    _root_component=root_component;
    for (int i=0;i<(*current_list).count();i++) {
        if (SwAddress_ToolBox::FindTarget((*current_list)[i],root_component)!=NULL) {
            _exe_paths.push_back((*current_list)[i]);
            _exe_modes.push_back((*current_mode_list)[i]);
        }
    }    
    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(400,400));

    /* Composant de l'ihm */
    QVBoxLayout * main_layout=new QVBoxLayout(this);
    this->setLayout(main_layout);
    QHBoxLayout * data_layout=new QHBoxLayout(this);
    QHBoxLayout * button_layout=new QHBoxLayout(this);
    _SwExecutorListEditor * list_view=new _SwExecutorListEditor(this,_root_component,&_exe_paths,&_exe_modes);
    QScrollArea * sa=new QScrollArea(this);
    sa->setWidget(list_view);
    sa->resize(200,400);
    _SwExecutorStreamTreeModel *model=new _SwExecutorStreamTreeModel(this,_root_component);
    list_view->connect(list_view,SIGNAL(OnExeAdded ( const QStringList)),model,SLOT(OnExeAdded ( const QStringList)));
    list_view->connect(list_view,SIGNAL(OnExeRemoved ( const QStringList)),model,SLOT(OnExeRemoved ( const QStringList)));
    list_view->UpdateList();
    QTreeView * view=new QTreeView(this);
    view->setModel(model);
    view->setDragEnabled(true);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QSplitter * splitter=new QSplitter(this);
    splitter->addWidget(sa);
    splitter->addWidget(view);
    data_layout->addWidget(splitter);
    main_layout->addLayout(data_layout);
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
_SwExecutionSetup::~_SwExecutionSetup()
{

}
/*! \brief Permet d'acceder a la nouvelle liste */
const QList<QString> * _SwExecutionSetup::GetEditedList() {
    return &_exe_paths;
}
/*! \brief Permet d'acceder a la nouvelle liste mode */
const QList<_SwExecutionMode> * _SwExecutionSetup::GetEditedListMode() {
    return &_exe_modes;
}
