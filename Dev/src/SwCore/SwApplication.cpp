/*!
\file SwApplication.cpp
\date 12/04/2006
\brief Classe d'une application StreamWork
\author  Big
\version 1.0
 */

#include <QApplication>
#include <QMetaType>
#include <QDir>
#include <QStringList>
#include <iostream>
#include "SwApplication.h"
#include "_SwPluginsBank_Class.h"
#include "_SwFileEditorManager.h"
#include "_SwComplexeTypeAdaptersFactoriesBankImpl.h"
#include "SwLoader_Class.h"
#include "SwEnum.h"
#include "SwUUID.h"
#include "SwFileDescriptor.h"
#include "SwIpV4Address.h"
#include "SwTime_ToolBox.h"
#include "ISwCheckService.h"
#ifdef Q_WS_WIN
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#endif

using namespace StreamWork::SwCore;
using namespace std;

//Instance du singleton
SwApplication * _singleton=NULL;
_SwPluginsBank_Class * _bank=NULL;
_SwComplexeTypeAdaptersFactoriesBankImpl * _ctadaptersbank=NULL;
_SwFileEditorManager * _feManager=NULL;
bool            _is_launch=false;

/*! \brief Constructeur*/
SwApplication::SwApplication():SwServicesManager_Class() {
    _startPath=QDir::currentPath();
    if (dynamic_cast<QApplication *>(qApp)!=NULL) {
        _isGuiApp=true;
    } else {
        _isGuiApp=false;        
    }
    //Initialisation des types
    Initialize_Types();
    //Pas d'affichage
    _isVerboseMode=false;
    //Pas d'executeur
    _executor=NULL;
    //Finalisation de l'initialisation
    _initialisationFinalized=false;
    //Creation de la banque de plugin
    _bank=new _SwPluginsBank_Class;
    //Creation de la banque des fabriques des adatpateurs de type complexe
    _ctadaptersbank=new _SwComplexeTypeAdaptersFactoriesBankImpl;
    //Pas de stream
    _streams.clear();
    _current_stream=_streams.begin();
    //Compteur d'historique a 1
    _historic_counter=Q_UINT64_C(1);
}
/*! \brief Destructeur*/
SwApplication::~SwApplication() {
    UnregisterService(_feManager->GetServiceName());
    delete _feManager;
    _singleton=NULL;
    delete _bank;
    _bank=NULL;
    delete _ctadaptersbank;
    _ctadaptersbank=NULL;
}

/*! \brief Acces a l'instance unique
\renvoie l'unique instance de l'application StreamWork*/
SwApplication * SwApplication::GetInstance() {
    if (_singleton==NULL) {
        QString appName=QCoreApplication::applicationName();
        if (appName.isEmpty()) {
            QStringList names=QCoreApplication::applicationFilePath().split(QRegExp("[\\\\/]"));
            if (names.count()>0) {
                QStringList namep=names[names.count()-1].split(".");
                if (namep.count()>0) {
                    QCoreApplication::setApplicationName(namep[0]);
                    QCoreApplication::setOrganizationName("Diginext");
                    QCoreApplication::setOrganizationDomain("diginext.fr");
                }
            }
        }

        _singleton=new SwApplication;
        _singleton->waitOnRestart();       
		//prise en compte des parametres passes en ligne de commande
		_singleton->readParameters();	   	   
    }
    return _singleton;
}
/*! \brief lit les parametres passes */
void SwApplication::readParameters() {
	    QStringList args=QCoreApplication::instance()->arguments();      
		int nbArgs = args.count();		
		bool isAppDirPathChanged  = false;
		
        for(int i=1; i<nbArgs; i++) {
			//debug
            if (args[i]=="-d") {
                this->Verbose();
            }
			//Ajout d'un path
            else if (args[i]=="-ppath" && i+1<nbArgs) {
                QDir dir(args[i+1]);
                if (!dir.exists()) {
                    LAUNCH_SWEXCEPTION("swapplication",QString("Plugin path %1 doesn't exist").arg(args[i+1]));
                } else {
                    this->ComponentsBank().AddPath(dir.path());
                }
            }
            //Ajout d'un descripteur de paths
            else if (args[i]=="-pdesc" && i+1<nbArgs) {
                QFile file(args[i+1]);
                if (!file.exists()) {
                    LAUNCH_SWEXCEPTION("swapplication",QString("Plugin path %1 doesn't exist").arg(args[i+1]));
                } else {
                    this->ComponentsBank().AddPaths(args[i+1]);
                }
            }    
			//Modification de l'application dir path
			else if (args[i]=="-appDirPath" && i+1<nbArgs)  {
				QDir dir(args[i+1]);
                if (!dir.exists()) {
                    LAUNCH_SWEXCEPTION("swapplication",QString("application dir path %1 doesn't exist").arg(args[i+1]));
                } else {
					isAppDirPathChanged = true;
                    this->SetApplicationDirPath(dir.path());					
                }
			}			
		}   //fin for     
		
		if (!isAppDirPathChanged) { 
			//s'il n'y a pas eu de changement de l'appli dir path: on affecte celui par defaut
			this->SetApplicationDirPath(qApp->applicationDirPath());
		}
}		
/*! \brief Acces au path de l'application ou celui configure comme tel par l'utilisateur*/			
const QString & SwApplication::GetApplicationDirPath() {
	return _applicationDirPath;
}
/*! \brief path de l'application ou celui configure comme tel par l'utilisateur*/
void SwApplication::SetApplicationDirPath(const QString & path) {
	//enregistrement du path
	_applicationDirPath = path;
	//ajout des paths pour la banque de plugins
	_bank->AddPath(_applicationDirPath);
	_bank->AddPath(_applicationDirPath+"/sw_plugins");
}
/*! \brief Acces au path de demarrage*/
QString SwApplication::GetWorkingPath() {
    return _startPath;
}

			
/*! \brief Lancement de l'application avec un stream definit dans une chaine
\param[in] stream definit dans une chaine de caractère
\return resultat de l'execution
\exception SwException Only one launch can be done simultaneously
*/
int SwApplication::Launch(QString stream_desc) throw(SwException) {
    int result=0;
    QString xml_error;
    int error_line,error_column;
    SwComponent_ClassPtr root_component;

    FinalizeInitialisation();
    if (_is_launch) {
        LAUNCH_SWEXCEPTION("SwCore","Only one launch can be done simultaneously");    
    }
    //Lancement effectuée
    _is_launch=true;    
    //creation du document xml
    QDomDocument doc;
    //parsing xml du document
    if (!doc.setContent(stream_desc,&xml_error,&error_line,&error_column)) {
        QString msg=QString("XML Parsing:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
        LAUNCH_SWEXCEPTION("SwCore",msg)
    }
    //Construction du stream
    SwLoader_Class loader;
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Loading stream\n"));
    root_component=loader.Load(doc);
    if (root_component==0) {
        LAUNCH_SWEXCEPTION("SwCore","Unable to build stream or stream is empty");
    }
    //Enregistrement du composant
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Registering stream\n"));
    AddNewStream(root_component);
    //Si c'est une application GUI lancer la boucle d'execution
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Launching application execution\n"));
    //Check service
    ISwCheckService * cservice=dynamic_cast<ISwCheckService *>(QueryService(CG_SW_CHECK_SERVICE));
    if (cservice!=0) {
        cservice->check();
    }
    //Indication des mises a jours
    if (_bank->updateHaveDone()) {
        _bank->displayUpdate();
    }
    //Lancement
    if (_isGuiApp) {
        QApplication * gui_app=dynamic_cast<QApplication *>(qApp);
        if (_executor!=NULL) 
            result=_executor->StreamExecute();
        result=gui_app->exec();
    } else {
        //Sinon execution du stream
        if (_executor!=NULL) {
            result=_executor->StreamExecute();
        } else {
            result=-1;
            cout << "No Executor found!!!" <<endl;
        }
    }
    //Fin
     if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("Destroying all streams\n"));
   //Destruction des streams
    _streams.clear(); 
    //Destruction du singleton
    if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Info,QString("End of application execution\n"));
    root_component=0;
    delete this;
    _is_launch=false;
    //Retour
    return result;
}
/*! \brief Finalisation de l'initialisation*/
void SwApplication::FinalizeInitialisation() {
    if (_initialisationFinalized)
        return;

    //Creation de la bd des file editors lors de la phase de finale de l'initialisation
    _feManager=new _SwFileEditorManager();
    RegisterService(_feManager);

    QMap<QString,SwPluginFactory_Class *> * plugins=_bank->GetAllPlugins();
    QMap<QString,SwPluginFactory_Class *>::iterator it=plugins->begin();
    while(it!=plugins->end()) {
        it.value()->FinalizeInitialisation();
        it++;
    }
    //Fin
    _initialisationFinalized=true;
}
/*! \brief Definition de l'executeur (uniquement pour les applications consoles)
\param[in] executor executeur a appelé lors du lancement
*/
void SwApplication::RegisterExecutor(ISwExecutor * executor) {
    _executor=executor;
}
/*! \brief Acces a la banque de gestion des composants (creation composant,ajout path,liste plugins)
\return banque de plugins
*/
ISwPluginsBank & SwApplication::ComponentsBank() {
    return *((ISwPluginsBank *)_bank);    
}
/*! \brief Acces a la banque des fabriques d'adaptateur de type complexe*/
ISwComplexeTypeAdaptersFactoriesBank & SwApplication::CTFactoriesBank() {
    return *((ISwComplexeTypeAdaptersFactoriesBank *)_ctadaptersbank);    
}
/*! \brief Acces a l'interface de log)
\return l'interface de log*/
ISwLogger & SwApplication::Logger() {
    return *((ISwLogger *)&_logger);    
}
/*! \brief Acces a l'interface d'alerte*/
ISwAlerter & SwApplication::Alerter() {
    return *((ISwAlerter *)&_alerter);    
}
/*! \brief Activation du mode d'affichage des actions du core*/
void SwApplication::Verbose() {
    _isVerboseMode=true;
}
/*! \brief connaitre l'etat d'activation du mode d'affichage des actions du core*/
bool SwApplication::IsVerbose() {
    return _isVerboseMode;
}
/*! \brief Ajout d'un stream preconstruit
\param[in] racine du stream*/
void SwApplication::AddNewStream(SwComponent_Class * stream_root) {
    _streams.insert(SwComponent_ClassPtr(stream_root));   
}
/*! \brief Creation d'un stream
\param[in] name_of_stream nom du stream a créer
\return le composant racine
*/
SwComponent_Class * SwApplication::CreateNewStream(QString name_of_stream,QString component_type) {
    SwComponent_ClassPtr component;

    component=_bank->CreateComponent(component_type);
    component->SetName(name_of_stream);
    _streams.insert(component);
    return component;
}
/*! \brief Destruction d'un stream 
\param[in] stream_root composant racine du stream a detruire
\exception SwException Trying to destroy undefined stream
*/
void SwApplication::DestroyStream(SwComponent_Class * stream_root)  throw(SwException){
    QSet<SwComponent_ClassPtr>::iterator it;

    it=_streams.find(SwComponent_ClassPtr(stream_root));
    if (it==_streams.end()) {
        //LAUNCH_SWEXCEPTION("SwCore","Trying to destroy undefined stream");            
    } else {
        if (_current_stream==it) _current_stream++;
        //(*it)->Release();
        _streams.erase(it);
    }
}
/*! \brief Acces au premier enfant
\return le premier stream ou NULL si non trouvé*/
SwComponent_Class * SwApplication::GetFirstStream(){
    SwComponent_Class * stream;
    _current_stream=_streams.begin();
    //Sommes nous la fin
    if (_current_stream==_streams.end()) {
        return NULL;//Oui
    }
    //Non, recuperation de l'enfant
    stream=*_current_stream;
    //Incrementation
    _current_stream++;
    //Renvoie de l'enfant
    return stream;
}
/*! \brief Acces a l'enfant suivant
\return le stream suivant ou NULL si non trouvé*/
SwComponent_Class * SwApplication::GetNextStream(){
    SwComponent_Class * stream;
    //Sommes nous la fin
    if (_current_stream==_streams.end()) {
        return NULL;//Oui
    }
    //Non, recuperation de l'enfant
    stream=*_current_stream;
    //Incrementation
    _current_stream++;
    //Renvoie de l'enfant
    return stream;
}
//------------------------------------------------------------------------
// Demarrage externe
//------------------------------------------------------------------------
/*! \brief demarre l'execution du stream */
void SwApplication::LaunchAutoStart() {
    QStringList liste_arg=QCoreApplication::instance()->arguments();
    for(int i=0;i<(liste_arg.count()-1);i++) {
        if (liste_arg[i]=="-autostart") {
            i=liste_arg.count();
            if (_executor!=NULL) {
                _executor->StreamExecute();
                _executor=0;
            }
        }
    }
}

/*! \brief Acces au compteur d'historique
\renvoie un nombre unique depuis le debut de l'execution de l'application*/
quint64 SwApplication::GetHistoricCpt() {
    quint64 cpt;

    cpt=_historic_counter;
    _historic_counter++;
    return cpt;
}
/*! \brief Initialisation des types*/
void SwApplication::Initialize_Types() {
    //Enregistrement de SwEnum
    qRegisterMetaType<StreamWork::SwCore::SwEnum>("StreamWork::SwCore::SwEnum");
    //Et de ses methodes de serialisation
    qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwEnum>("StreamWork::SwCore::SwEnum");
    //Enregistrement de SwUUID
    qRegisterMetaType<StreamWork::SwCore::SwUUID>("StreamWork::SwCore::SwUUID");
    //Et de ses methodes de serialisation
    qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwUUID>("StreamWork::SwCore::SwUUID");
    //Enregistrement de SwFileDescriptor
    qRegisterMetaType<StreamWork::SwCore::SwFileDescriptor>("StreamWork::SwCore::SwFileDescriptor");
    //Et de ses methodes de serialisation
    qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwFileDescriptor>("StreamWork::SwCore::SwFileDescriptor");
    //Enregistrement de SwEnum
    qRegisterMetaType<StreamWork::SwCore::SwIpV4Address>("StreamWork::SwCore::SwIpV4Address");
    //Et de ses methodes de serialisation
    qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwIpV4Address>("StreamWork::SwCore::SwIpV4Address");
}
/*! \brief attente suite a un redemarrage*/
void SwApplication::waitOnRestart() {
    //Check si c'est un redemarrage,
    //Si oui recupere l'identifiant
    unsigned int processId=0;
    QStringList liste_arg=qApp->arguments();
    int nb_args=liste_arg.count();
    for(int i=1;i<nb_args && processId==0;i++) {
        if (liste_arg[i]=="-restart" && i+1<nb_args) {
            processId=liste_arg[i+1].toUInt();
        }
    }
    if (processId==0)
        return;
#ifdef Q_WS_WIN
    //Recherche du process origine,
    //Si trouve on attends et sinon, on contenu
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    bool start=false;
    SW_DEBUG("Waiting to restart");
    while (!start) {
        SwTime_ToolBox::InternalSleep((unsigned long)50); //50 ms d'attente  
        if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
            return;
        start=true;
        // Calculate how many process identifiers were returned.
        cProcesses = cbNeeded / sizeof(DWORD);
        // Print the name and process identifier for each process.
        for (unsigned int i = 0; i < cProcesses; i++ ) {
            if (aProcesses[i]==processId)
                start=false;
        }
    }

#endif
}

