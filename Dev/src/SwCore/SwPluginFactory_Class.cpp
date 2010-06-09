/*!
 \file SwPluginFactory_Class.cpp
 \brief Implementation of class SwPluginFactory_Class
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QMimeData>
#include <QTextStream>
/*
  * INCLUDES LOCAUX
  */
#include "SwPluginFactory_Class.h"

using namespace StreamWork::SwCore;


/*! \brief Constructeur */
SwPluginFactory_Class::SwPluginFactory_Class()
    :QAbstractListModel(NULL),overview(this) {
    _path=QString();
    _name=QString();
    _component_description.clear();
    _data_type.clear();
    _controllers.clear();
    _controllers_list.clear();
}
/*! \brief Destructeur */
SwPluginFactory_Class::~SwPluginFactory_Class(){
    _component_description.clear();
    _data_type.clear();
    _controllers.clear();
    _controllers_list.clear();
}
/*! \brief Definition path du plugin
\param[in] path path du plugin*/
void SwPluginFactory_Class::SetPath(QString path){
    _path=path;
}
/*! \brief Definition du nom du plugin 
\param[in] plugin_name nom du plugin**/
void SwPluginFactory_Class::SetPluginName(QString plugin_name){
    _name=plugin_name;
}
/*! \brief Initialisation 
\note a surcharger*/
void SwPluginFactory_Class::Initialize(){
    //A surcharger
}
/*! \brief finalisation de l'initialisation */
void SwPluginFactory_Class::FinalizeInitialisation() {
    //A surcharger
}
/*! \brief Liberation
\\note a surcharger*/
void SwPluginFactory_Class::Liberate(){
    //A surcharger
}
/*! \brief enregistrement d'un composant 
\param[in] name nom du composant
\param[in] description description du composant
\exception SwException unable to register component because he's already registered
*/
void SwPluginFactory_Class::RegisterComponent(QString name,QString description) {
    QMap<QString,QString>::const_iterator it;

    it=_component_description.find(name);
    if (it!=_component_description.end()) {
        QString msg=QString("In plugin %1, unable to register component %2 because he's already registered").arg(_path).arg(name);
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    _component_description.insert(name,description);
}
/*! \brief enregistrement d'un controller  
\param[in] type_id type pris en charge (qMetaTypeId<T>())
\param[in] component_name nom du composant qui fait office de controlleur
\exception SwException unable to register controller because related component is undefined
*/
void  SwPluginFactory_Class::RegisterController(int type_id,QString component_name) {
    QMap<QString,QString>::const_iterator it;

    it=_component_description.find(component_name);
    if (it==_component_description.end()) {
        QString msg=QString("In plugin %1, unable to register controller %2 because related component is undefined").arg(_path).arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    _controllers.insert(type_id,component_name);
    _controllers_list.insert(component_name);
}
/*! \brief enregistrement d'une data 
\param[in] type id de la data
\param[in] type name de la data
\exception SwException unable to register data type because he's already registered
*/
void SwPluginFactory_Class::RegisterData(const SwUUID & type_id,QString type_name) {
    QMap<SwUUID,QString>::const_iterator it;

    it=_data_type.find(type_id);
    if (it!=_data_type.end()) {
        QString msg=QString("In plugin %1, unable to register data type %2(%3) because he's already registered").arg(_path).arg(type_name).arg(type_id.toQString());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    _data_type.insert(type_id,type_name);
}
/*! \brief instanciation d'un composant
\param[in] name nom du composant
\return l'instance du composant ou NULL si non trouvé*/
SwComponent_Class * SwPluginFactory_Class::CreateInstanceOf(QString name) {
    return NULL;
}
/*! \brief instanciation d'une data
\param[in] name nom du composant
\return l'instance du composant ou NULL si non trouvé */
SwData_Class * SwPluginFactory_Class::CreateInstanceOf(const SwUUID & type_id) {
    return NULL;
}
/*! \brief acces a l'icone d'un composant
\param[in] name nom du composant
\return l'incone ou icone vide si non trouvé*/
QIcon SwPluginFactory_Class::CreateIconOf(QString name) const{
    QIcon ico;
    return ico;
}
//---------------------------------------------------------------------
// Interface ISwPluginFactory
//---------------------------------------------------------------------
/*! \brief Acces au path du plugin 
\return path du composant*/
QString SwPluginFactory_Class::GetPath(){
    return _path;
}
/*! \brief Acces au nom du plugin
\return nom du plugin*/
QString SwPluginFactory_Class::GetPluginName(){
    return _name;
}
/*! \brief Acces a la version du plugin
\return version*/
QString SwPluginFactory_Class::GetPluginVersion(){
    return QString("unknown"); //A surcharger
}
/*! \brief Acces a la liste des noms de tous les composants 
\return liste des noms des composants*/
QSet<QString> SwPluginFactory_Class::GetComponentsList(){
    QMap<QString,QString>::const_iterator it;
    QSet<QString> liste;

    for (it=_component_description.begin();it!=_component_description.end();it++) {
        liste.insert(it.key());    
    }
    return liste;
}
/*! \brief Acces a la description d'un composant
\param[in] component_name nom du composant
\return description
\exception SwException unable de find description for component because he's undefined
*/
QString SwPluginFactory_Class::GetComponentDescription(QString component_name) throw(SwException){
    QMap<QString,QString>::const_iterator it;

    it=_component_description.find(component_name);
    if (it==_component_description.end()) {
        QString msg=QString("In plugin %1, unable de find description for component %2 because he's undefined").arg(_name).arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    return it.value();
}
/*! \brief Acces a l'icone d'un composant */
QIcon SwPluginFactory_Class::GetComponentIcon(QString component_name) const throw(SwException) {
    QMap<QString,QString>::const_iterator it;
    QIcon icon;

    it=_component_description.find(component_name);
    if (it==_component_description.end()) {
        QString msg=QString("In plugin %1, unable de find icon for component %2 because he's undefined").arg(_name).arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    icon=CreateIconOf(component_name);
    if (icon.isNull()) {
        return QIcon(":/SwCore/unknown.png");    
    }
    return icon;
}
/*! \brief Creation d'un composant
\param[in] component_name nom du composant
\return instance du composant
\exception SwException fail to create component because he's undefined
*/
SwComponent_Class * SwPluginFactory_Class::CreateComponent(QString component_name) throw(SwException){
    SwComponent_Class * component;

    component=CreateInstanceOf(component_name);
    if (component==NULL) {
        QString msg=QString("In plugin %1, fail to create component %2 because he's undefined").arg(_name).arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    component->SetFactoryComponentName(component_name);
    component->InitializeResources();
    component->RegisterService(&overview); \
    return component;
}
/*! \brief Acces au modèle liste */
QAbstractListModel * SwPluginFactory_Class::GetModelList() {
    return this;
}
/*! \brief Acces a la liste des noms de tous les composants */
QSet<SwUUID> SwPluginFactory_Class::GetDataList() {
    QMap<SwUUID,QString>::const_iterator it;
    QSet<SwUUID> liste;

    for (it=_data_type.begin();it!=_data_type.end();it++) {
        liste.insert(it.key());    
    }
    return liste;
}
/*! \brief Acces a la multimap des controlleyr */
QMultiMap<int,QString> & SwPluginFactory_Class::GetControllersMap() {
    return _controllers;
}
/*! \brief Acces au nom du type d'une donnée definie par son id */
QString SwPluginFactory_Class::GetDataTypeNameFromDataTypeId(const SwUUID & id) {
    QMap<SwUUID,QString>::const_iterator it;

    it=_data_type.find(id);
    if (it!=_data_type.end()) {
        return it.value();
    }
    return QString();
}
/*! \brief Creation d'une data */
SwData_Class * SwPluginFactory_Class::CreateData(const SwUUID & id) throw(SwException) {
    SwData_Class * data;

    data=CreateInstanceOf(id);
    if (data==NULL) {
        QString msg=QString("In plugin %1, fail to create data of id type %2 because he's undefined").arg(_name).arg(id.toQString());
        LAUNCH_SWEXCEPTION("SwCore",msg)        
    }
    return data;
}
//---------------------------------------------------------------------
// Interface ISwServicesManager_Listener
//---------------------------------------------------------------------
/*! \brief sur ajout d'un service */
void SwPluginFactory_Class::OnRegisterService(ISwService * service) {

}
/*! \brief sur suppression d'une  interface */
void SwPluginFactory_Class::OnUnregisterService(ISwService * service) {

}
//---------------------------------------------------------------------
// Interface QAbstractListModel
//---------------------------------------------------------------------
/*! \brief Flags du modele*/
Qt::ItemFlags SwPluginFactory_Class::flags(const QModelIndex &index) const {
     Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

     if (index.isValid()) {
         if (_controllers_list.find(data(index, Qt::DisplayRole).toString())!=_controllers_list.end())
            return Qt::ItemFlags();   
         return Qt::ItemIsDragEnabled | defaultFlags;
     } else
         return defaultFlags;

}
/*! \brief list mimeTypes */
QStringList SwPluginFactory_Class::mimeTypes() const{
     QStringList types;
     types << "application/mod_list";
     return types;
}
/*! \brief acces a la mimeData correspondantes */
QMimeData * SwPluginFactory_Class::mimeData(const QModelIndexList &indexes) const{
     QMimeData *mimeData = new QMimeData();
     QString text;

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             text=text+data(index, Qt::DisplayRole).toString();
             text+= ";";
         }
     }

     //mimeData->setText(text);
     mimeData->setData("application/mod_list",text.toLatin1());
     return mimeData;
}
/*! \brief Renvoie le nombre de lignes */
int SwPluginFactory_Class::rowCount(const QModelIndex &parent) const {
    return _component_description.size();   
}
/*! \brief Renvoie la donnée correspondante*/
QVariant SwPluginFactory_Class::data(const QModelIndex &index, int role) const {
    QMap<QString,QString>::const_iterator it;
    int row=index.row();

    if (!index.isValid())
        return QVariant();

    if (index.row() >= _component_description.size())
        return QVariant();

    it=_component_description.begin();
    while (row>0) {it++;row--;}
    if (it==_component_description.end()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return QVariant(it.key());
    }
    if (role == Qt::ForegroundRole && _controllers_list.find(it.key())!=_controllers_list.end()) {
        return QVariant(QColor(Qt::lightGray));
    }
    if (role == Qt::DecorationRole && index.column()==0) {
        QIcon icon=GetComponentIcon(it.key());
        return QVariant(icon);
    }
    if (role == Qt::ToolTipRole || role == Qt::WhatsThisRole) {
        return QVariant(it.value());
    }
    return QVariant();
}
