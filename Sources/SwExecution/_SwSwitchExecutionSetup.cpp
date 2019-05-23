/*!
\file _SwSwitchExecutionSetup.cpp
\date dec 2008
\brief Ihm de configuration de l'execution
\author  HLG
\version 1.0
 */

//#include <QString>

#include <SwAddress_ToolBox.h>
#include "_SwSwitchExecutionSetup.h"

/*! \brief Constructeur */
_SwSwitchExecutionSetup::_SwSwitchExecutionSetup(QWidget *parent,SwComponent_Class * root_component,QList<QString> * current_list,
                                                 _SwSwitchExecutionList * switchExecutionList)
    : QDialog(parent)
{
    deleteInProgress = false;
    addInProgress = false;
    _switchExecutionList = switchExecutionList;
    _root_component=root_component;

    if(_switchExecutionList->_exeListName.size() != 0) {
        ExeListMap::iterator it = _switchExecutionList->_exeListMap.find(_switchExecutionList->_list_name.ToString());
        ModeListMap::iterator itm = _switchExecutionList->_modeListMap.find(_switchExecutionList->_list_name.ToString());
        if(it != _switchExecutionList->_exeListMap.end() && itm != _switchExecutionList->_modeListMap.end()) {
            for(int i=0; i<it.value().size(); i++) {
                if(SwAddress_ToolBox::FindTarget(it.value()[i],_root_component) != NULL) {
                    _exe_paths.push_back(it.value()[i]);
                    _exe_modes.push_back(itm.value()[i]);
                }
            }
        }
    }

    //Construction de l'ihm
    setSizeGripEnabled(true);
    resize(QSize(400,400));

    /* Composant de l'ihm */
    main_layout = new QVBoxLayout(this);
    this->setLayout(main_layout);
    
    list_layout1  = new QHBoxLayout(this);
    list_layout2  = new QHBoxLayout(this);
    data_layout   = new QHBoxLayout(this);
    button_layout = new QHBoxLayout(this);
    
    list_view = new _SwExecutorListEditor(this,_root_component,&_exe_paths,&_exe_modes);    
    sa = new QScrollArea(this);
    sa->setWidget(list_view);
    sa->resize(200,400);    
    model = new _SwExecutorStreamTreeModel(this,_root_component);    
    list_view->connect(list_view,SIGNAL(OnExeAdded ( const QStringList)),model,SLOT(OnExeAdded ( const QStringList)));
    list_view->connect(list_view,SIGNAL(OnExeRemoved ( const QStringList)),model,SLOT(OnExeRemoved ( const QStringList)));
    list_view->UpdateList();
    
    view = new QTreeView(this);
    view->setModel(model);
    view->setDragEnabled(true);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    splitter = new QSplitter(this);
    splitter->addWidget(sa);
    splitter->addWidget(view);    
    data_layout->addWidget(splitter);
    
    but = new QPushButton(QString("Valid"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(accept()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignLeft);
    but = new QPushButton(QString("Cancel"),this);
    but->connect(but,SIGNAL(clicked()),this,SLOT(reject()));
    button_layout->addWidget(but);
    button_layout->setAlignment(but,Qt::AlignRight);

    
    combo_exeList = new QComboBox(this);
    combo_exeList->setObjectName(QString::fromUtf8("combo_exeList"));
    list_layout1->addWidget(combo_exeList);
    list_layout1->setAlignment(combo_exeList,Qt::AlignLeft);
    if(_switchExecutionList->_exeListName.size()!=0)
    {   
        ExeListName::iterator i = _switchExecutionList->_exeListName.begin();
        while(i != _switchExecutionList->_exeListName.end())
        {
            combo_exeList->addItem(i.value(), i.key());
            i++;
        }
    }
    combo_exeList->setCurrentIndex(_switchExecutionList->_list_name.ToInt());
    combo_exeList->connect(combo_exeList, SIGNAL(currentIndexChanged(int)), this, SLOT(CurrentIndexChanged(int)));


    button_deleteSelectedList = new QPushButton(QString("Delete Selected List"),this);
    button_deleteSelectedList->setObjectName(QString::fromUtf8("button_deleteSelectedList"));
    button_deleteSelectedList->connect(button_deleteSelectedList, SIGNAL(clicked()), this, SLOT(Clicked_deleteSelectedList()));
    list_layout1->addWidget(button_deleteSelectedList);
    list_layout1->setAlignment(button_deleteSelectedList,Qt::AlignRight);

    edit_newListName = new QLineEdit(this);
    edit_newListName->setObjectName(QString::fromUtf8("edit_newListName"));
    list_layout2->addWidget(edit_newListName);
    list_layout2->setAlignment(edit_newListName,Qt::AlignLeft);
    
    button_addNewListName = new QPushButton(QString("Add New List Name"),this);
    button_addNewListName->setObjectName(QString::fromUtf8("button_addNewListName"));
    button_addNewListName->connect(button_addNewListName, SIGNAL(clicked()), this, SLOT(Clicked_addNewListName()));
    list_layout2->addWidget(button_addNewListName);
    list_layout2->setAlignment(button_addNewListName,Qt::AlignRight);
    
    main_layout->addLayout(list_layout1);
    main_layout->addLayout(list_layout2);
    main_layout->addLayout(data_layout);
    main_layout->addLayout(button_layout);

    //rend les ihms de choix des executables inaccessibles
    //car il n'ya pas de liste pour le moment
    if(_switchExecutionList->_exeListName.size() == 0) {
        sa->setDisabled(true);
        view->setDisabled(true);
    }
}
/*! \brief Destructeur */
_SwSwitchExecutionSetup::~_SwSwitchExecutionSetup()
{

}
/*! \brief Permet d'acceder AUX MODES DE LA NOUVELLE LISTE */
const QList<_SwExecutionMode> * _SwSwitchExecutionSetup::GetEditedListModes() {
    return &_exe_modes;
}
/*! \brief Permet d'acceder a la nouvelle liste */
const QList<QString> * _SwSwitchExecutionSetup::GetEditedList() {
    return &_exe_paths;
}
void _SwSwitchExecutionSetup::Clicked_addNewListName(void) {
    
    addInProgress = true;
    //recuperation du nom
    QString newName = edit_newListName->text();
    
    if(newName.size() != 0) {
        //sauvegarde de la liste d'executables precedente
        SaveExePathInCurrentList();

        //clear
        _exe_paths.clear();
        _exe_modes.clear();

        // position de la liste dans la map des noms de liste
        int pos = _switchExecutionList->_exeListName.size(); 

        //ajout du nom dans la liste de nom
        _switchExecutionList->AddListName(pos, newName);
        edit_newListName->clear();
        
        //ajout du nom dans la combo
        combo_exeList->addItem(newName, pos);
        combo_exeList->setCurrentIndex(pos);
        
        //ajout d'une liste vide d'executable
        _switchExecutionList->AddExeList(newName, _exe_paths,_exe_modes);

        //update des ihms de choix des executables
        UpdateExecutableView();

        //rend les ihms de choix des executables accessibles
        sa->setDisabled(false);
        view->setDisabled(false);
    }
    addInProgress = false;

}
void _SwSwitchExecutionSetup::Clicked_deleteSelectedList(void) {

    deleteInProgress = true;
    int indexToDelete = _switchExecutionList->_list_name.ToInt();
    _switchExecutionList->DeleteList(indexToDelete);

    if(_switchExecutionList->_exeListName.size() != 0) {
        _exe_paths.clear();
        _exe_modes.clear();
        // mise à jour de la liste d'exe locale
        //recherche la liste d'executable correspondant
        QString tmp = _switchExecutionList->_list_name.ToString();
        ExeListMap::iterator it = _switchExecutionList->_exeListMap.find(tmp);
        ModeListMap::iterator itm = _switchExecutionList->_modeListMap.find(tmp);
        if(it!=_switchExecutionList->_exeListMap.end() && itm != _switchExecutionList->_modeListMap.end()) {
            //copie de la liste dans la liste locale
            for(int i=0; i<it.value().size(); i++) {
                if(SwAddress_ToolBox::FindTarget(it.value()[i],_root_component) != NULL) {
                    _exe_paths.push_back(it.value()[i]);
                    _exe_modes.push_back(itm.value()[i]);
                }
            }
        }

        //reconstruction de la combo box        
        ExeListName::iterator it2 = _switchExecutionList->_exeListName.begin();
        combo_exeList->clear();
        while(it2 != _switchExecutionList->_exeListName.end())
        {
            combo_exeList->addItem(it2.value(), it2.key());
            it2++;
        }
        
        //update des ihms de choix des executables
        UpdateExecutableView();
    }
    deleteInProgress = false;
    combo_exeList->setCurrentIndex(0);
}
void _SwSwitchExecutionSetup::CurrentIndexChanged(int index) {
    if(!deleteInProgress && !addInProgress) {
        //sauvegarde de la liste d'executables precedents
        SaveExePathInCurrentList();
        
        //clear
        _exe_paths.clear();
        _exe_modes.clear();
        //changement d'index dans la propriété
        _switchExecutionList->_list_name.FromInt(index);    
        QVariant _list_name_qvariant;
        _list_name_qvariant.setValue(_switchExecutionList->_list_name);
        _switchExecutionList->_list_name_property->SetValue(_list_name_qvariant);

        //nom de la liste nouvellement selectionne
        _switchExecutionList->_list_name.FromInt(index);
        //recherche la liste d'executable correspondant
        ExeListMap::iterator it = _switchExecutionList->_exeListMap.find(_switchExecutionList->_list_name.ToString());
        ModeListMap::iterator itm = _switchExecutionList->_modeListMap.find(_switchExecutionList->_list_name.ToString());
        if(it!=_switchExecutionList->_exeListMap.end() && itm != _switchExecutionList->_modeListMap.end()) {
            //copie de la liste dans la liste locale
            for(int i=0; i<it.value().count(); i++) {
                if(SwAddress_ToolBox::FindTarget(it.value()[i],_root_component) != NULL) {
                    _exe_paths.push_back(it.value()[i]);
                    _exe_modes.push_back(itm.value()[i]);
                }
            }
        }

        //update des ihms de choix des executables
        UpdateExecutableView();
    }

}
void _SwSwitchExecutionSetup::SaveExePathInCurrentList(void) {
    //sauvegarde
    if(_switchExecutionList->_exeListName.size()!=0) {
        QString currentName = _switchExecutionList->_list_name.ToString();
        _switchExecutionList->DeleteExeList(currentName);
        _switchExecutionList->AddExeList(currentName, _exe_paths,_exe_modes);
    }
}
void _SwSwitchExecutionSetup::UpdateExecutableView(void) {

    delete list_view;
    list_view = new _SwExecutorListEditor(this,_root_component,&_exe_paths,&_exe_modes);    
    sa->setWidget(list_view);

    delete model;
    model = new _SwExecutorStreamTreeModel(this,_root_component);    
    list_view->connect(list_view,SIGNAL(OnExeAdded ( const QStringList)),model,SLOT(OnExeAdded ( const QStringList)));
    list_view->connect(list_view,SIGNAL(OnExeRemoved ( const QStringList)),model,SLOT(OnExeRemoved ( const QStringList)));
    list_view->UpdateList();  

    view->setModel(model);
}
