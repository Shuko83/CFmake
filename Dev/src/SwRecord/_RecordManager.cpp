/**
 * @file _RecordManager.cpp
 * @brief Record Manager
 * @version 1.0
 * @date Tue Apr 28 15:55:16 CEST 2009
 * @author F.Bighelli
 */


#include <SwApplication.h>
#include <SwMacros.h>
#include "_RecordManager.h"
#include "SwRecordConstantes.h"
using namespace StreamWork::SwCore;

/** @brief Constructeur */
_RecordManager::_RecordManager(_SwServiceRecording * serviceRecord):SwComponent_Class() {
    _serviceRecord=serviceRecord;
    _properties_service=NULL;
    _provider_service=NULL;
	_consumer_service=NULL;
    _writer=0;
    _writerData =0;
    _fileWriter=0;
    _itime=0.0;
    _configSaver = 0;
	//RESET Interfaces
    
	//RESET Pins

	//RESET Properties
	//INITIALIZER LES AUTRES ATTRIBUTS DE LA CLASSE
    _repository=SwFileDescriptor(SwFileDescriptor::DirectorySelect,QString(),QString());
    _configuration=SwFileDescriptor(SwFileDescriptor::FileRead,QString(),QString());
    _enableRecording=false;
    _maxRecordSize=100000; //100K
    _enabledInternalWidget=true;
    _widget=new _RecordWidget();
    addRecordManagerListener(_widget);
}
/** @brief Destructeur */
_RecordManager::~_RecordManager() {

	//Destruction properties automatiques
	//Desenregistrement automatiques des interfaces
	//Destruction Pins

    //Desenregistrement des services
    this->UnregisterService(GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;

	//DETRUIRE LES AUTRES ATTRIBUTS DE LA CLASSE
    removeRecordManagerListener(_widget);
    delete _widget;
}
/** 
 * @brief Initialisation des ressources
 * @note tous les services du composants doivent être déclarés dans cette methodes
 */
void _RecordManager::InitializeResources() throw(SwException) {
    QVariant tmp;

    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);

    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    this->RegisterService(this);

    //--------------------------------------
    //Definition Interfaces fournis
    //--------------------------------------
    _provider_service->RegisterProvidedInterface<ISwRecordManager>("RecordManager",(ISwRecordManager *)this);
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
void _RecordManager::OnPropertyChange(ISwProperty * property) {
}
//---------------------------------------------------------------------
// Interface ISwInterfaces_ConsumerObserver
//---------------------------------------------------------------------
/** @brief Avant changement de la disponibilité de l'interface */
void _RecordManager::BeforeInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
/** @brief Apres changement de la disponibilité de l'interface */
void _RecordManager::AfterInterfaceAvailabilityChange(QString interface_name,SwComponent_Class * provider_host) {
}
//---------------------------------------------------------------------
// Interface ISwRecordManager
//---------------------------------------------------------------------
/*@brief definition du repertoire d'enregistrement */
void _RecordManager::setRecordDirectory(QString directoryName) {
    _repository.setFileName(directoryName);
}
/*@brief definition du fichier de configuration */
void _RecordManager::setConfigurationFile(QString fileName){
    _configuration.setFileName(fileName);
}
/*@brief definition de l outil de sauvegarde du fichier de configuration */
void _RecordManager::setConfigurationSaver(StreamWork::SwConfiguration::ISwConfigurationSaver * configSaver)
{
    _configSaver = configSaver;
}
    
/*@brief demarrage de l'enregistrement */
void _RecordManager::startRecording(){
    _totalSize=0;
    QDir d=QDir(_repository.getFileName());
    if (!d.exists())
        return;
    //Sauvegarde la configuration
    saveConfiguration();
    //Creation du fichier d'enregistrement
    _fileWriter=new QFile(d.absolutePath()+QDir::separator()+CG_RECORD_FILE);
    if (!_fileWriter->open(QIODevice::WriteOnly | QIODevice::Text)) {
        delete _fileWriter;
        _fileWriter=0;
        return;
    }
    _dataCounter=0;
    //Creation du writer
    _writer=new QXmlStreamWriter(_fileWriter);
    _writer->setAutoFormatting(true);
    _writer->writeStartDocument("1.0");
    _writer->writeStartElement(CG_RECORD);
    //Creation du writer data
    createWriterData();
}
/*@brief arret de l'enregistrement */
void _RecordManager::stopRecording(){
    if (_writerData!=0) {
        closeWriterData();
    }
    if (_writer!=0) {
        //Desenregistrement du record manager au pres des record points 
        QList<ISwRecordPoint *> points=_serviceRecord->getRecordPoints();
        foreach(ISwRecordPoint * rpoint,points) {
            rpoint->setRecordManager(0);
        }
        _writer->writeEndElement(); //Body
        _writer->writeEndElement(); //Record
        //Fin du doc
        _writer->writeEndDocument();
        //Destruction du writer
        delete _writer;
        _writer=0;
        _fileWriter->close();
        delete _fileWriter;
        _fileWriter=0;
    }
}
/*@brief demande de creation de clef d'enregistrement*/
QXmlStreamWriter *_RecordManager::queryRecordKey(ISwRecordPoint * recordPoint,double currentTime) {
    QMap<ISwRecordPoint *,int>::iterator it=_recordPointMapping.find(recordPoint);
    if (it!=_recordPointMapping.end()) {
        _writerData->writeStartElement(CG_RECORD_KEY);
        //_writer->writeAttribute("t",QString("%1").arg(currentTime-_itime,0,'f',3));
        _writerData->writeAttribute(CG_RECORD_RP_ATT_NUMBER,QString("%1").arg(it.value()));
        return _writerData;
    }
    return 0;
}
/*@brief finalisation de la clef d'enregistrement*/
void _RecordManager::finalizeRecordKey() {
    _writerData->writeEndElement();
}
/** @brief  creation d'un writer pour les données*/
void _RecordManager::createWriterData() {
    QDir d=QDir(_repository.getFileName());
    QString name=QString(d.absolutePath()+QDir::separator()+CG_RECORD_FILE_DATA).arg(_dataCounter++);
    _fileWriterData=new QFile(name);
    if (!_fileWriterData->open(QIODevice::WriteOnly | QIODevice::Text)) {
        delete _fileWriterData;
        _fileWriterData=0;
        return;
    }
    //Creation du writer
    _writerData=new QXmlStreamWriter(_fileWriterData);
    _writerData->setAutoFormatting(true);
    _writerData->writeStartDocument("1.0");
    _writerData->writeStartElement(CG_RECORD_DATA);

}
/** @brief  fermetude du writer pour les données */
void _RecordManager::closeWriterData() {
    _writerData->writeEndElement(); //Record Data
    //Fin du doc
    _writerData->writeEndDocument();
    _totalSize+=_fileWriterData->size();
    //Destruction du writer
    delete _writerData;
    _writerData=0;
    _fileWriterData->close();
    delete _fileWriterData;
    _fileWriterData=0;
}
/** @brief  sauvegarde de la configuration */
void _RecordManager::saveConfiguration() {
    if (_configuration.getFileName().isEmpty()) 
    {
        SW_LOG.Log(LogLvl_Emergency,"Configuration file does not exist");
        return;
    }
    //creation du repertoire pour stocker les configurations
    QDir d=QDir(_repository.getFileName()+QDir::separator()+CG_RECORD_CONFIGURATION_DIRECTORY);
    if (!d.exists()) {
        d=QDir(_repository.getFileName());
        d.mkdir(CG_RECORD_CONFIGURATION_DIRECTORY);
        d.setPath(_repository.getFileName()+QDir::separator()+CG_RECORD_CONFIGURATION_DIRECTORY);
    }
    // sauvegarde du fichier de configuration
    if (_configSaver != 0)
    {
        _configSaver->SaveConfiguration(_repository.getFileName()+QDir::separator()+CG_RECORD_CONFIGURATION_FILE);
    }
    else
    {
        SW_LOG.Log(LogLvl_Emergency,"Configuration saving Interface not set");
    }


}
/*@brief ajout de listener*/
void _RecordManager::addRecordManagerListener(ISwRecordManagerListener * listener) {    
    if (listener!=0)
        _listeners.push_back(listener);
}
/*@brief suppression de listener*/
void _RecordManager::removeRecordManagerListener(ISwRecordManagerListener * listener){
    if (listener!=0)
        _listeners.removeOne(listener);
}

//---------------------------------------------------------------------
// Interface ISwExecutable_Service
//---------------------------------------------------------------------
/*! \brief Initialisation */
void _RecordManager::Initialize(double start_time,ISwExecution_Service * executor) throw (SwException) {
    _itime=start_time;
    if(_writer!=0) {
        foreach(ISwRecordManagerListener * listener,_listeners) {
            listener->setEnableRecordInformation(true);
            listener->setStartTime(start_time);
        }
        _writer->writeStartElement(CG_RECORD_HEADER);
        _writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(_itime,0,'f',3));
        //Construction et enregistrement du mapping des points d'enregistrement
        buildRecordPointMapping();
        //Fin header
        _writer->writeEndElement();
        _writer->writeStartElement(CG_RECORD_BODY);
        //Enregistrement du record manager au pres des record points 
        QList<ISwRecordPoint *> points=_serviceRecord->getRecordPoints();
        foreach(ISwRecordPoint * rpoint,points) {
            rpoint->setRecordManager(this);
        }
    }
}
/*! \brief Demarrage */
void _RecordManager::Start(double current_time) throw (SwException){
    if(_writer!=0) {
        _writer->writeStartElement(CG_RECORD_START);
        _writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(current_time-_itime,0,'f',3));
        _writer->writeEndElement();
    }
}            
/*! \brief Execution */
void _RecordManager::Execute(double current_time,bool is_first_call) throw (SwException){
    if(_writer!=0) {
        int size=_totalSize+(unsigned int)_fileWriterData->size();
        foreach(ISwRecordManagerListener * listener,_listeners) {
            listener->setCurrentTime(current_time);
            listener->setDataSize(size);
        }
        if (_maxRecordSize<(int)_fileWriterData->size()) {
            closeWriterData();
            createWriterData();
        }
        _writerData->writeStartElement(CG_RECORD_EXE);
        _writerData->writeAttribute(CG_RECORD_TIME,QString("%1").arg(current_time-_itime,0,'f',3));
        _writerData->writeEndElement();
    }
}            
/*! \brief Execution */
void _RecordManager::Stop(double current_time){
    if(_writer!=0) {
        _writer->writeStartElement(CG_RECORD_STOP);
        _writer->writeAttribute(CG_RECORD_TIME,QString("%1").arg(current_time-_itime,0,'f',3));
        _writer->writeAttribute(CG_RECORD_STOP_DCOUNT,QString("%1").arg(_dataCounter));
        _writer->writeEndElement();
        foreach(ISwRecordManagerListener * listener,_listeners) {
            listener->setEnableRecordInformation(false);
        }
    }
}            
//---------------------------------------------------------------------
// Interface ISwService
//---------------------------------------------------------------------            
/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
void _RecordManager::Liberate(){

}  
//---------------------------------------------------------------------
// Interface ISwHost
//---------------------------------------------------------------------
/*! \brief acces a son composant hote */
SwComponent_Class * _RecordManager::GetHostComponent(){
    return this;
}  
//---------------------------------------------------------------------
// Interface ISwWidget
//---------------------------------------------------------------------
/*! \brief Renvoie le widget
\return le widget */
QWidget & _RecordManager::GetWidget() {
    return *_widget;
}
//--------------------------------------------------------------
//Properties getter and setter
//--------------------------------------------------------------
/** @brief recordDirectory */
SwFileDescriptor _RecordManager::getRecordDirectory() const {
    return _repository;
}
void _RecordManager::setRecordDirectory(const SwFileDescriptor & val){
    _repository=val;
}
/** @brief record */
bool _RecordManager::getEnableRecording() const{
    return _enableRecording;
}
void _RecordManager::setEnableRecording(bool val){
    if (_enableRecording==val)
        return;
    _enableRecording=val;
    if (_enableRecording) {
        startRecording();
    } else {
        stopRecording();
    }
}
/** @brief recordMaxSize */
int _RecordManager::getMaxRecordSize() const {
    return _maxRecordSize;
}
void _RecordManager::setMaxRecordSize(int val) {
    _maxRecordSize=val;
}
/** @brief recordConfiguration */
SwFileDescriptor _RecordManager::getRecordConfiguration() const {
    return _configuration;
}
void _RecordManager::setRecordConfiguration(const SwFileDescriptor & val) {
    _configuration=val;
}

/** @brief  Construction et enregistrement du mapping des points d'enregistrement */
void _RecordManager::buildRecordPointMapping() {
    _recordPointMapping.clear();
    QList<ISwRecordPoint *> rpList=_serviceRecord->getRecordPoints();
    int i=0;
    QList<ISwRecordPoint *>::iterator it=rpList.begin();
    while(it!=rpList.end()) {
        _recordPointMapping.insert((*it),i);
        _writer->writeStartElement(CG_RECORD_RP);
        _writer->writeAttribute(CG_RECORD_RP_ATT_NUMBER,QString("%1").arg(i));
        _writer->writeAttribute(CG_RECORD_RP_ATT_NAME,(*it)->getRecordName());
        _writer->writeEndElement();
        i++;
        it++;
    }
}
/** @brief enable internal widget */
bool _RecordManager::getEnableInternalWidget() const {
    return _enabledInternalWidget;
}
void _RecordManager::setEnableInternalWidget(bool val) {
    if (_enabledInternalWidget==val)
        return;
    if (val) {
        _enabledInternalWidget=true;
        _listeners.push_back(_widget);
        _widget->setEnabled(true);
    } else {
        _enabledInternalWidget=false;
        _listeners.removeOne(_widget);
        _widget->setEnabled(false);
    }
}

