/*!
 \file _SwModel_Class.cpp
 \brief Implementation d'un model
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/
#include <QMessageBox>
#include <SwApplication.h>
#include <SwMacros.h>
#include "_SwModel_Class.h"
#include <SwMacros.h>
#include <QUuid>
#include <QRegExp>
#include <SwAddress_ToolBox.h>
#include "_SwModelHost_Class.h"
#include "_SwModelHostSelector.h"

using namespace StreamWork::SwCore;

/*! \brief Constructeur */
_SwModel_Class::_SwModel_Class(): SwComponent_Class(){
    _root_element=NULL;
    _host_entry=NULL;
    _isDefault=true;
    _activationDelegate=0;
}
/*! \brief Constructeur */
_SwModel_Class::_SwModel_Class(QString model_name,SwComponent_Class * root_element,QString ipath,QString filepath): SwComponent_Class(){
    QString s;
    QUuid uuid;

    _root_element=root_element;
    _ipath=ipath;
    _filepath=filepath;
    _model_name=model_name;
    _activationDelegate=0;
    //generation du nom
    uuid=QUuid::createUuid();
    s=uuid.toString();
    s.replace(QRegExp("[{}-]"), "_");
    s=s.toUpper();
    s=model_name+s;
    
    //Changement du nom root pour qu'il soit unique
    _root_element->SetName(s);
    //Enregistrement du stream
    SW_APP->AddNewStream(_root_element);

    //Recuperation de l'host entry
    _host_entry=dynamic_cast<_SwModelHost_Class *>(SwAddress_ToolBox::FindTarget(ipath,_root_element));
    if (_host_entry==NULL) 
	{
        SW_APP->Logger().Log(LogLvl_Critical,QString("Fail to found host_entry for model %1\n").arg(_model_name));   
    }
    _isDefault=false;
}
/*! \brief Destructeur */
_SwModel_Class::~_SwModel_Class(){

    if (_root_element!=NULL) 
        DestroyBinding();
    //Desenregistrement des services
    this->UnregisterService(_pins_service->GetServiceName());
    this->UnregisterService(_consumer_service->GetServiceName());
    this->UnregisterService(_provider_service->GetServiceName());
    this->UnregisterService(_properties_service->GetServiceName());
    //Destruction des services
    delete _pins_service;
    delete _consumer_service;
    delete _provider_service;
    delete _properties_service;
    //Destruction du stream modele
    if (!_isDefault) {
        _host_entry=NULL;
        SW_APP->DestroyStream(_root_element);
        _root_element=0;
    }
}

/*! \brief Initialisation des ressources
\note tous les services du composants doivent être déclarés dans cette methodes*/
void _SwModel_Class::InitializeResources() throw(SwException) {

    //Creation des services
    _consumer_service=new SwInterfaces_Consumer_Class(this) ;
    _provider_service=new SwInterfaces_Provider_Class(this) ;
    _properties_service=new SwProperties_Class(this);
    _pins_service=new SwPins_Manager_Class(this) ;
    //Enregistrement des services
    this->RegisterService(_properties_service);
    this->RegisterService(_consumer_service);
    this->RegisterService(_provider_service);
    this->RegisterService(_pins_service);

    if (!_isDefault && _host_entry)
        CreateBinding();
	else
		SW_APP->Logger().Log(LogLvl_Critical,QString("Fail to bind model\n"));   

    
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("InitializeResources of _SwModel_Class done\n"));
}
/*! \brief Permet de definir le nom de l'instance nomme*/
void _SwModel_Class::SetName(const QString & new_name) throw(SwException) { 
    SwComponent_Class::SetName(new_name);
    if (!_isDefault) {
        QString goodName=SwAddress_ToolBox::BuildUniversalPath(this);
        goodName.replace(QRegExp("[\\.$]"), "_");
        _root_element->SetName(goodName+"__ModelInstance");    
        _root_element->AcceptVisitor(this);
    }
}
/*! \brief La racine du parent racine est mise a jour*/
void _SwModel_Class::RootParentNameHasChanged() {
    if (!_isDefault) {
        QString goodName=SwAddress_ToolBox::BuildUniversalPath(this);
        goodName.replace(QRegExp("[\\.$]"), "_");
        _root_element->SetName(goodName+"__ModelInstance");    
        _root_element->AcceptVisitor(this);
    }    
}
//----------------------------------------------------
// ISwVisitor
//----------------------------------------------------
/*! \brief methode de visite */
void _SwModel_Class::Visit(SwComponent_Class * visited) {
    _SwModel_Class * submodel=dynamic_cast<_SwModel_Class *>(visited);   
    if (submodel!=0) {
        submodel->RootParentNameHasChanged();
    }
}
//----------------------------------------------------
// Interface ISwAdminSetup
//----------------------------------------------------
/*! \brief Permet d'acceder aux preferences d'administration*/
void _SwModel_Class::AdminSetup() {
    if (!_isDefault) {
        //QMessageBox::warning(0,"Model alert","This model is fixed model");
        return;
    }
    _SwModelHostSelector * selector=new _SwModelHostSelector(0,_root_element,_host_entry);
    if (selector->exec()==QDialog::Accepted) {
        DestroyBinding();
        _root_element=selector->GetRootComponent();
        _host_entry=selector->GetModelHost();
        if (_host_entry!=NULL) 
            CreateBinding();
    }
    delete selector;
}
/*! \brief Permet d'acceder au sous stream*/
SwComponent_Class * _SwModel_Class::getSubStream() {
    return _root_element.get();
}
/*! \brief Permet d'acceder au path du fichier correspondant au sous stream*/
QString _SwModel_Class::getSubStreamPath() {
    return _filepath;
}

//----------------------------------------------------
// Gestion liaison entre model et model_host
//----------------------------------------------------
/*! \brief Mise en place du pont entre le model et le model host*/
void _SwModel_Class::CreateBinding() {
    _host_entry->CreateBinding(this);
}
/*! \brief Suppression du pont entre le model et le model host*/
void _SwModel_Class::DestroyBinding() {
    if (_host_entry!=NULL)
        _host_entry->DestroyBinding();
}

//----------------------------------------------------
// ISwActivation
//----------------------------------------------------
bool _SwModel_Class::isActive()
{
     return SwComponent_Class::isActive();
}

void _SwModel_Class::setActive(bool value)
{
   SwComponent_Class::setActive(value);
   if (_activationDelegate != 0)
   {
        _activationDelegate->setActive(isActive()); 
   }
   
}
//----------------------------------------------------
// delegue l activation
//----------------------------------------------------    
void _SwModel_Class::setActivationDelegate (ISwActivable * activable)
{
   _activationDelegate =  activable;
   if (_activationDelegate != 0)
   {
        _activationDelegate->setActive(isActive()); 
   }
}