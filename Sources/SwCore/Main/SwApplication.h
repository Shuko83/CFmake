/*!
\file SwApplication.h
\date 12/04/2006
\brief Classe singleton d'une application StreamWork
\author  Big
\version 1.0
 */

#ifndef _SwApplication_H
#define _SwApplication_H
/*
  * INCLUDES LOCAUX
  */
#include "SwException.h"
#include "SwCoreConstantes.h"
#include "ISwExecutor.h"
#include "ISwExecutor2.h"
#include "ISwPluginsBank.h"
#include "ISwComplexeTypeAdaptersFactoriesBank.h"
#include "SwServicesManager_Class.h"
#include "SwLogger_Class.h"
#include "SwAlerter_Class.h"

namespace StreamWork 
{
	namespace SwCore
	{
		class _SwPluginsBank_Class;
		class _SwComplexeTypeAdaptersFactoriesBankImpl;

		/*!
		\class SwApplication
		\brief Classe singleton d'une application StreamWork accessible via la macro SW_APP
        @ingroup SwCoreGrp

        Il y a toujours une seule instance de cette classe dans un executable

        Grace a ce singleton, on accède à tous les streams disponibles et à la banque des plugins disponibles

        Par ailleurs, si l'on souhaite mettre a disposition des services accessibles par tout entité de l'application, on utlisera les fonctionnalités de manager de services (SwServicesManager_Class) de ce singleton. 

        Mise en oeuvre 
        \code 
        //Exemple d'application simple
        #include <exception>
        #include <SwApplication>
        #include <SwMacros>

        using namespace StreamWork::SwCore;

        int main(int argc,char **argv) {
            QApplication app(argc,argv);
            try {
                //Ajout de path eventuel 
                SW_APP->ComponentsBank().AddPath(un_path);
                //Lancement
                return swApp->Launch(my_stream_desc);
            } catch(exception & e) {
                //L'application a levé une exception
                //Traiter l'exception
                //Si on arrive ici, fin anormale
                return -1;
            }
            //fin normale
            return 0;
        }
        \endcode
        Pour plus de details voir les sources du swlauncher.

        Pour acceder a l'unique instance de SwApplication
        \code 
        SwApplication * app=SwApplication::GetInstance();    
        \endcode
        ou mieux utiliser la macro SW_APP
        \code 
        SwApplication * app=SW_APP;    
        \endcode

        Pour executer un stream (dont la description xml est defini dans la chaine my_stream_desc)
        \code
        //Si ce n'est pas une application console
        //Definir le point d'entrée de l'execution via la methode RegisterExecutor
        //Puis
        try {
            //Enregistrer les paths de plugins
            SW_FACTORIES.AddPath(my_path);
            //Lancer l'application
            SW_APP->Launch(my_stream_desc);
        } catch (SwException &e) {
            //Erreur dans l'execution du stream
        }
        \endcode

        Pour accéder a la banque des fabriques
        \code
        SW_APP->ComponentsBank();
        \endcode
        ou mieux utliser la macro SW_FACTORIES

        Pour accéder a l'interface de log
        \code
        SW_APP->Logger();
        \endcode

        Les methodes suivantes permettent de gerer les differents streams
        AddNewStream, CreateNewStream, DestroyStream, GetFirstStream, GetNextStream

        La methode pour obtenir une valeur unique que cours de la session de l'instance est
        GetHistoricCpt;
		
		Les parametres suivants sont pris en compte:
		-ppath						ajouter un path pour la recherche de plugins
		-pdesc						ajouter un fichier de description de path pour la recherche de plugins
		-d 							mode debug/display
		-appDirPath 			change l'application dir path (recuperable via GetApplicationDirPath)

		*/
        class BUILD_SWCORE SwApplication : public SwServicesManager_Class {
        private:
			_SwPluginsBank_Class *_bank;
			_SwComplexeTypeAdaptersFactoriesBankImpl *_ctadaptersbank;
			bool _is_launch;
			bool _isCheck;
			bool _logTime;
			bool _autoStart;
			bool _developerMode;
            /*! \brief Est une application avec ihm (QApplication et non QCoreApplication) */
            bool _isGuiApp;
            /*! \brief mode affichage */
            bool _isVerboseMode;
			/*! \brief services manquant */
			QSet<QString> _missingServices;
            /*! \brief finalisation de l'initialisation */
            bool _initialisationFinalized;
            /*! \brief finalisation de l'initialisation */
            QString _startPath;
			/*! \brief path de l'application ou celui entree comme tel par l'utilisateur */
            QString _applicationDirPath;
            /*! \brief Handle sur l'executeur (Que pour les applications consoles)*/
            ISwExecutor * _executor;
            /*! \brief Handle sur l'executeur2 (Que pour les applications consoles)*/
            ISwExecutor2 * _executor2;
            /*! \brief QSet des stream */
            QSet<SwComponent_ClassPtr> _streams;
            /*! \brief iterateur de parcours sur la QMap du stream */
            QSet<SwComponent_ClassPtr>::iterator _current_stream;
            /*! \brief compteur d'historique*/
            quint64 _historic_counter;
            /*! \brief logger basique*/
            SwLogger_Class _logger;			
            /*! \brief alerter basique*/
            SwAlerter_Class _alerter;
            /*! \brief Initialisation des types*/
            void Initialize_Types();
            /*! \brief Constructeur*/
            SwApplication();
            /*! \brief Destructeur*/
            ~SwApplication();
        public:
            /*! \brief Acces a l'instance unique*/
            static SwApplication * GetInstance();
            /*! \brief Acces au path de demarrage*/
            QString GetWorkingPath();
			/*! \brief Acces au path de l'application ou celui configure comme tel par l'utilisateur*/			
            const QString & GetApplicationDirPath();
			/*! \brief Acces au path de l'application ou celui configure comme tel par l'utilisateur*/
            void SetApplicationDirPath(const QString & path);
            /*! \brief Lancement de l'application avec un stream definit dans une chaine*/
            int Launch(QString stream_desc) throw(SwException);
            /*! \brief Finalisation de l'initialisation*/
            void FinalizeInitialisation();
            /*! \brief Definition de l'executeur (uniquement pour les applications consoles)*/
            void RegisterExecutor(ISwExecutor * executor);
 
            /*! \brief Acces a la banque de gestion des composants (creation composant,ajout path,liste plugins)*/
            ISwPluginsBank & ComponentsBank();
            /*! \brief Acces a la banque des fabriques d'adaptateur de type complexe*/
            ISwComplexeTypeAdaptersFactoriesBank & CTFactoriesBank();
            /*! \brief Acces a l'interface de log*/
            ISwLogger & Logger();
            /*! \brief Acces a l'interface d'alerte*/
            ISwAlerter & Alerter();
            /*! \brief Activation du mode d'affichage des actions du core*/
            void Verbose();
            /*! \brief connaitre l'etat d'activation du mode d'affichage des actions du core*/
            bool IsVerbose() const;
			/*! \brief Activation du log du temps*/
			void enableLogTime();
			/*! \brief connaitre l'etat du log de temps*/
			bool logTime() const;
			/*! \brief Activation de l'autoStart*/
			void enableAutoStart();
			/*! \brief Activation du mode developpeur*/
			void enableDeveloperMode();
			/*! \brief connaitre l'etat du mode developpeur*/
			bool developerMode() const;

            //------------------------------------------------------------------------
            //Gestion de stream multiples
            //------------------------------------------------------------------------
            /*! \brief Ajout d'un stream preconstruit*/
            void AddNewStream(SwComponent_Class * stream_root);
            /*! \brief Creation d'un stream */
            SwComponent_Class * CreateNewStream(QString name_of_stream,QString component_type=QString());
            /*! \brief Destruction d'un stream */
            void DestroyStream(SwComponent_Class * stream_root) throw(SwException);
			/*! \brief Acces au premier stream */
            SwComponent_Class * GetFirstStream();
            /*! \brief Acces au stream suivant */
			SwComponent_Class * GetNextStream();
            //------------------------------------------------------------------------
            // Demarrage externe
            //------------------------------------------------------------------------
            /*! \brief demarre l'execution du stream */
            bool LaunchAutoStart();
            /*! \brief demarre l'execution du stream */
            void StopLaunch();

            //------------------------------------------------------------------------
            //Compteur d'historique (A chaque appel le compteur est incremente de 1)
            //Utile pour connaitre l'ordre de construction des liens entres composants
            //------------------------------------------------------------------------
            /*! \brief Acces au compteur d'historique */
            quint64 GetHistoricCpt();

			// The goal is to push hitoric counter over the highest index to generate 
        	// indices only for new objects and reuse old index for the others.
			void SetHistoricCpt(quint64 value);

		protected :
			virtual void raiseQueryError(QString serviceName);

		};
	}
}

#endif
