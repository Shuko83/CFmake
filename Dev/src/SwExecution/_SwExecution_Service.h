/*!
\file _SwExecution_Service.h
\date 12/04/2006
\brief service pour l'execution
\author  Big
\version 1.0
 */

#ifndef __SwExecution_Service_H
#define __SwExecution_Service_H
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
#include "ISwExecution_Service.h"
#include "ISwExecutable_Service.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;


typedef enum {Timer_exe,Thread_exe,External_exe,Slave_exe} _SwExecutionType;
typedef enum {Normal_mode,Replay_mode,Both_mode} _SwExecutionMode;
/*!
\class _SwExecution_Services
\brief  service pour l'execution
*/
class _SwExecution_Service: public QThread, public ISwExecution_Service {
    Q_OBJECT

    
public:
	/* Constructeur */
	_SwExecution_Service(SwComponent_Class * host);
	/* Destructeur */
	~_SwExecution_Service();
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
	void Prepare(int time_step,bool overload,_SwExecutionType exe_type);
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
    _SwExecutionType _exe_type;
    /* Timer d'execution */
    QTimer * _exe_timer;
    /* fournisseur de temps */
    ISwClockProvider * _clockProvider;

};

#endif
