/*!
 \file _SwEditorApplicativeCore.cpp
 \brief Coeur de traitement de l'editeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QList>
#include <QStringList>
#include <QDomDocument>
#include <QFile>
#include <QByteArray>
#include <QGraphicsView>
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"
#include "Main/Serialization/SwLoader_Class.h"
#include "Main/Serialization/SwSaver_Class.h"
#include <QApplication>
#include "Component/Services/ISwController.h"
#include "_SwEditorApplicativeCore.h"
#include "_SwEditorGraphicItem.h"
#include "_SwEditorGraphicScene.h"
#include "_SwEditorInterfacesGraphicItemManager.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwGui;
using namespace StreamWork::SwEditor;

/*! \brief Constructeur */
_SwEditorApplicativeCore::_SwEditorApplicativeCore(): SwComponent_Class(),_current_selection(this){
    _provider_service=NULL;
    _observers.clear();
    _streams.clear();
    _current_stream_index=-1;
    _current_navigator.DefinePermanentSelection(&_current_selection);
    _model=new _SwEditorStreamTreeModel(NULL,this,&_current_navigator,&_current_selection);
    _il_manager=new _SwEditorInterfacesLayerManager ;
    _pl_manager=new _SwEditorPinsLayerManager ;
}
/*! \brief Destructeur */
_SwEditorApplicativeCore::~_SwEditorApplicativeCore(){
    //Nettoyage de la liste des streams
    _streams.clear();
    //Suppression des observers existants
    _observers.clear();
    //Desenregistrement des services
    this->UnregisterService(_provider_service->GetServiceName());
    //destruction du manager d'affichage des interfaces
    delete _il_manager;
    //Destruction des services
    delete _provider_service;
    //Destruction du modele
    delete _model;
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwEditorApplicativeCore::InitializeResources() throw(SwException) {
    //Creation des service
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    //Enregistrement des services
    this->RegisterService(_provider_service);

    //Exportation de l'interface ISwEditorStreamsManager
    _provider_service->RegisterProvidedInterface<ISwEditorStreamsManager>("StreamsManager",(ISwEditorStreamsManager *)this);
    //Exportation de l'interface ISwEditorStreamNavigation
    _provider_service->RegisterProvidedInterface<ISwEditorStreamNavigation>("StreamNavigator",(ISwEditorStreamNavigation *)&_current_navigator);
    //Exportation de l'interface ISwEditorStreamOperations
    _provider_service->RegisterProvidedInterface<ISwEditorStreamOperations>("StreamOperations",(ISwEditorStreamOperations *)&_current_selection);
    //Exportation de l'interface QAbstractItemModel
    _provider_service->RegisterProvidedInterface<QAbstractItemModel>("Model",(QAbstractItemModel *)_model);
    //Exportation de l'interface ISwQAbstractItemViewSlots
    _provider_service->RegisterProvidedInterface<ISwQAbstractItemViewSlots>("ViewSlots",(ISwQAbstractItemViewSlots *)_model);
    //Exportation de l'interface ISwComponentProvider
    _provider_service->RegisterProvidedInterface<ISwComponentProvider>("SelectedComponent",(ISwComponentProvider *)this);

    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of SwEditorApplicativeCore done\n"));


}
//-------------------------------------------------------------
// methodes internes
//-------------------------------------------------------------
/*! \brief Ouverture interne d'un*/
void _SwEditorApplicativeCore::InternalOpenStream(QString file_name) {
    QFile file;
    QDomDocument doc;
    QString xml_error;
    int error_line,error_column;
    SwComponent_ClassPtr root_component;

    RegisterPostProcessing();
    _create_graphic_scene=true;
    try {
        QString stream_desc;
        file.setFileName(file_name);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to open file stream %1").arg(file_name));
            return;
        }
        stream_desc=QString(file.readAll());
        //parsing xml du document
        if (!doc.setContent(stream_desc,&xml_error,&error_line,&error_column)) {
            QString msg=QString("XML Parsing error:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
            QMessageBox::warning(NULL,"StreamWorkEditor alert",msg);
            return;
        }
        SwLoader_Class loader;
        root_component=loader.Load(doc);
        if (root_component==NULL) {
            QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Unable to load %1").arg(file_name));
            return;
        }
        root_component->SetDescription(file_name);
        //Enregistrement du composant
        SW_APP->AddNewStream(root_component);
    } catch(SwException & e) {
        UnregisterPostProcessing();
        QMessageBox::critical(NULL,"StreamWorkEditor critical",QString("Exception when load %1\n%2").arg(file_name).arg(e.what()));
        return;
    }
    UnregisterPostProcessing();
    //On redefinit le navigateur courant
    DefineComponentAsCurrentNavigator(root_component);
    //Enregistrement du nouveau stream
    _streams.push_back(root_component);
    //On met le nouveau stream comme strem courant
    _current_stream_index=_streams.size()-1;// dernier index puisque push_back
    //Enregistrement du undo
    //TO DO
}
/*! \brief Sauvegarde interne d'un stream*/
void _SwEditorApplicativeCore::InternalSaveStream(QString file_name) {
    SwSaver_Class saver;
	
	QFile file(file_name);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false)
	{
		QMessageBox::critical(NULL, "StreamWorkEditor critical", QString("Fail to save stream in file %1").arg(file_name));
		return;
	}

	QXmlStreamWriter writer(&file);
	writer.writeStartDocument();
	saver.Save(_streams[_current_stream_index], writer);
	writer.writeEndDocument();
	//Fermeture du fichier
	file.close();
}
/*! \brief Definition du navigateur courant*/
void _SwEditorApplicativeCore::DefineComponentAsCurrentNavigator(SwComponent_Class * root_component) {
    if (root_component==NULL) {
        _current_navigator.DefineNavigator(NULL);
        _current_navigator.Update();
        return;
   }
    if (root_component->QueryService(CG_SW_SERVICE_NAVIGATOR)==NULL) {
        root_component->RegisterService(new _SwEditorNavigator_Class(root_component));
    }
    _current_navigator.DefineNavigator(dynamic_cast<ISwEditorStreamNavigation *>(root_component->QueryService(CG_SW_SERVICE_NAVIGATOR)));
    _current_navigator.Update();
}
/*! \brief Enregistrement du postprocessing suite a creation de composant*/
void _SwEditorApplicativeCore::RegisterPostProcessing() {
    SW_APP->ComponentsBank().AddCreationPostProcessor(this);
}
/*! \brief Enregistrement du postprocessing suite a creation de composant*/
void _SwEditorApplicativeCore::UnregisterPostProcessing(){
    SW_APP->ComponentsBank().RemoveCreationPostProcessor(this);
}

//-------------------------------------------------------------
// Principales operations de gestion de stream
//-------------------------------------------------------------
/*! \brief Quitter l'application*/
void _SwEditorApplicativeCore::QuitEditor() {
    while (_current_stream_index!=-1) {
        CloseStream();
    }
    qApp->exit(0);
}
/*! \brief Crée un nouveau stream vide*/
void _SwEditorApplicativeCore::NewStream() {
    QString stream_name;
	QString root_component_type;
	QString root_plugin_name;
    QStringList list_of_components;
    bool result;
    SwComponent_Class * root_component;

    //Demande nom du stream a l'utilisateur
    stream_name=QInputDialog::getText(NULL,"StreamWorkEditor::Name of the new stream","Enter name of the new stream",QLineEdit::Normal,"Nonamed",&result);
    if (result!=true)
        return;
    //Demande du type de demarrage
    QList<SwPluginFactory_Class*> list_of_plugins=SW_APP->ComponentsBank().GetAllPlugins()->values();
	for (auto it = list_of_plugins.begin(); it != list_of_plugins.end(); it++)
	{
		for(QString component : (*it)->GetComponentsList())
			list_of_components.push_back((*it)->GetPluginName() + "::" + component);
	}
    list_of_components.push_front(QString("<empty component>"));
    root_component_type=QInputDialog::getItem(NULL,"StreamWorkEditor::Type of the root component","Select the type of the root component",list_of_components,0,false,&result);
    if (result!=true)
        return;
    if (root_component_type==QString("<empty component>")) {
		root_component_type = QString();
		root_plugin_name = QString();
    }
	else
	{
		QStringList nameSplit = root_component_type.split("::");
		root_component_type = nameSplit[0];
		root_plugin_name = nameSplit[1];
	}
    //Creation du nouveau stream
    RegisterPostProcessing();
    _create_graphic_scene=true;
    try {
        root_component=SW_APP->CreateNewStream(stream_name, root_plugin_name,root_component_type);
        root_component->SetDescription(stream_name+".xml");
    } catch (SwException & e) {
        UnregisterPostProcessing();
        QMessageBox::warning(NULL,"StreamWorkEditor alert",QString("Fail to create new stream because\n")+QString(e.what()));
        return;
    }
    UnregisterPostProcessing();
    //Enregistrement du nouveau stream
    _streams.push_back(root_component);
    //On met le nouveau stream comme strem courant
    _current_stream_index=_streams.size()-1;// dernier index puisque push_back
    //On signale le changement aux observers
    SignalChangeToObservers();
    //On redefinit le navigateur courant
    DefineComponentAsCurrentNavigator(root_component);
    //Enregistrement du undo
    //TO DO
}
/*! \brief Ouvrir un stream existant*/
void _SwEditorApplicativeCore::OpenStream(){
    QSettings settings;
    QString currentDir="";
    if(settings.contains("EditorDirectory")) {
        currentDir=settings.value("EditorDirectory").toString();
    }
    //Demande des fichier a lire
    QStringList files = QFileDialog::getOpenFileNames(
                         NULL,
                         "Select one or more stream files to open",
                         currentDir,
                         "Streams (*.xml)");
    QStringList list = files;
    QStringList::Iterator it = list.begin();
    if(it!=list.end()) {
        QFileInfo fi(*it);
        settings.setValue("EditorDirectory",QVariant(fi.filePath()));
    }
    while(it != list.end()) {
        InternalOpenStream(*it);
        ++it;
    }
    //On signale le changement aux observers
    SignalChangeToObservers();

}
/*! \brief Sauvegarde du stream courant */
void _SwEditorApplicativeCore::SaveStream(){
    InternalSaveStream(_streams[_current_stream_index]->GetDescription());
    //On signale le changement aux observers
    SignalChangeToObservers();
}
/*! \brief Sauvegarde du stream courant sous un nom donné*/
void _SwEditorApplicativeCore::SaveStreamAs(){

    if (_current_stream_index==-1)
        return;

    QSettings settings;
    QString currentDir="";
    if(settings.contains("EditorDirectory")) {
        currentDir=settings.value("EditorDirectory").toString();
    }
    QString fileName = QFileDialog::getSaveFileName(NULL,"Save stream",
                            currentDir,
                            "Streams (*.xml)");
    if (fileName.isEmpty())
        return;
    QFileInfo fi(fileName);
    settings.setValue("EditorDirectory",QVariant(fi.filePath()));
    _streams[_current_stream_index]->SetDescription(fileName);
    InternalSaveStream(fileName);
    //On signale le changement aux observers
    SignalChangeToObservers();
}
/*! \brief Fermeture du stream courant*/
void _SwEditorApplicativeCore::CloseStream(){
    SwComponent_Class * root_component;

    if (_current_stream_index==-1)
        return;

    //Fermeture et destruction du stream
    root_component=_streams[_current_stream_index];

    int ret = QMessageBox::question(NULL,QString("StreamWork Editor - ")+root_component->GetName(),
                   "Do you want to save stream before closing?",
                   QMessageBox::Yes | QMessageBox::No,
                   QMessageBox::Yes);
    //Sauvegarde du stream
    if (ret==QMessageBox::Yes) {
        SaveStream();
    }
    //Fermeture et destruction du stream
    root_component=_streams[_current_stream_index];
    //Retrait de la liste des streams geres
    _streams.erase(_streams.begin()+_current_stream_index);
    if (_current_stream_index>=_streams.size()) _current_stream_index--;
    //Mise a jour du navigateur courant
    if (_streams.size()==0) DefineComponentAsCurrentNavigator(NULL);
    else DefineComponentAsCurrentNavigator(_streams[_current_stream_index]);
    //On signale le changement aux observers
    SignalChangeToObservers();
    //Suppression du stream
    SW_APP->DestroyStream(root_component);
}
//-------------------------------------------------------------
// Principales operations de navigation au travers des streams
//-------------------------------------------------------------
/*! \brief Renvoie le nombre de streams ouverts*/
int _SwEditorApplicativeCore::GetStreamsCount(){
    return _streams.size();
}
/*! \brief Renvoie l'information d'un stream*/
QVariant _SwEditorApplicativeCore::GetStreamDescription(int index,int role){
    if (role==Qt::DisplayRole) {
        QString s;
        s=_streams[index]->GetDescription();
        if (!s.isEmpty()) {
            s=s.section('/', -1);
            s=_streams[index]->GetName()+"("+s+")";
        } else {
            s=_streams[index]->GetName();
        }
        return QVariant(s);
    }
    return QVariant();
}
/*! \brief Renvoie le stream courant*/
int _SwEditorApplicativeCore::GetCurrentStream(){
    return _current_stream_index;
}
/*! \brief Renvoie la racine du stream courant*/
SwComponent_Class * _SwEditorApplicativeCore::GetCurrentStreamRoot(){
    if (_current_stream_index<0 || _streams.size()==0)
        return NULL;
    return _streams[_current_stream_index];
}

/*! \brief Definition du stream courant*/
void _SwEditorApplicativeCore::SetCurrentStream(int index){
    _current_stream_index=index;
    if (index==-1)
		DefineComponentAsCurrentNavigator(NULL);
	else	
		DefineComponentAsCurrentNavigator(_streams[_current_stream_index]);
    //On signale le changement aux observers
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// Acces a la scene QT du stream courant
//-------------------------------------------------------------
/*! \brief Renvoie la scene graphique relative au stream courant courante*/
ISwEditorGraphicScene * _SwEditorApplicativeCore::GetCurrentStreamGraphicScene() {
    if (_current_stream_index<0 || _streams.size()==0)
        return NULL;
    return dynamic_cast<ISwEditorGraphicScene *>(_streams[_current_stream_index]->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_SCENE));
}
//-------------------------------------------------------------
// Observations
//-------------------------------------------------------------
/*! \brief Attachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::AttachStreamManagerObserver(ISwObserver * observer){

    if (_observers.find(observer)==_observers.end()) {
        _observers.insert(observer);
    }
}
/*! \brief Detachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::DetachStreamManagerObserver(ISwObserver * observer){
    QSet<ISwObserver *>::iterator it;

    it=_observers.find(observer);
    if (it!=_observers.end()) {
        _observers.erase(it);
    }
}
/*! \brief Signale un changement aux observers*/
void _SwEditorApplicativeCore::SignalChangeToObservers() {
    for (QSet<ISwObserver *>::iterator it=_observers.begin();it!=_observers.end();it++)
        (*it)->Update();
}
//-------------------------------------------------------------
// interface ISwCreationPostProcessor
//-------------------------------------------------------------
/*! \brief methode appelée après la creation d'un composant
\param[in] h_comp handle sur le composant qui vient d'etre créé*/
void _SwEditorApplicativeCore::AfterCreatePostProcess(SwComponent_Class *h_comp) {
    _SwEditorGraphicItem * item;
    //On n'affichage pas les controllers
    if (h_comp->QueryService(CG_SW_SERVICE_CONTROLLER)!=NULL) {
        return;
    }
    _il_manager->AddCompToSpy(h_comp);
    _pl_manager->AddCompToSpy(h_comp);
    if (h_comp->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_ITEM)==NULL) {
        item=new _SwEditorGraphicItem(h_comp);
        h_comp->RegisterService(item);
        item->Initialize(&_current_navigator,&_current_selection,_il_manager,_pl_manager);
    }
    if (_create_graphic_scene) {
        RegisterGraphicSceneService(h_comp);
        _create_graphic_scene=false;
    }
}
/*! \brief register graphic scene service*/
void _SwEditorApplicativeCore::RegisterGraphicSceneService(SwComponent_Class *h_comp) {
    _SwEditorGraphicScene * scene;
    if (h_comp->QueryService(CG_SW_SERVICE_EDITOR_GRAPHIC_SCENE)==NULL) {
        scene=new _SwEditorGraphicScene(h_comp);
        h_comp->RegisterService(scene);
    }
}
/*! \brief Forcer le reset du model*/
void _SwEditorApplicativeCore::ForceModelReset() {
    _model->ForceReset();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
// Selection permanente
//-------------------------------------------------------------
/*! \brief Constructeur */
_SwEditorApplicativeCore::_InternalSelection::_InternalSelection(_SwEditorApplicativeCore * p_core) {
    _selection=NULL;
    _observers.clear();
    _p_core=p_core;
}
/*! \brief Destructeur */
_SwEditorApplicativeCore::_InternalSelection::~_InternalSelection(){
    _observers.clear();
}
/*! \brief Definition du navigateur */
void _SwEditorApplicativeCore::_InternalSelection::DefineSelection(ISwEditorStreamOperations * selection){
    if (_selection!=NULL) {
        _selection->DetachSelectionObserver(this);
    }
    _selection=selection;
    if (_selection!=NULL) {
        _selection->AttachSelectionObserver(this);
    }
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// Ajout composant
//-------------------------------------------------------------
/*! \brief Ajoute un component enfant au composant courant*/
void _SwEditorApplicativeCore::_InternalSelection::AddChild() {
	AddChild(QString(), QString());
}
void _SwEditorApplicativeCore::_InternalSelection::AddChild(QString plugin_name, QString component_type){
    _p_core->RegisterPostProcessing();
    if (_selection!=NULL) _selection->AddChild(plugin_name, component_type);
    _p_core->UnregisterPostProcessing();
}
//-------------------------------------------------------------
// Operations de la selection
//-------------------------------------------------------------
/*! \brief Tout selectionner*/
void _SwEditorApplicativeCore::_InternalSelection::SelectAll(){
    if (_selection!=NULL) _selection->SelectAll();
}
/*! \brief Tout Deselectionner*/
void _SwEditorApplicativeCore::_InternalSelection::SelectNone(){
    if (_selection!=NULL) _selection->SelectNone();
}
/*! \brief Inverser la selection*/
void _SwEditorApplicativeCore::_InternalSelection::SelectInvert(){
    if (_selection!=NULL) _selection->SelectInvert();
}
/*! \brief Ajouter a la selection*/
void _SwEditorApplicativeCore::_InternalSelection::AddToSelection(QString component_name){
    if (_selection!=NULL) _selection->AddToSelection(component_name);
}
/*! \brief Retirer de la selection*/
void _SwEditorApplicativeCore::_InternalSelection::RemoveFromSelection(QString component_name){
    if (_selection!=NULL) _selection->RemoveFromSelection(component_name);
}
//-------------------------------------------------------------
// Consultation de la selection
//-------------------------------------------------------------
/*! \brief Acces au nombre d'elements selectionnée*/
int _SwEditorApplicativeCore::_InternalSelection::GetSelectedComponentNumber(){
    if (_selection!=NULL) return _selection->GetSelectedComponentNumber();
    return 0;
}
/*! \brief Acces a un element selectionné*/
SwComponent_Class * _SwEditorApplicativeCore::_InternalSelection::GetSelectedComponent(int index){
    if (_selection!=NULL) return _selection->GetSelectedComponent(index);
    return NULL;
}
/*! \brief Acces au nombre d'elements non selectionnée*/
int _SwEditorApplicativeCore::_InternalSelection::GetUnselectedComponentNumber(){
    if (_selection!=NULL) return _selection->GetUnselectedComponentNumber();
    return 0;
}
/*! \brief Acces a un element non selectionné*/
SwComponent_Class * _SwEditorApplicativeCore::_InternalSelection::GetUnselectedComponent(int index){
    if (_selection!=NULL) return _selection->GetUnselectedComponent(index);
    return NULL;
}
/*! \brief Permet de savoir si un composant est dans la selection*/
bool _SwEditorApplicativeCore::_InternalSelection::IsInSelection(SwComponent_Class * component) {
    if (_selection!=NULL) return _selection->IsInSelection(component);
    return false;
}
//-------------------------------------------------------------
// Operations sur la selection
//-------------------------------------------------------------
/*! \brief Supprimer la selection courante*/
void _SwEditorApplicativeCore::_InternalSelection::RemoveSelection(){
    _p_core->ForceModelReset();
    if (_selection!=NULL) return _selection->RemoveSelection();
}
/*! \brief Copie la selection courante*/
void _SwEditorApplicativeCore::_InternalSelection::CopySelection(){
    if (_selection!=NULL) return _selection->CopySelection();
}
/*! \brief Coller la selection courante*/
void _SwEditorApplicativeCore::_InternalSelection::PasteSelection(){
    _p_core->RegisterPostProcessing();
    if (_selection!=NULL) return _selection->PasteSelection();
    _p_core->UnregisterPostProcessing();
}
/*! \brief Couper la selection courante*/
void _SwEditorApplicativeCore::_InternalSelection::CutSelection(){
    _p_core->ForceModelReset();
    if (_selection!=NULL) return _selection->CutSelection();
}
//-------------------------------------------------------------
// Renommage
//-------------------------------------------------------------
/*! \brief Couper la selection courante*/
void _SwEditorApplicativeCore::_InternalSelection::RenameComponent(QString old_name,QString new_name) {
    if (_selection!=NULL) return _selection->RenameComponent(old_name,new_name);
}
//-------------------------------------------------------------
// Observations
//-------------------------------------------------------------
/*! \brief Attachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::_InternalSelection::AttachSelectionObserver(ISwObserver * observer){
    if (_observers.find(observer)==_observers.end()) {
        _observers.insert(observer);
    }
}
/*! \brief Detachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::_InternalSelection::DetachSelectionObserver(ISwObserver * observer){
    QSet<ISwObserver *>::iterator it;

    it=_observers.find(observer);
    if (it!=_observers.end()) {
        _observers.erase(it);
    }
}
/*! \brief Signale un changement aux observers*/
void _SwEditorApplicativeCore::_InternalSelection::SignalChangeToObservers() {
    for (QSet<ISwObserver *>::iterator it=_observers.begin();it!=_observers.end();it++)
    (*it)->Update();
    _p_core->OnProvidedComponentChange(_p_core);
}
//-------------------------------------------------------------
// ISwObserver
//-------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwEditorApplicativeCore::_InternalSelection::Update(StreamWork::SwCore::ISwObservable* sender){
    SignalChangeToObservers();
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
// Navigateur permanent
//-------------------------------------------------------------
//-------------------------------------------------------------
/*! \brief Constructeur */
_SwEditorApplicativeCore::_InternalNavigator::_InternalNavigator() {
    _navigator=NULL;
    _observers.clear();
    _h_selection=NULL;
}
/*! \brief Destructeur */
_SwEditorApplicativeCore::_InternalNavigator::~_InternalNavigator() {
    _observers.clear();
}
/*! \brief Definition de la selection permanente */
void _SwEditorApplicativeCore::_InternalNavigator::DefinePermanentSelection(_InternalSelection * selection) {
    _h_selection=selection;
}
/*! \brief Definition du navigateur */
void _SwEditorApplicativeCore::_InternalNavigator::DefineNavigator(ISwEditorStreamNavigation * navigator) {
    if (_navigator!=NULL) {
        _navigator->DetachNavigationObserver(this);
    }
    _navigator=navigator;
    if (_navigator!=NULL) {
        _navigator->AttachNavigationObserver(this);
    }
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// Navigation dans l'arbre du stream
//-------------------------------------------------------------
/*! \brief Remonter au parent*/
void _SwEditorApplicativeCore::_InternalNavigator::GoToParent() {
    if (_navigator!=NULL) _navigator->GoToParent();
}
/*! \brief Descendre sur un des enfants*/
void _SwEditorApplicativeCore::_InternalNavigator::GoToChild(QString child_name) {
    if (_navigator!=NULL) _navigator->GoToChild(child_name);
}
/*! \brief Acces au composant courant*/
SwComponent_Class * _SwEditorApplicativeCore::_InternalNavigator::GetCurrentComponent() {
    if (_navigator!=NULL) return _navigator->GetCurrentComponent();
    return NULL;
}
/*! \brief Definition au composant courant*/
void _SwEditorApplicativeCore::_InternalNavigator::SetCurrentComponent(SwComponent_Class * component) {
    if (_navigator!=NULL)
        _navigator->SetCurrentComponent(component);
}
/*! \brief Acces au path courant*/
QString _SwEditorApplicativeCore::_InternalNavigator::GetCurrentPath() {
    if (_navigator!=NULL) return _navigator->GetCurrentPath();
    return QString();
}
//-------------------------------------------------------------
// Observations
//-------------------------------------------------------------
/*! \brief Attachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::_InternalNavigator::AttachNavigationObserver(ISwObserver * observer) {
    if (_observers.find(observer)==_observers.end()) {
        _observers.insert(observer);
    }
}
/*! \brief Detachement d'un observateur sur cette interface*/
void _SwEditorApplicativeCore::_InternalNavigator::DetachNavigationObserver(ISwObserver * observer) {
    QSet<ISwObserver *>::iterator it;

    it=_observers.find(observer);
    if (it!=_observers.end()) {
        _observers.erase(it);
    }
}
/*! \brief Signale un changement aux observers*/
void _SwEditorApplicativeCore::_InternalNavigator::SignalChangeToObservers() {
    for (QSet<ISwObserver *>::iterator it=_observers.begin();it!=_observers.end();it++)
    (*it)->Update();
}
//-------------------------------------------------------------
// ISwObserver
//-------------------------------------------------------------
/*! \brief methode appelée par l'observable*/
void _SwEditorApplicativeCore::_InternalNavigator::Update(StreamWork::SwCore::ISwObservable* sender) {
    if (_navigator!=NULL) _h_selection->DefineSelection(dynamic_cast<ISwEditorStreamOperations *>(_navigator->GetCurrentComponent()->QueryService(CG_SW_SERVICE_SELECTION)));
    else  _h_selection->DefineSelection(NULL);
    SignalChangeToObservers();
}
//-------------------------------------------------------------
// interface ISwComponentProvider
//-------------------------------------------------------------
/*! \brief Fournit un composant */
SwComponent_Class * _SwEditorApplicativeCore::ProvideComponent() {
    if (_current_selection.GetSelectedComponentNumber()!=1)
        return NULL;
    return _current_selection.GetSelectedComponent(0);
}

