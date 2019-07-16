/*!
\file _SwEditorSelection_Class.cpp
\date 12/04/2006
\brief implementation de la selection
\author  Big
\version 1.0
 */
#include <QClipboard>
#include <SwApplication.h>
#include <QApplication>
#include <SwMacros.h>
#include <QMessageBox>
#include "_SwEditorSelection_Class.h"
#include <SwSaver_Class.h>
#include <SwLoader_Class.h>
#include <QMimeData>
#include <QBuffer>
#include "ISwEditorGraphicItem.h"


using namespace StreamWork::SwCore;
using namespace StreamWork::SwEditor;

/*! \brief Construsteur */
_SwEditorSelection_Class::_SwEditorSelection_Class(SwComponent_Class * host) {
    SwComponent_Class * component;
    _host_component=host;
    _observers.clear();

    //Enregistrement de tous les composants dans non selected
    component=_host_component->GetFirstChild();
    while (component!=NULL) {
        if (dynamic_cast<ISwEditorGraphicItem *>(component->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM))!=NULL)
            _unselecteds.push_back(component);
        component=_host_component->GetNextChild();
    }
    //Aucun element selectionne
    _selecteds.clear();
}
/*! \brief Destrusteur */
_SwEditorSelection_Class::~_SwEditorSelection_Class(){
    _observers.clear();
}
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _SwEditorSelection_Class::Liberate() {

}
//-------------------------------------------------------------
// Ajout composant
//-------------------------------------------------------------
/*! \brief Ajoute un component enfant au composant courant*/
void _SwEditorSelection_Class::AddChild(QString component_type) {
    SwComponent_ClassPtr component;
    QString new_name;
    //Creation du composant
    component=SW_APP->ComponentsBank().CreateComponent(component_type);
    //Renommage du composant si necessaire
    new_name=_host_component->GetSuggestedNameForChild(component->GetName());
    component->SetName(new_name);
    //On supprime la selection courante
    _unselecteds+=_selecteds;
    _selecteds.clear();
    //Insertion du composant
    _host_component->AddChild(component);
    //Fin
    //Selection du composant
    AddToSelection(new_name);
}
//-------------------------------------------------------------
// Operations de la selection
//-------------------------------------------------------------
/*! \brief Tout selectionner*/
void _SwEditorSelection_Class::SelectAll(){
    _selecteds+=_unselecteds;
    _unselecteds.clear();
    SignalChangeToObservers();
}
/*! \brief Tout Deselectionner*/
void _SwEditorSelection_Class::SelectNone(){
    _unselecteds+=_selecteds;
    _selecteds.clear();
    SignalChangeToObservers();
}
/*! \brief Inverser la selection*/
void _SwEditorSelection_Class::SelectInvert(){
    QList<SwCore::SwComponent_Class *> tmp_list;
    tmp_list=_unselecteds;
    _unselecteds=_selecteds;
    _selecteds=tmp_list;
    SignalChangeToObservers();
}
/*! \brief Ajouter a la selection*/
void _SwEditorSelection_Class::AddToSelection(QString component_name){
    SwComponent_Class * component;
    int index;

    component=_host_component->GetChild(component_name);
    if (component!=NULL && dynamic_cast<ISwEditorGraphicItem *>(component->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM))!=NULL) {
        index=_unselecteds.indexOf(component);
        if (index!=-1) {
            _unselecteds.removeAt(index);
        }
        _selecteds.push_back(component);
    }
    SignalChangeToObservers();
}
/*! \brief Retirer de la selection*/
void _SwEditorSelection_Class::RemoveFromSelection(QString component_name){
    SwComponent_Class * component;
    int index;

    component=_host_component->GetChild(component_name);
    if (component!=NULL && dynamic_cast<ISwEditorGraphicItem *>(component->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM))!=NULL) {
        index=_selecteds.indexOf(component);
        if (index!=-1) {
            _selecteds.removeAt(index);
            _unselecteds.push_back(component);
        }
    }
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// Consultation de la selection
//-------------------------------------------------------------
/*! \brief Acces au nombre d'elements selectionnée*/
int _SwEditorSelection_Class::GetSelectedComponentNumber(){
    return _selecteds.size();
}
/*! \brief Acces a un element selectionné*/
SwComponent_Class * _SwEditorSelection_Class::GetSelectedComponent(int index){
    if (index>=_selecteds.size()) return NULL;
    return _selecteds[index];
}
/*! \brief Acces au nombre d'elements non selectionnée*/
int _SwEditorSelection_Class::GetUnselectedComponentNumber(){
    return _unselecteds.size();
}
/*! \brief Acces a un element non selectionné*/
SwComponent_Class * _SwEditorSelection_Class::GetUnselectedComponent(int index){
    if (index>=_unselecteds.size()) return NULL;
    return _unselecteds[index];
}
/*! \brief Permet de savoir si un composant est dans la selection*/
bool _SwEditorSelection_Class::IsInSelection(SwComponent_Class * component) {
    if (_selecteds.indexOf(component)!=-1) {
        return true;
    }
    return false;
}
//-------------------------------------------------------------
// Operations sur la selection
//-------------------------------------------------------------
/*! \brief Supprimer la selection courante*/
void _SwEditorSelection_Class::RemoveSelection(){

    for(int i=0;i<_selecteds.count();i++) {
        _host_component->RemoveChild(_selecteds[i]);
    }
    _selecteds.clear();
    //On signale les changements
    SignalChangeToObservers();
}
/*! \brief Copie la selection courante*/
void _SwEditorSelection_Class::CopySelection(){
   //Recuperation & Construction du stream
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly | QIODevice::Text);
	QXmlStreamWriter writer(&buffer);
	writer.setAutoFormattingIndent(4);
	writer.setAutoFormatting(true);

	SwSaver_Class saver;
	saver.SaveGroup(_selecteds, writer);
	//Enregistrement dans le clipboard
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(buffer.data());
}
/*! \brief Coller la selection courante*/
void _SwEditorSelection_Class::PasteSelection(){
    SwLoader_Class loader;
    QDomDocument doc;
    QString xml_error;
    int error_line,error_column;
    SwComponent_Class * component;

    QClipboard *clipboard = QApplication::clipboard();

    QString stream_desc;
    //Recuperation du stream
    if (!clipboard->mimeData()->hasText())
        return;

    stream_desc=clipboard->text();
    //parsing xml du document
    if (!doc.setContent(stream_desc,&xml_error,&error_line,&error_column)) {
        QString msg=QString("XML Parsing:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
        QMessageBox::warning(NULL, "StreamWork Editor",msg);
        return;
    }
    //On deselectionne tout
    SelectNone();
    //Chargement du stream
    loader.LoadGroup(_host_component,doc);
    //On selectionne tout composant ne faisant pas partie des non selectionnés
    //Enregistrement de tous les composants dans non selected
    component=_host_component->GetFirstChild();
    while (component!=NULL) {
        if (dynamic_cast<ISwEditorGraphicItem *>(component->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM))!=NULL) {
            if (_unselecteds.indexOf(component)==-1) {
                _selecteds.push_back(component);
            }
        }
        component=_host_component->GetNextChild();
    }
    //On signale les changements
    SignalChangeToObservers();
}
/*! \brief Couper la selection courante*/
void _SwEditorSelection_Class::CutSelection(){
    CopySelection();
    RemoveSelection();
}
//-------------------------------------------------------------
// Renommage
//-------------------------------------------------------------
/*! \brief Couper la selection courante*/
void _SwEditorSelection_Class::RenameComponent(QString old_name,QString new_name) {
    try {
        SwComponent_Class * component;
        int index;
        component=_host_component->GetChild(old_name);
        if (dynamic_cast<ISwEditorGraphicItem *>(component->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM))==NULL)
            return;
        component->SetName(new_name);
        //Mise a jour de la selection
        index=_selecteds.indexOf(component);
        if (index!=-1) {
            _selecteds.removeAt(index);
            _selecteds.push_back(component);
        }
        index=_unselecteds.indexOf(component);
        if (index!=-1) {
            _unselecteds.removeAt(index);
            _unselecteds.push_back(component);
        }
    } catch(SwException & se) {
        //Mauvais nom
        QMessageBox::warning(0,QString("Invalid component name..."),QString(se.GetReason()));
        return;
    }
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// Observations
//-------------------------------------------------------------
/*! \brief Attachement d'un observateur sur cette interface*/
void _SwEditorSelection_Class::AttachSelectionObserver(ISwObserver * observer){

    if (_observers.find(observer)==_observers.end()) {
        _observers.insert(observer);
    }
}
/*! \brief Detachement d'un observateur sur cette interface*/
void _SwEditorSelection_Class::DetachSelectionObserver(ISwObserver * observer){
    QSet<ISwObserver *>::iterator it;

    it=_observers.find(observer);
    if (it!=_observers.end()) {
        _observers.erase(it);
    }
}
/*! \brief Signale un changement aux observers*/
void _SwEditorSelection_Class::SignalChangeToObservers() {
    for (QSet<ISwObserver *>::iterator it=_observers.begin();it!=_observers.end();it++)
        (*it)->Update();
}
