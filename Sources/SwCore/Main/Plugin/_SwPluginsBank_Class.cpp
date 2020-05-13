/*!
 \file _SwPluginsBank_Class.cpp
 \brief Implementation of the class _SwPluginsBank_Class
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#include <QLibrary>
#include <QProcess>
#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QColor>
#include <QDomDocument>
#include <QDomElement>
#include <QDateTime>
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <windows.h>
#include <psapi.h>
#include <QJsonObject>
#include <QJsonDocument>

#include "_SwPluginsBank_Class.h"
#include "SwApplication.h"
#include "SwMacros.h"
#include "SwData_Class.h"
#include "_SwTreeItem.h"
#include "SwProtectedPluginFactory_Class.h"
#include "cryptlib.h"
#include "rsa.h"
#include "hex.h"
#include "osrng.h"
#include "files.h"


#include <stdlib.h> 

#ifdef Q_OS_WIN
	//Enregistrement du path dans le path applicatif
#include <windows.h>
#include "ImageHlp.h"
#define WIN32_BUFSIZE 30000
#include <QMessageBox>
TCHAR chNewEnv[WIN32_BUFSIZE];
#define VARNAME_PATH "PATH"
#define VARNAME_USER TEXT("USERNAME")
#endif

using namespace StreamWork::SwCore;

/*! \brief list des popups déjà popé pour une DLL pour éviter le flood */
static QList<QString> _msgBoxAllReadyPopup;

static std::string private_key = "30820274020100300D06092A864886F70D01010105000482025E3082025A02010002818100A8B3838DAE1FC7F9F33C643BBF5A3B5B2D3E1A7C94319BD00353B8538CE6F38503B5AD74EBAF5D6BB80870ECD1D1C79BD1E735E70BD02B76BBB06184D3CA4024D87433C49006E1D9EA568F08468F990E8E9D66D3E875D9711B6A30C7EA311871DF77FD503335EDFDB1CF9B58BB8BE8855BA63162B4EDCC3EDD5CBCA5904B0F470201110281800881858EAC467157E58A9309775CC04DBD70D8762596F03135F302D709416D15C3F3A5C9A8ECE0238C818050C7E3DABC8FF62523503137F91438388C1348958500B2000F86410E31C934803312DAA1288A3869FFC47B5216904CD9DF2A1BBC0821B9C708EBEDB8EACDC67F1DCFACFBA5EB51F1F0443B2EB759696BFA4B932151024100C74A47926015FC6109568C552CCE79B80C7E35DEABAA3ACB960C429E2D5DA52E28CC447F725DDD60005943ECBC9F6814D626FFC605518C6FCF9080166D92AA4F024100D8B4EAFE493BF69D75196968CE7BA307CCC04FF9D93DC290ACBF0D0A4F02B74D19C7229FCEED3D176AEAD39CBD5B016B1FA5F32A1106769AA076B4559D27A5890241008CACE73A25B52A9ED96A44F0D4558318814A07E8792CDE355ADB7A51896F476BE0903059F660600787C68A4CC16176A54BDF4B225E1B7230CEC05A6A2F3A5A1902404C7C16B419D8EDA0FC271624FD950C5D1B16B2D0A706BD2400BBE67C1BE2D748637375A1D08FF771AD43D23751E3E2620B2B82FFC9C60BBE1A843FA5BEFEEF2102400F9F8E497279C4FEDF35939C6C53CAB9F713677A703A56CE3959D876AC260FE0F6D9F8E3FA0B90AF4313926280E6BE31FCD8924C0394D407ACADBA3779E6EA69";


int setenv(const char *name, const char *value, int overwrite)
{
	int errcode = 0;
	if(!overwrite) {
		size_t envsize = 0;
		errcode = getenv_s(&envsize, NULL, 0, name);
		if(errcode || envsize) return errcode;
	}
	return _putenv_s(name, value);
}

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

#ifdef _DEBUG	
		if(!_msgBoxAllReadyPopup.contains(QString(name))) {
			QMessageBox::critical(0,"Dynamic load library failed",QString("Unable to find dynamic library %1").arg(name));
			_msgBoxAllReadyPopup.append(QString(name));
		}		
#endif
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
			//qDebug("Checking %s", result.toLatin1().data());
			SW_APP->Logger().Log(LogLvl_Info,QString("Checking %1").arg(result));
			if (0==LoadLibrary(result.toStdWString().c_str())) {
					 DWORD error=GetLastError();
					 qDebug(QString("-->LoadLibrary failed for %1:%2)").arg(result).arg(error).toLatin1().data());
					 SW_APP->Logger().Log(LogLvl_Critical,QString("Check %1 failed :error core:%2").arg(result).arg(error));
					 if (depth>0) {
						 DumpDllFromPath(result.toStdWString().c_str(), depth - 1);
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
		userName = (LPSTR)chNewEnv;
#else
	   userName = QString::fromLocal8Bit(chNewEnv);
#endif
	}
	pluginLicence = getPluginLicence();
}
/*! \brief Destructeur */
_SwPluginsBank_Class::~_SwPluginsBank_Class(){
	QMap<QString,SwPluginFactory_Class *>::iterator it;

	for(it=_plugin_by_name.begin();it!=_plugin_by_name.end();it++) {
		SW_APP->RemoveServicesManagerObserver(it.value());
		it.value()->Liberate();
	}
	qDeleteAll(_plugin_by_name);
	_plugin_by_name.clear();
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

	QString expendedPath = path;
	
	QRegularExpression envVarsRegex("%(.*?)%");
	QRegularExpressionMatchIterator envVarsMatchIt = envVarsRegex.globalMatch(path);
	while (envVarsMatchIt.hasNext())
	{
		QProcessEnvironment sysEnv = QProcessEnvironment::systemEnvironment();
		QRegularExpressionMatch match = envVarsMatchIt.next();
		QString envVarName = match.captured(1);
		QString envVarValue = sysEnv.value(envVarName);
		if (!envVarValue.isEmpty())
		{
			expendedPath.replace(QStringLiteral("%%1%").arg(envVarName), envVarValue);
		}
	}
	
	QString realPath = expendedPath;
	//Si le repertoire existe
	if (QDir::isRelativePath(expendedPath))
	{
		if (!QDir(SW_APP->GetApplicationDirPath()).isRelative())
			realPath = QDir::cleanPath(SW_APP->GetApplicationDirPath() + "/" + expendedPath);
		else
		{
			realPath = QDir::cleanPath(QDir::current().absolutePath() + "/" + SW_APP->GetApplicationDirPath() + "/" + expendedPath);
		}
	}

	//Si le path existe deja,
	it=_paths.find(realPath);
	if (it!=_paths.end()) {
		//S'il n'est pas registerable mais que dans la demande il l'est, on change l'etat
		if (!it.value() && registerable) it.value()=registerable;
		return;
	}

	QDir pathDir(realPath);
	QString qstrMessage(getenv ("PATH"));
	
	qstrMessage.prepend(pathDir.canonicalPath()+";");

	/*bool result=*/setenv(VARNAME_PATH, qstrMessage.toUtf8().constData(), true);
   
	/* Flood pour rien
	if (!result) 
		SW_APP->Logger().Log(LogLvl_Warning,QString("Failed to set PATH environment variable (pathSize : %1)\n").arg(qstrMessage.length()));
	else
		if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("PATH= %1\n").arg(qstrMessage));
	*/

	//Ajout a la liste des path
	if (SW_APP->IsVerbose())
		SW_APP->Logger().Log(LogLvl_Debug,QString("Adding path %1\n").arg(realPath));

	_paths.insert(realPath,registerable);
	_plugins_paths.insert(realPath,set_of_plugins);
	itpp=_plugins_paths.find(realPath);
	_has_changed=true;
	//Si le repertoire existe
	QDir dir(realPath);
	if (!dir.exists()) 
	{
		//Il n'existe pas on ne fait rien
		qCritical() << __FUNCTION__ << " Directory in devpath, doesn't exist <" + path + ">";
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
		list_files[j].endsWith(".swdl")) {
			QString real_file=realPath;
			real_file+="/";real_file+=list_files[j];
			QFileInfo fileInfo(real_file);
			QString libPath = fileInfo.absoluteFilePath();
			//On recherche le point d'entrée du plugin
			QLibrary lib(libPath);
#ifndef QT_NO_DEBUG
			Tf_getPluginEntry plugin_entry = (Tf_getPluginEntry)lib.resolve("GetPluginInterfaceD");
			if (!plugin_entry)
				plugin_entry = (Tf_getPluginEntry)lib.resolve("GetPluginInterface");
			else
				qWarning() << list_files[j] << ": Function GetPluginInterfaceD() will be unused soon!";
#else
			Tf_getPluginEntry plugin_entry=(Tf_getPluginEntry)lib.resolve("GetPluginInterface");
#endif
			if (plugin_entry!=NULL) {
				//Si trouvé extraction du plugin
				SwPluginFactory_Class * plugin=plugin_entry();

				// Check if plugin is protected
				SwProtectedPluginFactory_Class* protectedPlugin = dynamic_cast<SwProtectedPluginFactory_Class*>(plugin);
				if (protectedPlugin && !protectedPlugin->unlock(pluginLicence.c_str()))
				{
					qDebug() << QString("Plugin %1 is protected").arg(plugin->GetPluginName());
					delete plugin;
					continue;
				}

				//Info
				if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug,QString("\tAdding plugin %1\n").arg(list_files[j]));
				if (_plugin_by_name.contains(plugin->GetPluginName()))
				{
					//Signaler le pb
					qDebug() << QString("Plugin %1 already registered").arg(plugin->GetPluginName());
					delete plugin;
					continue;
				}
				//Enregistrement du plugin dans la banque
				itpp.value().insert(plugin);
				_plugin_by_name.insert(plugin->GetPluginName(), plugin);
				//Initialisation du plugin
				plugin->SetPath(QDir::cleanPath(realPath));
				plugin->Initialize();
				SW_APP->AddServicesManagerObserver(plugin);
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
				QMultiMap<int,QString> map_of_controllers = plugin->GetControllersMap();
				for (auto itController = map_of_controllers.begin(); itController != map_of_controllers.end(); itController++) {
					if (_controllers.find(itController.key()) == _controllers.end()) {
						//info
						if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info, QString("\t\tRegistering controller type id %1\n").arg(itd->toQString()));
						//Enregistrement du controllers
						_controllers.insert(itController.key(), qMakePair(plugin->GetPluginName(), itController.value()));
					}
					else {
						//Signaler le pb
					}
				}
			} else {
				SW_APP->Logger().Log(LogLvl_Critical,QString("Unable to load Lib %1").arg(real_file).toLatin1(),lib.errorString());
				qDebug(QString("QLoadLibrary failed for %1").arg(real_file).toLatin1().data()); 
				DumpDllFromPath(real_file.toStdWString().c_str(),5);
				_dllWithError.append(realPath.toLower());
				if(0 == LoadLibrary(real_file.toStdWString().c_str())) {
					DWORD error=GetLastError();
					 qDebug(QString("LoadLibrary failed for %1:%2)").arg(real_file).arg(error).toLatin1().data()); 
				} 

			}
		}
	}
	addPluginToModel(itpp.key(), itpp.value());
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

/* get the plugin licence */
std::string StreamWork::SwCore::_SwPluginsBank_Class::getPluginLicence() const
{
	HANDLE	process = GetCurrentProcess();
	DWORD	size;

	// Process ID
	int		processId = GetCurrentProcessId();

	// Process Name
	char processName[4096];
	size = GetModuleBaseNameA(process, NULL, processName, 4095);
	processName[size] = '\0';

	// Process Times
	FILETIME lpCreationTime;
	FILETIME lpExitTime;
	FILETIME lpKernelTime;
	FILETIME lpUserTime;
	if (!GetProcessTimes(process, &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime))
		return false;

	// User name
	char userName[4096];
	size = 4096;
	GetUserNameA(userName, &size);
	userName[size] = '\0';

	// Computer name
	char computerName[4096];
	size = 4096;
	GetComputerNameA(computerName, &size);
	computerName[size] = 0;

	QJsonObject jsonInfo;
	jsonInfo.insert("ProcessID", processId);
	jsonInfo.insert("ProcessName", processName);
	jsonInfo.insert("ProcessLowDateTime", (int)lpCreationTime.dwLowDateTime);
	jsonInfo.insert("ProcessHighDateTime", (int)lpCreationTime.dwHighDateTime);
	jsonInfo.insert("UserName", userName);
	jsonInfo.insert("ComputerName", computerName);

	// json info to string
	QJsonDocument doc(jsonInfo);
	QString data(doc.toJson(QJsonDocument::Compact));

	try
	{
		// Génération du signer a partir de la clef privé
		std::string privateKeyBin;
		CryptoPP::StringSource(private_key, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(privateKeyBin)));
		CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::PrivateKey privKey;
		privKey.BERDecode(CryptoPP::StringStore(privateKeyBin).Ref());
		CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Signer priv(privKey);

		// génération de la signature
		CryptoPP::AutoSeededRandomPool rng;
		byte *signature = new byte[priv.MaxSignatureLength()];
		size_t size = priv.SignMessage(rng, (const byte*)data.toStdString().c_str(), data.length(), signature);

		// Transformation de la signature de binaire a hexa
		std::string signatureStringBinaire(reinterpret_cast<char const*>(signature), size);
		std::string signatureStringhex;
		CryptoPP::StringSource(signatureStringBinaire, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(signatureStringhex)));
		return signatureStringhex;
	}
	catch (...)
	{
		return "";
	}
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

		for (auto upath : pathListUser) {
			QString tmpupath=upath;
			tmpupath.replace("\\Stable\\","\\********\\",Qt::CaseInsensitive); 
			tmpupath.replace("\\Dev\\","\\********\\",Qt::CaseInsensitive); 
			int count=pathList.size();
			bool substitution=false;
			for(int i=0;i<count;i++) {
				QString tmppath=pathList[i];
				tmppath.replace("\\Stable\\","\\********\\",Qt::CaseInsensitive); 
				tmppath.replace("\\Dev\\","\\********\\",Qt::CaseInsensitive); 
				if(tmpupath.toLower()==tmppath.toLower()) {
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

	for (auto path : pathList) {
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
/*! \brief Acces a la liste des noms de tous les composants controllers relatif a un type donné*/
QList< QPair<QString,QString> > _SwPluginsBank_Class::GetControllersListForType(int type_identifier) {
	return _controllers.values(type_identifier);
}
/*! \brief Acces a la description d'un composant
\param[in] component_name nom du composant
\return la description du composant
\exception SwException Unable to find description of unknown component
*/
QString _SwPluginsBank_Class::GetComponentDescription(QString plugin_name, QString component_name)  throw(SwException){
	QMap<QString,SwPluginFactory_Class *>::const_iterator it;

	if (component_name.isEmpty()) return QString();
	it= _plugin_by_name.find(plugin_name);
	if (it== _plugin_by_name.end()) {
		QString msg=QString("Unable to find description of component %1 from unknown plugin %2").arg(component_name, plugin_name);
		LAUNCH_SWEXCEPTION("SwCore",msg)
	}
	return it.value()->GetComponentDescription(component_name);
}
/*! \brief Acces a l'icone d'un composant
\param[in] component_name nom du composant
\return l'icone du composant
\exception SwException Unable to find icon of unknown component
*/
QIcon _SwPluginsBank_Class::GetComponentIcon(QString plugin_name, QString component_name) throw(SwException) {
	QMap<QString,SwPluginFactory_Class *>::const_iterator it;

	if (component_name.isEmpty() || plugin_name.isEmpty()) return QIcon();
	it= _plugin_by_name.find(plugin_name);
	if (it== _plugin_by_name.end()) {
		QString msg=QString("Unable to find icon of component %1 from unknown plugin %2").arg(component_name, plugin_name);
		LAUNCH_SWEXCEPTION("SwCore",msg)
	}
	return it.value()->GetComponentIcon(component_name);
}
/*! \brief Creation d'un composant a partir de son nom
\param[in] component_name nom du composant
\return l'instance du composant
\exception SwException Unable to create instance of unknown component
*/
SwComponent_Class * _SwPluginsBank_Class::CreateComponent(QString plugin_name, QString component_name)  throw(SwException){
	QMap<QString,SwPluginFactory_Class *>::const_iterator it;
	QSet<ISwCreationPostProcessor *>::iterator itp;
	SwComponent_Class * created_component;

/// TEMP FOR UPDATING STREAMS
	if (!component_name.isEmpty() && plugin_name.isEmpty()) {
		for (SwPluginFactory_Class* plugin : _plugin_by_name)
		{
			created_component = plugin->CreateComponent(component_name);
			if (created_component)
				break;
		}
		if (!created_component) {
			QString msg = QString("Unable to create instance of component %1").arg(component_name);
			LAUNCH_SWEXCEPTION("SwCore", msg)
		}
	}
	else
	if (!component_name.isEmpty() && !plugin_name.isEmpty()) {
		it= _plugin_by_name.find(plugin_name);
		if (it== _plugin_by_name.end()) {
			QString msg=QString("Unable to create instance of component %1 from unknown plugin %2").arg(component_name, plugin_name);
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
	_has_changed = true;
	RebuildModel();
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
	for (QString f : _updatedFiles) {
		message+=f+"\n";
	}
	_trayIcon->showMessage("Update information",message);
}
/*! \brief Affiche les mises a jours */
void _SwPluginsBank_Class::hideDisplayUpdate() {
	_trayIcon->setVisible(false);
}

//---------------------------------------------------------------------------------
void _SwPluginsBank_Class::addPluginToModel(QString name, TL_plugins& plugins)
{
	if (!_tree_items) //Si model non initialisé, initialisation du model
	{
		RebuildModel();
	}
	else //sinon ajout de la ligne 
	{
		beginInsertRow(_tree_items->childCount(), _tree_items);

		_SwTreeItem * path_item;
		_SwTreeItem * plugin_item;
		QFont f_bold = QFont();
		f_bold.setBold(true);
		QColor f_color = QColor("navy");

		QList<QVariant> buildData;
		QColor pathColor = QColor("black");
		if (globalUserPathList.contains(name)) {
			buildData << "Path (User)" << QDir::cleanPath(name);
			pathColor = QColor("green");
		}
		else {
			buildData << "Path" << QDir::cleanPath(name);
		}
		if (_dllWithError.contains(name.toLower()))
		{
			pathColor = QColor("red");
		}
		path_item = new _SwTreeItem(buildData, QIcon(":/SwCore/path.png"), _tree_items, pathColor, f_bold);
		_tree_items->appendChild(path_item);
		for (ISwPluginFactory * plugin : plugins) {
			buildData.clear();
			buildData << plugin->GetPluginName() << QString("Build %1").arg(plugin->GetPluginVersion());
			plugin_item = new _SwTreeItem(buildData, QIcon(":/SwCore/plugin.png"), path_item, f_color, f_bold);
			path_item->appendChild(plugin_item);
			for (QString compName : plugin->GetComponentsList()) {
				buildData.clear();
				buildData << compName << plugin->GetComponentDescription(compName);
				plugin_item->appendChild(new _SwTreeItem(buildData, plugin->GetComponentIcon(compName), plugin_item));
			}
		}
		endInsertRow();
	}
}

/*! \brief Reconstruction du modele*/
void _SwPluginsBank_Class::RebuildModel() {
	QMap<QString, TL_plugins>::iterator jt;

	if (_has_changed || _tree_items == NULL) {
		beginResetModel();
		if (_tree_items != NULL) {
			//Indiquer un changement
			delete _tree_items;
		}
		QList<QVariant> buildData;
		buildData << "Paths/Plugins/Components" << "Details";
		_tree_items = new _SwTreeItem(buildData, QIcon());
		//Remplissage des données
		for (jt = _plugins_paths.begin(); jt != _plugins_paths.end(); jt++) {
			addPluginToModel(jt.key(), jt.value());
		}
		endResetModel();
		_has_changed = false;
	}
}

//---------------------------------------------------------------------------------
void _SwPluginsBank_Class::beginInsertRow(int index, _SwTreeItem * node)
{
	beginInsertRows(createIndex(node->row(), 0, node), index, index);
}

//---------------------------------------------------------------------------------
void _SwPluginsBank_Class::endInsertRow()
{
	endInsertRows();
}