/*!
\file _SwSwitchExecution_Service.h
\date dec 2008
\brief service pour l'execution
\author  HLG
\version 1.0
 */

#ifndef __SwSwitchExecution_Service_H
#define __SwSwitchExecution_Service_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
/*
  * INCLUDES LOCAUX
 */
#include <QThread>
#include <QTimer>
#include <ISwPersistent.h>
#include <ISwService.h>
#include <SwException.h>
#include <ISwProperty.h>
#include "ISwExecution_Service.h"
#include "ISwExecutable_Service.h"
#include "_SwSwitchExecutionList.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;


typedef enum {TimerType,ThreadType,ExternalType,SlaveType} _SwSwitchExecutionType;
/*!
\class _SwSwitchExecution_Services
\brief  service pour l'execution
*/

class _SwSwitchExecution_Service: public QThread, public ISwExecution_Service {
    Q_OBJECT
public:
	/* List des path des composants executables */
    QList<QString> _exe_paths;
	/* List des mode d'execution des composants executables */
    QList<_SwExecutionMode> _exe_modes;
    /* List des interfaces d'execution */
    QList<ISwExecutable_Service *> _exe_servs;
    /* Hote */
    SwComponent_Class * _host;
    /* Est ce la premiere execution */
    bool _is_first_execute;
    /* Indique que la boucle d'execution doit être arretée */
    bool _must_be_stopped;
    /* Indique que la boucle d'execution est arretée */
    bool _is_stopped;
    /* Indique que le stop est necessaire */
    bool _persistentStopNeeded;
    /* Overload */
    bool _overload;
    /* Pas d'execution */
    double _time_step;
    /* Type d'execution */
    _SwSwitchExecutionType _exe_type;
    /* Timer d'execution */
    QTimer * _exe_timer;
    /* fournisseur de temps */
    ISwClockProvider * _clockProvider;
public:
	/* Constructeur */
	_SwSwitchExecution_Service(SwComponent_Class * host,_SwSwitchExecutionList * switchExecutionList);
	/* Destructeur */
	~_SwSwitchExecution_Service();
    /* Edit execution list */
	void Edit();
    //---------------------------------------------------------------------
    // Interface ISwService
    //---------------------------------------------------------------------
    /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
    lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
    void Liberate();  
    //---------------------------------------------------------------------
    // Interface ISwPersistence
    //---------------------------------------------------------------------
	/*! \brief methode permettant de charger des donnees
    \param[in] elt neoud parent
    \param[in] finalizer_manager manager de finalisation
    */
	void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
	/*! \brief methode permettant de sauver des donnees
    \param[in] elt neoud parent
    \param[in] doc document parent
    */
	void Save(QDomElement & elt,QDomDocument &doc);
	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief acces a son composant hote */
	SwComponent_Class * GetHostComponent();            
    //---------------------------------------------------------------------
    // Gestion de l'execution des composants selectionnés
    //---------------------------------------------------------------------
	/*! \brief Resolution des liens */
    void ResolveLinks();
	/*! \brief Acces a la liste des services executables */
    QList<ISwExecutable_Service *> * GetExecutablesList();
	/*! \brief Initialisation de tous les composants */
    void InitializeAll();
	/*! \brief Demarrage de tous les composants */
    void StartAll();
public slots:
	/*! \brief Execution de tous les composants */
    void ExecuteAll();
public:
    /*! \brief Arret de tous les composants */
    void StopAll();
    //---------------------------------------------------------------------
    // Algorithmes d'execution 
    //---------------------------------------------------------------------
	/*! \brief Run periodique */
    void RunPeriodic();
	/*! \brief Run periodique avec gestion de la surcharge*/
    void RunPeriodicWithOverload();
    //---------------------------------------------------------------------
    // Pilotage de l'execution
    //---------------------------------------------------------------------
    /* Preparation pour l'execution*/
	void Prepare(int time_step,bool overload,_SwSwitchExecutionType exe_type);
	/*! \brief Demarrage de l'execution */
    void StartExecution();
    /*! \brief execution */
    void Execution();
	/*! \brief demande d'arret de l'execution*/
    void AskForStopExecution();
	/*! \brief demande d'arret de l'execution*/
    bool IsExecutionStopped();
	/*! \brief Arret de l'execution */
    void StopExecution();
    /*! \brief definition du fournisseur de temps */
    void SetClockProvider(ISwClockProvider * provider);
    //---------------------------------------------------------------------
    // QThread
    //---------------------------------------------------------------------
	/*! \brief run du thread */
    void run();
private :
    _SwSwitchExecutionList * _switchExecutionList;
};

#endif
