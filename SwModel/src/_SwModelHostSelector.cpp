/*!
\file _SwModelHostSelector.cpp
\date 12/04/2006
\brief Ihm de selection du model host
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
#include "_SwModelHostSelector.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModelHostSelector::_SwModelHostSelector(QWidget *parent,SwComponent_Class * selected_root_element,_SwModelHost_Class * selected_model_host)
    : QDialog(parent)
{
    int good_root_elt_index;

    setWindowTitle("Select model host");

    _selected_root_element=selected_root_element;
    _selected_model_host=selected_model_host;

    //Listage des composants racine
    SwComponent_Class * root_elt;
    root_elt=SW_APP->GetFirstStream();
    while (root_elt!=NULL) {
        _roots_list.push_back(root_elt);
        root_elt=SW_APP->GetNextStream();
    }

    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(400,400));

    /* Composant de l'ihm */
    QVBoxLayout * main_layout=new QVBoxLayout(this);
    this->setLayout(main_layout);

    //ComboBox
    QComboBox * combo=new QComboBox(this);
    good_root_elt_index=0;
    for (int i=0;i<_roots_list.count();i++) {
        QString s;
        s=_roots_list[i]->GetName()+" ["+_roots_list[i]->GetDescription()+"]";
        combo->addItem(s);
        if (_roots_list[i]==_selected_root_element) {
            good_root_elt_index=i;    
        }
    }
    if (_selected_root_element==NULL) {
        _selected_root_element=_roots_list[0];
    }
    combo->setCurrentIndex(good_root_elt_index);
    combo->connect(combo,SIGNAL(activated(int)),this,SLOT(OnComboChange(int)));
    main_layout->addWidget(combo);
    //Ajout de l'arbre
    _view=new QTreeView(this);
    _model=new _SwModelStreamTreeModel(_view,_selected_root_element);
    _view->setModel(_model);
    _model->SetSelected(_selected_model_host);
    _view->expandAll();
    _view->connect(_view,SIGNAL(clicked(const QModelIndex &)),this,SLOT(OnTreeViewClicked (const QModelIndex &)));
    main_layout->addWidget(_view);
    //Ajout des boutons
    QHBoxLayout * button_layout=new QHBoxLayout(this);
    main_layout->addLayout(button_layout);
    QPushButton *but=new QPushButton(QString("Valid"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(accept()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignLeft);
    if (_selected_model_host!=NULL) {
        but=new QPushButton(QString("Unbind"),this);
        but->connect(but,SIGNAL(clicked()),this,SLOT(OnUnBind()));
        button_layout->addWidget(but);
        button_layout->setAlignment(but,Qt::AlignCenter);        
    }
    but=new QPushButton(QString("Cancel"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(reject()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignRight);
}
/*! \brief Destructeur */
_SwModelHostSelector::~_SwModelHostSelector()
{

}
/*! \brief recuperation du composant racine*/
SwComponent_Class * _SwModelHostSelector::GetRootComponent() {
    return _selected_root_element;
}

/*! \brief recuperation du model host*/
_SwModelHost_Class * _SwModelHostSelector::GetModelHost() {
    return _selected_model_host;
}
/*! \brief Changement combo*/
void _SwModelHostSelector::OnComboChange(int index) {
    if (_selected_root_element!=_roots_list[index])
        _selected_model_host=NULL;
    _selected_root_element=_roots_list[index];
    _model=new _SwModelStreamTreeModel(_view,_selected_root_element);
    _model->SetSelected(_selected_model_host);
    _view->setModel(_model); 
    _view->expandAll();
}
/*! \brief Selection modele hote*/
void _SwModelHostSelector::OnTreeViewClicked (const QModelIndex & index) {
    if (index.isValid()) {
        SwComponent_Class * comp=(SwComponent_Class *)index.internalPointer();
        if (dynamic_cast<_SwModelHost_Class *>(comp)!=NULL) {
            _selected_model_host=(_SwModelHost_Class *)index.internalPointer();
            _model->SetSelected(_selected_model_host);
        }
    }
}
/*! \brief sur unbing */
void _SwModelHostSelector::OnUnBind() {
    _selected_model_host=NULL;
    _selected_root_element=NULL;
    accept(); 
}
