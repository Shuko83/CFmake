/*!
 \file _SwPluginsBank_Class.cpp
 \brief Implementation of the class _SwPluginsBank_Class
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QApplication>
#include <QLibrary>
#include <QProcess>
#include <QRegExp>
#include <QDir>
#include <QFile>
#include <QColor>
#include <QDomDocument>
#include <QDomElement>
#include <QDateTime>
#include "_SwPluginsBank_Class.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "SwData_Class.h"
#include "_SwTreeItem.h"

#ifdef Q_WS_WIN
    //Enregistrement du path dans le path applicatif
#include <windows.h>
#include "ImageHlp.h"
#define WIN32_BUFSIZE 30000
TCHAR chNewEnv[WIN32_BUFSIZE];
#define VARNAME_PATH TEXT("PATH")
#define VARNAME_USER TEXT("USERNAME")
#endif

using namespace StreamWork::SwCore;

//--- DLL access 
template <class T> PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva, T* pNTHeader) // 'T' == PIMAGE_NT_HEADERS 
{
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
    unsigned i;

    for ( i=0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++ )
    {
        // This 3 line idiocy is because Watcom's linker actually sets the
        // Misc.VirtualSize field to 0.  (!!! - Retards....!!!)
        DWORD size = section->Misc.VirtualSize;
        if ( 0 == size )
            size = section->SizeOfRawData;

        // Is the RVA within this section?
        if ( (rva >= section->VirtualAddress) && 
             (rva < (section->VirtualAddress + size)))
            return section;
    }

    return 0;
}

template <class T> LPVOID GetPtrFromRVA( DWORD rva, T* pNTHeader, PBYTE imageBase ) // 'T' = PIMAGE_NT_HEADERS 
{
    PIMAGE_SECTION_HEADER pSectionHdr;
    INT delta;

    pSectionHdr = GetEnclosingSectionHeader( rva, pNTHeader );
    if ( !pSectionHdr )
        return 0;

    delta = (INT)(pSectionHdr->VirtualAddress-pSectionHdr->PointerToRawData);
    return (PVOID) ( imageBase + rva - delta );
}


void DumpDllFromPath(const wchar_t* path,int depth) {
    char name[300];
    wcstombs(name,path,300);

    PLOADED_IMAGE image=ImageLoad(name,0);
    if (image==0) {
        qDebug(QString("Unable to find dynamic library :%1").arg(name).toLatin1().data());
        SW_APP->Logger().Log(LogLvl_Critical,QString("Unable to find dynamic library :%1").arg(name));
        return;
    }
    if (image->FileHeader->OptionalHeader.NumberOfRvaAndSizes>=2) {
        PIMAGE_IMPORT_DESCRIPTOR importDesc=
            (PIMAGE_IMPORT_DESCRIPTOR)GetPtrFromRVA(
                image->FileHeader->OptionalHeader.DataDirectory[1].VirtualAddress,
                image->FileHeader,image->MappedAddress);
        while ( 1 )
        {
            // See if we've reached an empty IMAGE_IMPORT_DESCRIPTOR
            if ( (importDesc->TimeDateStamp==0 ) && (importDesc->Name==0) )
                break;

            QString result=QString((const char *)GetPtrFromRVA(importDesc->Name,
                                           image->FileHeader,
                                           image->MappedAddress) );
            qDebug("Checking %s", result.toLatin1().data());
            SW_APP->Logger().Log(LogLvl_Info,QString("Checking %1").arg(result));
            if (0==LoadLibrary(result.utf16())) {
                     DWORD error=GetLastError();
                     qDebug(QString("-->LoadLibrary failed for %1:%2)").arg(result).arg(error).toLatin1().data());
                     SW_APP->Logger().Log(LogLvl_Critical,QString("Check %1 failed :error core:%2").arg(result).arg(error));
                     if (depth>0) {
                        DumpDllFromPath(result.utf16(),depth-1);
                     }
            }
            importDesc++;
        }
    }
    ImageUnload(image);

}

//




/*! \brief Constructeur */
_SwPluginsBank_Class::_SwPluginsBank_Class():QAbstractItemModel() {
    _tree_items=NULL;
    _has_changed=false;
    RebuildModel();
    _data_to_factory.insert(SwUUID(),NULL);
    _trayIcon=new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon(":/SwCore/SysTrayUpdate.png"));
    _trayIcon->setVisible(false);
    connect(_trayIcon,SIGNAL(messageClicked ()),this,SLOT(hideDisplayUpdate()));
    DWORD dwRet;
    dwRet = GetEnvironmentVariable(VARNAME_USER, chNewEnv, WIN32_BUFSIZE);
    if (dwRet<=WIN32_BUFSIZE) {
#ifdef UNICODE
       userName = QString::fromUtf16(chNewEnv);
#else
       userName = QString::fromLocal8Bit(chNewEnv);
#endif
    }

}
/*! \brief Destructeur */
_SwPluginsBank_Class::~_SwPluginsBank_Class(){
    QMap<QString,SwPluginFactory_Class *>::iterator it;

    for(it=_plugin_by_name.begin();it!=_plugin_by_name.end();it++) {
        SW_APP->RemoveServicesManagerObserver(it.value());
        it.value()->Liberate();
    }
    for(it=_plugin_by_name.begin();it!=_plugin_by_name.end();it++) {
        delete it.value();
    }
    _controllers.clear();
	object_factory_map.clear();
}
//------------------------------------------------------------------
// Interface ISwPluginsBank
//------------------------------------------------------------------
/*! \brief Ajouter un path
\param[in] path path a ajouter*/
void _SwPluginsBank_Class::AddPath(QString path,bool registerable){
    QMap<QString,bool>::iterator it;
    QMap<QString,TL_plugins>::iterator itpp;
    QSet<QString>::const_iterator itc;
    QSet<SwUUID>::iterator itd;
    QSet<QString> set_of_components;
    QSet<SwUUID> set_of_data;
    TL_plugins set_of_plugins;
    QString realPath;

    realPath=path;
    //Si le repertoire existe
    if (QDir::isRelativePath(path)) {
        realPath=qApp->applicationDirPath()+"/"+path;
    }

    //Si le path existe deja,
    it=_paths.find(realPath);
    if (it!=_paths.end()) {
        //S'il n'est pas registerable mais que dans la demande il l'est, on change l'etat
        if (!it.value() && registerable) it.value()=registerable;
        return;
    }
#ifdef Q_WS_WIN
    //Enregistrement du path dans le path applicatif
    DWORD dwRet;
    dwRet = GetEnvironmentVariable(VARNAME_PATH, chNewEnv, WIN32_BUFSIZE);
    if (dwRet<=WIN32_BUFSIZE) {
        QString tmppath=realPath;
        tmppath=tmppath.replace('/','\\');
        #ifdef UNICODE
        QString qstrMessage = QString::fromUtf16(chNewEnv);
        #else
        qstrMessage = QString::fromLocal8Bit(chNewEnv);
        #endif
        qstrMessage+=";";
        qstrMessage+=tmppath;
#ifndef QT_NO_DEBUG
        SW_APP->Logger().Log(LogLvl_Warning,QString(">>>> path size %1\n").arg(qstrMessage.length()));
#endif
        #ifdef UNICODE
        bool result=SetEnvironmentVariable(VARNAME_PATH, (wchar_t*) qstrMessage.utf16());
        #else
        bool result=SetEnvironmentVariable(VARNAME_PATH, qstrMessage.local8Bit().constData());
        #endif
        if (!result) 
            SW_APP->Logger().Log(LogLvl_Warning,QString("Failed to set PATH environment variable\n"));
        else
            if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("PATH= %1\n").arg(qstrMessage));

    } else {
        QString msg=QString("Failed to get PATH environment variable");
        LAUNCH_SWEXCEPTION("SwPluginsBank_Class",msg)
    }
#endif
    //Ajout a la liste des path
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("Adding path %1\n").arg(realPath));
    _paths.insert(realPath,registerable);
    _plugins_paths.insert(realPath,set_of_plugins);
    itpp=_plugins_paths.find(realPath);
    _has_changed=true;
    //Si le repertoire existe
    QDir dir(realPath);
    if (!dir.exists()) {
        //Il n'existe pas on ne fait rien
        return;
    }
    //Operation de mise a jour
    doUpdate(realPath);
    //Recuperation de la liste des fichiers de ce repertoire
    QStringList list_files=dir.entryList(QDir::Files);
    //Pour chaque fichier
    for(int j=0;j<list_files.count();j++) {
        //Si le fichier n'est pas reference
		if (_plugin_by_name.find(list_files[j])==_plugin_by_name.end() &&
#ifndef QT_NO_DEBUG
		list_files[j].endsWith(".swdld")) {
#else
		list_files[j].endsWith(".swdl")) {
#endif			
            QString real_file=realPath;
            real_file+="/";real_file+=list_files[j];
            //On recherche le point d'entrée du plugin
			QLibrary lib(real_file);
#ifndef QT_NO_DEBUG
            Tf_getPluginEntry plugin_entry=(Tf_getPluginEntry)lib.resolve("GetPluginInterfaceD");
#else
            Tf_getPluginEntry plugin_entry=(Tf_getPluginEntry)lib.resolve("GetPluginInterface");
#endif			
			if (plugin_entry!=NULL) {
                //Si trouvé extraction du plugin
                SwPluginFactory_Class * plugin=plugin_entry();
                //Initialisation du plugin
                plugin->SetPath(QDir::cleanPath(realPath));
                plugin->SetPluginName(list_files[j]);
                plugin->Initialize();
                SW_APP->AddServicesManagerObserver(plugin);
                //Info
                if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("\tAdding plugin %1\n").arg(list_files[j]));
                //Enregistrement du plugin dans la banque
                itpp.value().insert(plugin);
                _plugin_by_name.insert(list_files[j],plugin);
                //Enregistrement des composants dans la bank
                set_of_components=plugin->GetComponentsList();
                for (itc=set_of_components.begin();itc!=set_of_components.end();itc++) {
                    //Si un composant de même nom n'est pas enregistrer
                    if (_comp_to_factory.find(*itc)==_comp_to_factory.end()) {
                        //info
                        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("\t\tRegistering component %1\n").arg(*itc));
                        //Enregistrement du composant
                        _comp_to_factory.insert(*itc,plugin);
                    } else {
                        //Signaler le pb
                    }
                }
                //Enregistrement des data types
                set_of_data=plugin->GetDataList();
                for (itd=set_of_data.begin();itd!=set_of_data.end();itd++) {
                    //Si un type de données de même nom n'est pas enregistrer
                    if (_data_to_factory.find(*itd)==_data_to_factory.end()) {
                        //info
                        if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("\t\tRegistering data type id %1\n").arg(itd->toQString()));
                        //Enregistrement du data type
                        _data_to_factory.insert(*itd,plugin);
                    } else {
                        //Signaler le pb
                    }
                }
                //Enregistrement des composants controllers
                _controllers+=plugin->GetControllersMap();
            } else {
                SW_APP->Logger().Log(LogLvl_Critical,lib.errorString());
                qDebug(QString("QLoadLibrary failed for %1:%2").arg(real_file).arg(lib.errorString()).toLatin1().data()); 
                DumpDllFromPath(real_file.utf16(),5);
                /* if (0==LoadLibrary(real_file.utf16())) {
                    DWORD error=GetLastError();
                     qDebug(QString("LoadLibrary failed for %1:%2)").arg(real_file).arg(error).toLatin1().data()); 
                } */

            }
        }
    }
    RebuildModel();
}
/*! \brief recuperatoin d'une liste de path d'un fichier descripteur */
QList<QString> _SwPluginsBank_Class::getPathsFromFile(QFile *f) {
    QDomDocument doc;
    QString xml_error;
    int error_line,error_column;

    QList<QString> pathList;
 
    f->open(QIODevice::ReadOnly | QIODevice::Text);
    QString desc=QString(f->readAll());
    f->close();

    if (!doc.setContent(desc,&xml_error,&error_line,&error_column)) {
        QString msg=QString("XML Path descriptor Parsing:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(desc);
        LAUNCH_SWEXCEPTION("SwPluginsBank_Class",msg)
    }

    QDomElement root=doc.documentElement();
    if (root.tagName()!="Paths") {
        QString msg=QString("XML Path descriptor don't start with <Paths> balise\n%1").arg(desc);
        LAUNCH_SWEXCEPTION("SwPluginsBank_Class",msg)
    }


    QDomElement pathelt=root.firstChildElement("Path");
    while (!pathelt.isNull()) {
        if (pathelt.hasAttribute("target")) {
            QString path=pathelt.attribute("target");
            QString usage=pathelt.attribute("use","all"); 
#ifndef QT_NO_DEBUG
            if (usage=="all" || usage=="debug") {
                pathList.push_back(path);
                //AddPath(path);
            }
#else
            if (usage=="all" || usage=="release") {
                pathList.push_back(path);
                //AddPath(path);
            }
#endif
        }
        pathelt=pathelt.nextSiblingElement("Path");
    }
    return pathList;
}

/*! \brief Ajouter un descripteur de paths */
void _SwPluginsBank_Class::AddPaths(QString pathsdescriptor) {
    QDomDocument doc;
    QString xml_error;
    /*int error_line,error_column*/;

    QList<QString> pathList;
    QList<QString> pathListUser;

    //Chargement du descripteur default
    QFile * f=new QFile(pathsdescriptor);
    pathList=getPathsFromFile(f);
    //Chargement du descripteur user
    f=new QFile(pathsdescriptor+"."+userName);
    if(f->exists()) {
        pathListUser=getPathsFromFile(f);
        QString upath;
        foreach(upath,pathListUser) {
            QString tmpupath=upath;
            tmpupath.replace("\\Stable\\","\\********\\"); 
            tmpupath.replace("\\Dev\\","\\********\\"); 
            int count=pathList.size();
            bool substitution=false;
            for(int i=0;i<count;i++) {
                QString tmppath=pathList[i];
                tmppath.replace("\\Stable\\","\\********\\"); 
                tmppath.replace("\\Dev\\","\\********\\"); 
                if(tmpupath==tmppath) {
                    pathList[i]=upath;
                    substitution=true;
                    break;
                }
            }
            if (!substitution) {
                pathList.push_back(upath);
            }
            globalUserPathList.append(pathListUser);
        }

    }
    QString path;
    foreach(path,pathList) {
        AddPath(path);
    }

}
/*! \brief Acces a QMap des paths (le champs bool suivant indique s'il doivent etre enregistré
\return la QMap des paths*/
QMap<QString,bool> _SwPluginsBank_Class::GetPathList(){
    return _paths;
}
/*! \brief Acces a liste des plugins usine par path
\param[in] path path conteneur des plugins
\return la liste des plugins associés
\exception SwException Unable to return plugin list of unknown path
*/
QSet<ISwPluginFactory *> & _SwPluginsBank_Class::GetPluginList(QString path)  throw(SwException){
    QMap<QString,TL_plugins>::iterator itpp;

    itpp=_plugins_paths.find(path);
    if (itpp==_plugins_paths.end()) {
        QString msg=QString("Unable to return plugin list of unknown path %1").arg(path);
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    return itpp.value();
}
/*! \brief Acces a liste de tous les plugins */
QMap<QString,SwPluginFactory_Class *> * _SwPluginsBank_Class::GetAllPlugins() {
    return & _plugin_by_name;
}
/*! \brief Acces a la liste des noms de tous les composants
\renvoie la liste des composants*/
const QSet<QString> _SwPluginsBank_Class::GetComponentsList(){
    QMap<QString,SwPluginFactory_Class *>::const_iterator it;
    QSet<QString> liste;

    for (it=_comp_to_factory.begin();it!=_comp_to_factory.end();it++) {
        liste.insert(it.key());
    }
    return liste;
}
/*! \brief Acces a la liste des noms de tous les composants controllers relatif a un type donné*/
QList<QString> _SwPluginsBank_Class::GetControllersListForType(int type_identifier) {
    return _controllers.values(type_identifier);
}
/*! \brief Acces a la description d'un composant
\param[in] component_name nom du composant
\return la description du composant
\exception SwException Unable to find description of unknown component
*/
QString _SwPluginsBank_Class::GetComponentDescription(QString component_name)  throw(SwException){
    QMap<QString,SwPluginFactory_Class *>::const_iterator it;

    if (component_name.isEmpty()) return QString();
    it=_comp_to_factory.find(component_name);
    if (it==_comp_to_factory.end()) {
        QString msg=QString("Unable to find description of unknown component %1").arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    return it.value()->GetComponentDescription(component_name);
}
/*! \brief Acces a l'icone d'un composant
\param[in] component_name nom du composant
\return l'icone du composant
\exception SwException Unable to find icon of unknown component
*/
QIcon _SwPluginsBank_Class::GetComponentIcon(QString component_name) throw(SwException) {
    QMap<QString,SwPluginFactory_Class *>::const_iterator it;

    if (component_name.isEmpty()) return QIcon();
    it=_comp_to_factory.find(component_name);
    if (it==_comp_to_factory.end()) {
        QString msg=QString("Unable to find icon of unknown component %1").arg(component_name);
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    return it.value()->GetComponentIcon(component_name);
}
/*! \brief Creation d'un composant a partir de son nom
\param[in] component_name nom du composant
\return l'instance du composant
\exception SwException Unable to create instance of unknown component
*/
SwComponent_Class * _SwPluginsBank_Class::CreateComponent(QString component_name)  throw(SwException){
    QMap<QString,SwPluginFactory_Class *>::const_iterator it;
    QSet<ISwCreationPostProcessor *>::iterator itp;
    SwComponent_Class * created_component;

    if (!component_name.isEmpty()) {
        it=_comp_to_factory.find(component_name);
        if (it==_comp_to_factory.end()) {
            QString msg=QString("Unable to create instance of unknown component %1").arg(component_name);
            LAUNCH_SWEXCEPTION("SwCore",msg)
        }
        created_component=it.value()->CreateComponent(component_name);
    } else {
        created_component=new SwComponent_Class();
    }
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("Creating component %1\n").arg(component_name));


    //Traitement apres creation
    for(itp=_post_processors.begin();itp!=_post_processors.end();itp++) {
        (*itp)->AfterCreatePostProcess(created_component);
    }

    //fin
    return created_component;
}
/*! \brief Relire le contenu d'un plugin (pour les plugins dont le contenu a changer)
\param[in] plugin handle sur le plugin a relire*/
void _SwPluginsBank_Class::RereadPluginContent(SwPluginFactory_Class * plugin) throw(SwException) {
    QMap<QString,SwPluginFactory_Class *>::iterator it,itd;
    QSet<QString> liste;
    QSet<QString>::const_iterator liste_it;

    liste=plugin->GetComponentsList();
    it=_comp_to_factory.begin();
    //Suppression des composants qui ne sont plus reference
    while (it!=_comp_to_factory.end()) {
        itd=it;
        it++;
        //Recherche si le composant appartient au plugin concerné
        if (itd.value()==plugin) {
            //existe-t-il toujours
            if (liste.find(itd.key())==liste.end()) {
                //non,suppression
                _comp_to_factory.erase(itd);
                _has_changed=true;
            }
        }
    }
    //Ajout des nouveaux composants
    for(liste_it=liste.begin();liste_it!=liste.end();liste_it++) {
        //S'il n'existe pas
        if (_comp_to_factory.find(*liste_it)==_comp_to_factory.end()) {
            //Ajout
            _comp_to_factory.insert(*liste_it,plugin);
            _has_changed=true;
        }
    }
    RebuildModel();
}
/*! \brief Reconstruction du modele*/
void _SwPluginsBank_Class::RebuildModel() {
    QMap<QString,TL_plugins>::const_iterator it;
    TL_pluginsIt itp;
    _SwTreeItem * path_item;
    _SwTreeItem * plugin_item;
    QString s;
    QSet<QString> l_comp;
    QSet<QString>::const_iterator l_comp_it;
    QFont f_bold=QFont();
    f_bold.setBold(true);
    QColor f_color=QColor("navy");


    if (_has_changed || _tree_items==NULL) {
        if (_tree_items!=NULL) {
            //Indiquer un changement
            layoutAboutToBeChanged();
             delete _tree_items;
        }
        QList<QVariant> buildData;
        buildData << "Paths/Plugins/Components" << "Details";
        _tree_items = new _SwTreeItem(buildData,QIcon());
        //Remplissage des données
        for(it=_plugins_paths.begin();it!=_plugins_paths.end();it++) {
            buildData.clear();
            QColor pathColor=QColor("black");
            if (globalUserPathList.contains(it.key())) {
                buildData << "Path (User)" << QDir::cleanPath(it.key());
                pathColor=QColor("darkRed");
            } else {
                buildData << "Path" << QDir::cleanPath(it.key());
            }
            path_item=new _SwTreeItem(buildData,QIcon(":/SwCore/path.png"),_tree_items,pathColor,f_bold);
            _tree_items->appendChild(path_item);
            for(itp=it.value().begin();itp!=it.value().end();itp++) {
                buildData.clear();
                buildData << (*itp)->GetPluginName() << QString("Build %1").arg((*itp)->GetPluginVersion());
                plugin_item=new _SwTreeItem(buildData,QIcon(":/SwCore/plugin.png"),path_item,f_color,f_bold);
                path_item->appendChild(plugin_item);
                l_comp=(*itp)->GetComponentsList();
                for(l_comp_it=l_comp.begin();l_comp_it!=l_comp.end();l_comp_it++) {
                    buildData.clear();
                    buildData << (*l_comp_it) << (*itp)->GetComponentDescription((*l_comp_it));
                    plugin_item->appendChild(new _SwTreeItem(buildData,(*itp)->GetComponentIcon((*l_comp_it)),plugin_item));
                }
            }
        }
        //Indiquer fin du changement
        layoutChanged();
        _has_changed=false;
    }
}
/*! \brief Acces au modèle pour l'affichage*/
QAbstractItemModel * _SwPluginsBank_Class::GetModel() {
    return this;
}
/*! \brief Acces a la liste des noms de tous les composants */
QSet<SwUUID> _SwPluginsBank_Class::GetDataList() {
    QMap<SwUUID,SwPluginFactory_Class *>::const_iterator it;
    QSet<SwUUID> liste;

    for (it=_data_to_factory.begin();it!=_data_to_factory.end();it++) {
        liste.insert(it.key());
    }
    return liste;
}
/*! \brief Acces au nom du type d'une donnée definie par son id */
QString _SwPluginsBank_Class::GetDataTypeNameFromDataTypeId(const SwUUID & id){
    QMap<SwUUID,SwPluginFactory_Class *>::const_iterator it;

    if (id!=SwUUID()) {
        it=_data_to_factory.find(id);
        if (it==_data_to_factory.end()) {
            QString msg=QString("Unable to type name of data type id %1").arg(id.toQString());
            LAUNCH_SWEXCEPTION("SwCore",msg)
        }
        return it.value()->GetDataTypeNameFromDataTypeId(id);
    }
    return CG_SW_DEFAULT_TYPE;
}
/*! \brief Creation d'une data */
SwData_Class * _SwPluginsBank_Class::CreateData(const SwUUID & id) throw(SwException){
    QMap<SwUUID,SwPluginFactory_Class *>::const_iterator it;
    SwData_Class * created_data;

    if (id!=SwUUID()) {
        it=_data_to_factory.find(id);
        if (it==_data_to_factory.end()) {
            QString msg=QString("Unable to create instance of data type id %1 - %2").arg(id.mostSigBits).arg(id.leastSigBits);
            LAUNCH_SWEXCEPTION("SwCore",msg)
        }
        created_data=it.value()->CreateData(id);
    } else {
        created_data=new SwData_Class();
    }

    //fin
    return created_data;
}
 /*! \brief Creation d'un objet */
void * _SwPluginsBank_Class::CreateObject(const SwUUID & uuid,void * parameter) throw(SwException) {
	ISwObjectFactory * factory=object_factory_map[uuid];
	if (factory!=0) {
		return factory->CreateObject(parameter);
	}
	return 0;
}
/*! \brief Enregsitrement d'une fabrique d' objet */
void _SwPluginsBank_Class::RegisterObjectFactory(const SwUUID & uuid,ISwObjectFactory * factory) throw(SwException) {
	object_factory_map.insert(uuid,factory);
}
/*! \brief Enregsitrement d'une fabrique d' objet */
void _SwPluginsBank_Class::UnregisterObjectFactory(const SwUUID & uuid) throw(SwException){
	object_factory_map.remove(uuid);
}
//------------------------------------------------------------------
// Interface QAbstractItemModel
//------------------------------------------------------------------
/*! \brief Returns the number of columns for the children of the given parent */
int _SwPluginsBank_Class::columnCount ( const QModelIndex & parent ) const {
    if (parent.isValid())
        return static_cast<_SwTreeItem*>(parent.internalPointer())->columnCount();
    else
        return _tree_items->columnCount();
}
/*! \brief Returns the number of row for the children of the given parent */
int _SwPluginsBank_Class::rowCount ( const QModelIndex & parent  ) const {
    _SwTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _tree_items;
    else
        parentItem = static_cast<_SwTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
/*! \brief Returns the data stored under the given role for the item referred to by the index */
QVariant _SwPluginsBank_Class::data ( const QModelIndex & index, int role ) const{
    if (!index.isValid())
        return QVariant();

    _SwTreeItem *item = static_cast<_SwTreeItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    if (role == Qt::DecorationRole && index.column()==0) {
        return item->icon();
    }
    if (role == Qt::ForegroundRole) {
        return QVariant(item->foreground());
    }
    if (role == Qt::FontRole) {
        return QVariant(item->font());
    }
    return QVariant();
}
/*! \brief Returns the index of the item in the model specified by the given row, column and parent index */
QModelIndex _SwPluginsBank_Class::index ( int row, int column, const QModelIndex & parent  ) const{
    _SwTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _tree_items;
    else
        parentItem = static_cast<_SwTreeItem*>(parent.internalPointer());

    _SwTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
/*! \brief Returns the parent of the item in the model specified*/
QModelIndex _SwPluginsBank_Class::parent ( const QModelIndex & index ) const {
    if (!index.isValid())
        return QModelIndex();

    _SwTreeItem *childItem = static_cast<_SwTreeItem*>(index.internalPointer());
    _SwTreeItem *parentItem = childItem->parent();

    if (parentItem == _tree_items)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
/*! \brief Returns the data for the given role and section in the header with the specified orientation */
QVariant _SwPluginsBank_Class::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _tree_items->data(section);

    return QVariant();
}

//----------------------------------------------------
//-- Gestion des Creations Post Processor
//----------------------------------------------------
/*! \brief Ajoute un post processeur*/
void _SwPluginsBank_Class::AddCreationPostProcessor(ISwCreationPostProcessor * post_processor) {
    QSet<ISwCreationPostProcessor *>::iterator it;

    it=_post_processors.find(post_processor);
    if (it==_post_processors.end()) {
        _post_processors.insert(post_processor);
    }

}
/*! \brief Supprime un post processeur*/
void _SwPluginsBank_Class::RemoveCreationPostProcessor(ISwCreationPostProcessor * post_processor){
    QSet<ISwCreationPostProcessor *>::iterator it;

    it=_post_processors.find(post_processor);
    if (it!=_post_processors.end()) {
        _post_processors.erase(it);
    }
}
#define NEW_SUFFIX ".new"
#define OLD_SUFFIX ".old"
#define NEW_SUFFIX_LG 4
/*! \brief Operation de mise a jour */
void _SwPluginsBank_Class::doUpdate(QString path) {
    QString new_file,file,old_file;
    QDir dir(path);
    //Recuperation de la liste des fichiers de ce repertoire
    QStringList list_files=dir.entryList(QDir::Files);
    //Pour chaque fichier
    for(int i=0;i<list_files.count();i++) {
        if (list_files[i].endsWith(NEW_SUFFIX)) {
            new_file=list_files[i];
            file=new_file;
            file.remove(file.length()-NEW_SUFFIX_LG,NEW_SUFFIX_LG);
            if (SW_APP->IsVerbose()) 
                SW_APP->Logger().Log(LogLvl_Info,QString("Updating %1 ...").arg(file));
            _updatedFiles.push_back(file);
            old_file=file+OLD_SUFFIX;
            //Si le vieux fichier existe alors on le detruit
            QFile fOldFile(path+"//"+old_file);
            if(fOldFile.exists()) {
                if (!fOldFile.remove())
                    qDebug("fail to remove old\n");
            }
            //Si le fichier courant existe, on le renomme en vieux
            QFile fFile(path+"//"+file);
            if (fFile.exists()) {
                if (fFile.rename(path+"//"+old_file))
                    qDebug("fail to rename current\n");

            }
            //Et le nouveau fichier devient le fichier courant
            QFile fNewFile(path+"//"+new_file);
            if (fNewFile.exists()) {
                if (!fNewFile.rename(path+"//"+file)) {
                    qDebug("fail to rename new\n");
                }
            }
            if (SW_APP->IsVerbose()) 
                SW_APP->Logger().Log(LogLvl_Info,QString("Update %1 done\n").arg(file));
        }
    };

}
/*! \brief Indique qu'il y a eu une mise a jour */
bool _SwPluginsBank_Class::updateHaveDone() {
    return _updatedFiles.count()>0;
}
/*! \brief Affiche les mises a jours */
void _SwPluginsBank_Class::displayUpdate() {
    _trayIcon->setVisible(true);
    QString message="Updated files:\n";
    foreach(QString f, _updatedFiles) {
        message+=f+"\n";
    }
    _trayIcon->showMessage("Update information",message);
}
/*! \brief Affiche les mises a jours */
void _SwPluginsBank_Class::hideDisplayUpdate() {
    _trayIcon->setVisible(false);
}

