/**
 * @file _ReplayManager.cpp
 * @brief _ReplayManager
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */

#include <SwApplication.h>
#include <SwMacros.h>
#include "_ReplayManager.h"
#include <SwAddress_ToolBox.h>
#include "SwRecordConstantes.h"
#include "ISwSupportReplay.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;

/** @brief Constructeur */
_ReplayManager::_ReplayManager(_SwServiceRecording * serviceRecord):SwComponent_Class() {
    _serviceRecord=serviceRecord;
    _properties_service=NULL;
    _provider_service=NULL;
	_consumer_service=NULL;

	//RESET Interfaces

	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE
    _repository=SwFileDescriptor(SwFileDescriptor::DirectorySelect,QString(),QString());
    _enableReplay=false;
    _executorPath="";
    _executor=0;
    _widget=new _ReplayWidget();
    _timeLine.addReplayManagerListener(_widget);
}
/** @brief Destructeur */
_ReplayManager::~_ReplayManager() {

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins

    //Desenregistrement des services
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    _timeLine.removeReplayManagerListener(_widget);
    delete _widget;
}
/** 
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void _ReplayManager::InitializeResources() throw(SwException) {
    QVariant tmp;

    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);

    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    _provider_service->RegisterProvidedInterface<ISwReplayManager>("ReplayManager",(ISwReplayManager *)this);
    _provider_service->RegisterProvidedInterface<ISwWidget>("Widget",(ISwWidget *)this);
    //--------------------------------------
    //Definition Interfaces consommés
    //--------------------------------------

    //S'enregistrer comme observer du consumer
    _consumer_service->AttachInterfacesConsumerObserver(this);

    //--------------------------------------
    //Definition Pins
    //--------------------------------------

    //--------------------------------------
    //Definition Properties
    //--------------------------------------
    _properties_service->CreatePropertiesForQObject(this,QString(),true);
    _properties_service->CreatePropertiesForQObject(_widget,QString("widget"),true);
    //--------------------------------------
    //Autres
    //--------------------------------------
}

/** @brief Callback sur les changements de propriétés*/
void _ReplayManager::OnPropertyChange(ISwProperty * property) {
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void _ReplayManager::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief Apres changement de la disponibilité de l'interface */
void _ReplayManager::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
//---------------------------------------------------------------------
// Interface ISwReplayManager
//---------------------------------------------------------------------
/*@brief definition du repertoire d'enregistrement */
void _ReplayManager::setRecordDirectory(QString directoryName) {
    _repository.setFileName(directoryName);
}
/*@brief acces au fichier de configuration */
QString _ReplayManager::getConfigurationFile(){
    return _repository.getFileName()+QDir::separator()+CG_RECORD_CONFIGURATION_FILE;
}
/*@brief demarrage du rejeu avec l'executeur de pilotage*/
void _ReplayManager::startReplay(ISwExecution_Service * exeService){
    //Chargement du fichier d'enregistrement
    QDir d=QDir(_repository.getFileName());
    if (!d.exists())
        return;
    //Creation du fichier d'enregistrement
    QFile *fileReader=new QFile(d.absolutePath()+QDir::separator()+CG_RECORD_FILE);
    if (!fileReader->open(QIODevice::ReadOnly | QIODevice::Text)) {
        delete fileReader;
        fileReader=0;
        _enableReplay=false;
        return;
    }
    QXmlStreamReader * reader=new QXmlStreamReader(fileReader);
    _timeLine.reset();
    _timeLine.loadRecordFile(reader,_serviceRecord,_repository.getFileName());
    delete reader;
    fileReader->close();
    delete fileReader;
    fileReader=0;
   //Definition de la clock a l'enregistrement
    _executor=exeService;
    _executor->SetClockProvider(&_timeLine);
    ISwSupportReplay * replaySupport=dynamic_cast<ISwSupportReplay *>(_executor->GetHostComponent());
    if (replaySupport!=0) {
        replaySupport->setReplayMode(true);
    }
    //Execution
    _executor->StartExecution();
}
/*@brief arret du rejeu */
void _ReplayManager::stopReplay(){
    if (_executor!=0) {
        if (!_executor->IsExecutionStopped()) {
            _executor->AskForStopExecution();
            _executor->StopExecution();
            _timeLine.queryStopTime();
        }
         _executor->SetClockProvider(0);
        ISwSupportReplay * replaySupport=dynamic_cast<ISwSupportReplay *>(_executor->GetHostComponent());
        if (replaySupport!=0) {
            replaySupport->setReplayMode(false);
        }
    }
}
/*@brief ajout de listener*/
void _ReplayManager::addReplayManagerListener(ISwReplayManagerListener * listener) {
    _timeLine.addReplayManagerListener(listener);
}
/*@brief suppression de listener*/
void _ReplayManager::removeReplayManagerListener(ISwReplayManagerListener * listener){
    _timeLine.removeReplayManagerListener(listener);
}

//---------------------------------------------------------------------
// Interface ISwWidget
//---------------------------------------------------------------------
/*! \brief Renvoie le widget
\return le widget */
QWidget & _ReplayManager::GetWidget() {
    return *_widget;
}
//--------------------------------------------------------------
//Properties getter and setter
//--------------------------------------------------------------
/** @brief recordDirectory */
SwFileDescriptor _ReplayManager::getRecordDirectory() const {
    return _repository;
}
void _ReplayManager::setRecordDirectory(const SwFileDescriptor & val){
    _repository=val;
}
/** @brief record */
bool _ReplayManager::getEnableReplay() const{
    return _enableReplay;
}
void _ReplayManager::setEnableReplay(bool val){
    ISwExecution_Service * executor=0;
    if (_enableReplay==val)
        return;
    if (val) {
        //Recuperation de l'executor
        SwComponent_Class * component=SwAddress_ToolBox::FindTarget(_executorPath,this);
        if (component!=0) {
            executor=dynamic_cast<ISwExecution_Service *>(component->QueryService(CG_SW_SERVICE_EXECUTION));
        }
        if (executor==0)
            return;
    }
    _enableReplay=val;
    if (_enableReplay) {
        startReplay(executor);
    } else {
        stopReplay();
    }
}
/** @brief executor */
QString _ReplayManager::getExecutor() const {
    return _executorPath;
}
void _ReplayManager::setExecutor(QString val) {
    _executorPath=val;
}
