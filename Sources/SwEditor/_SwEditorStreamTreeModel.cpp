/*!
 \file _SwEditorStreamTreeModel.cpp
 \brief Implementation d'un modèle QT du stream en cours d'edition
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
/*
  * INCLUDES LOCAUX
  */
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include <QMessageBox>
#include "_SwEditorStreamTreeModel.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorStreamTreeModel::_SwEditorStreamTreeModel(QObject * parent,
												   ISwEditorStreamsManager * manager,
												   ISwEditorStreamNavigation * navigator,
												   ISwEditorStreamOperations * operations):
												   QAbstractItemModel(parent),
												   _manager(manager),
												   _navigator(navigator),
												   _operations(operations),
                                                   _ico_edit(":/SwEditor/comp_edit.png"){
    _manager->AttachStreamManagerObserver(this);
    _navigator->AttachNavigationObserver(this);
    _operations->AttachSelectionObserver(this);
    _current_host=_manager->GetCurrentStreamRoot();
    _current_selection=_navigator->GetCurrentComponent();
    _font_ed.setBold(true);
    _color_ed=QColor(0,74,128);
    _font_sel.setBold(true);
    _color_sel=QColor(0,173,239);
    //_font_notsel.setItalic(true);
    _color_notsel=QColor(58,110,165);
    //_font_other.setItalic(true);
    _color_other=QColor(128,128,128);
    //Aide
    _help="<h2>Stream navigation controls:</h2>\n";
    _help+="<h5>Component editing</h5>Alt+Click\n";
    _help+="<h5>Component selecting</h5>Click\n";
    _help+="<h5>Name editing</h5>DoubleClick on name\n";
    _help+="<h5>Description editing</h5>DoubleClick on description\n";

}
/*! \brief Destructeur*/
_SwEditorStreamTreeModel::~_SwEditorStreamTreeModel() {
    _operations->DetachSelectionObserver(this);
    _navigator->DetachNavigationObserver(this);
    _manager->DetachStreamManagerObserver(this);
}
/*! \brief forceReset */
void _SwEditorStreamTreeModel::ForceReset() {
	beginResetModel();
	endResetModel();
}
/*! \brief Renvoie les capacites du modèle */
Qt::ItemFlags _SwEditorStreamTreeModel::flags ( const QModelIndex & index ) const {
    SwComponent_Class * cparent;
    SwComponent_Class * cparentparent;

    cparent=(SwComponent_Class*)index.internalPointer();
    cparentparent=cparent->GetParent();
    if (index.column()==0 && (cparentparent==_current_selection || cparentparent==NULL))
        return Qt::ItemFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
    if (index.column()<=1 && (cparentparent==_current_selection))
        return Qt::ItemFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
    return Qt::ItemFlags(Qt::ItemIsEnabled);
}
/*! \brief Renvoie le nombre de colonne pour parent donné */
int _SwEditorStreamTreeModel::columnCount ( const QModelIndex & parent ) const{
    return 4;
}
/*! \brief Renvoie le nombre de ligne pour un parent donné */
int _SwEditorStreamTreeModel::rowCount ( const QModelIndex & parent ) const {
    SwComponent_Class * cparent;
    if (_current_host==NULL)
        return 0;
    if (!parent.isValid())
        return 1;
    cparent=(SwComponent_Class *)parent.internalPointer();
    return cparent->GetNbChildren();
}
/*! \brief Renvoie les données d'entete */
QVariant _SwEditorStreamTreeModel::headerData ( int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        switch(section) {
            case 0:
                return QVariant(QString("Component"));
            case 1:
                return QVariant(QString("Description"));
            case 2:
                return QVariant(QString("Factory"));
            case 3:
                return QVariant(QString("Factory Description"));
            default:
                break;
        }
    }
    return QVariant();
}
/*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
QVariant _SwEditorStreamTreeModel::data ( const QModelIndex & index, int role ) const{
    SwComponent_Class * cparent;
    SwComponent_Class * cparentparent;
     if (!index.isValid())
         return QVariant();
    cparent=(SwComponent_Class*)index.internalPointer();
    cparentparent=cparent->GetParent();
    if (role == Qt::DisplayRole && index.column()==0) {
        return QVariant(cparent->GetName());
    }
    if (role == Qt::DisplayRole && index.column()==1) {
        return QVariant(cparent->GetDescription());
    }
    if (role == Qt::DisplayRole && index.column()==2) {
        return QVariant(cparent->GetFactoryComponentName());
    }
    if (role == Qt::DisplayRole && index.column()==3) {
        return QVariant(SW_APP->ComponentsBank().GetComponentDescription(cparent->GetFactoryName(),cparent->GetFactoryComponentName()));
    }
    if (role == Qt::DecorationRole && index.column()==0) {
        return QVariant(SW_APP->ComponentsBank().GetComponentIcon(cparent->GetFactoryName(),cparent->GetFactoryComponentName()));
    }
    if (cparent==_current_selection ) {
        if (role == Qt::ForegroundRole) {
            return QVariant(_color_ed);
        }
        if (role == Qt::FontRole && index.column()==0) {
            return QVariant(_font_ed);
        }
    } else {
        if (cparentparent==_current_selection) {
            if (_operations->IsInSelection(cparent)) {
                if (role == Qt::ForegroundRole) {
                    return QVariant(_color_sel);
                }
                if (role == Qt::FontRole && index.column()==0) {
                    return QVariant(_font_sel);
                }
            } else {
                if (role == Qt::ForegroundRole) {
                    return QVariant(_color_notsel);
                }
                if (role == Qt::FontRole && index.column()==0) {
                    return QVariant(_font_notsel);
                }
            }
        } else {
            if (role == Qt::ForegroundRole) {
                return QVariant(_color_other);
            }
            if (role == Qt::FontRole && index.column()==0) {
                return QVariant(_color_other);
            }
        }
    }
    return QVariant();
}
/*! \brief Permet de definir la valeur d'un item*/
bool _SwEditorStreamTreeModel::setData ( const QModelIndex & index, const QVariant & value, int role ) {
    QVariant ivalue , tmpvalue;
    SwComponent_Class * cparent;
    SwComponent_Class * cparentparent;

    if (!index.isValid() || role!=Qt::EditRole)
        return false;
    cparent=(SwComponent_Class*)index.internalPointer();
    if (index.column()==1 && !value.toString().isEmpty()) {
        cparent->SetDescription(value.toString());
        return true;
    }
    cparentparent=cparent->GetParent();
    if (index.column()==0 && !value.toString().isEmpty() && (cparentparent==_current_selection || cparentparent==NULL)) {
        if (cparentparent!=NULL) _operations->RenameComponent(cparent->GetName(),value.toString());
        else cparent->SetName(value.toString());
        return true;
    }
    return false;
}
/*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
QModelIndex _SwEditorStreamTreeModel::index ( int row, int column, const QModelIndex & parent  ) const{
    SwComponent_Class * cparent;
    if (_current_host==NULL)
        return QModelIndex();
    if (!parent.isValid())
        return createIndex(row,column,(void *)_current_host);

    cparent=(SwComponent_Class *)parent.internalPointer();
    return createIndex(row,column,(void *)cparent->GetChild(row));
}
/*! \brief Renvoie l'item parent d'un item index donné */
QModelIndex _SwEditorStreamTreeModel::parent ( const QModelIndex & index ) const{
    SwComponent_Class * cindex;
    SwComponent_Class * parent;
    int index_parent;
    if (!index.isValid())
        return QModelIndex();
    cindex=(SwComponent_Class *)index.internalPointer();
    parent=cindex->GetParent();
    if (parent==NULL)
        return QModelIndex();
    //Calcul de l'indice du parent
    cindex=parent;
    parent=cindex->GetParent();
    //Si le parent du parent est null, c'est le composant racine
    if (parent==NULL) {
        return createIndex(0,0,(void *)cindex);
    } else {
        //Sinon il faut calculer l'index
        index_parent=parent->GetChildIndex(cindex);
        if (index_parent==-1)
            return QModelIndex();
    }
    return createIndex(index_parent,0,(void *)cindex);
}
//-------------------------------------------------------------
// Interface observeur
//------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwEditorStreamTreeModel::Update(StreamWork::SwCore::ISwObservable* sender) {
    if (_current_host!=_manager->GetCurrentStreamRoot()) {
		beginResetModel();
        _current_host=_manager->GetCurrentStreamRoot();
		endResetModel();
        return;
    }
    if (_current_selection!=_navigator->GetCurrentComponent()) {
        _current_selection=_navigator->GetCurrentComponent();
        this->layoutChanged();//A Revoir
        return;
    }
    //Cas changement suite a operation
    this->layoutChanged();//A Revoir
}
//-------------------------------------------------------------
// Interface ISwQAbstractItemViewSlots
//------------------------------------------------------------
/*! \brief sur activation */
void _SwEditorStreamTreeModel::onActivated ( const QModelIndex & index ) {

}
/*! \brief sur click */
void _SwEditorStreamTreeModel::onClicked ( const QModelIndex & index ) {
    SwComponent_Class * cparent;
    SwComponent_Class * cparentparent;
    if (!index.isValid())
        return ;

    if (!_m_buttons.testFlag(Qt::LeftButton)) {
        return;
    }

    cparent=(SwComponent_Class*)index.internalPointer();

    //Selection module a editer
    if (_m_modifiers.testFlag(Qt::AltModifier)) {
        if (cparent!=_current_selection)
            this->layoutAboutToBeChanged();
        _navigator->SetCurrentComponent(cparent);
        return;
    }

    //Selection module a selectionner
    cparentparent=cparent->GetParent();

    if (cparentparent==_current_selection) {
        if (_m_modifiers.testFlag(Qt::ControlModifier)) {
            if (_operations->IsInSelection(cparent)) {
                _operations->RemoveFromSelection(cparent->GetName());
            } else {
                _operations->AddToSelection(cparent->GetName());
            }
        } else {
            _operations->SelectNone();
            _operations->AddToSelection(cparent->GetName());
        }
    }
}
/*! \brief sur double click */
void _SwEditorStreamTreeModel::onDoubleClicked ( const QModelIndex & index ){
}
/*! \brief sur entree */
void _SwEditorStreamTreeModel::onEntered ( const QModelIndex & index ){

}
/*! \brief sur  appuie de la sourie sur l'item */
void _SwEditorStreamTreeModel::onPressed ( const QModelIndex & index ){

}
/*! \brief sur entre dans la vue*/
void _SwEditorStreamTreeModel::onViewportEntered (){

}
/*! \brief sur evenement sourie quelconque*/
void _SwEditorStreamTreeModel::onMouseEvent(QMouseEvent * mouse_event) {
    Qt::MouseButtons m_buttons;
    m_buttons=mouse_event->buttons();
    if (0!=(int)m_buttons) {
        _m_buttons=m_buttons;
    }
    _m_modifiers=mouse_event->modifiers();
}
/*! \brief sur demande de l'aide*/
void _SwEditorStreamTreeModel::onHelp() {
    QMessageBox::information(NULL,"Stream navigation controls",_help);
    return;
}
/*! \brief OnMenuRequest*/
void _SwEditorStreamTreeModel::onMenuRequested( const QPoint & globalpos,QModelIndex index) {
    //Rien a faire pour le moment
}
