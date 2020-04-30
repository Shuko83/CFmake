/*!
 \file _SwSwitchExecutor.h
 \brief Implementation of the Class _SwSwitchExecutor realisant la fonction d'executeur de plusiseurs listes de modules
 \version 1.0
 \date dec 2008
 \author HLG
*/

#ifndef __SwSwitchExecutor_H
#define __SwSwitchExecutor_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include <SwComponent_Class.h>
#include <SwProperties_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include <ISwPin_Listener.h>
#include <ISwExecutor.h>
#include <ISwAdminSetup.h>
#include "_SwSwitchExecution_Service.h"
#include "ISwExecutable_Service.h"
#include "_SwSwitchExecutionList.h"
#include "ISwSwitchExecutorListProvider.h"
#include "ISwSupportReplay.h"
#include "SwExecutionConstantes.h"

using namespace StreamWork::SwCore;

/*!
	\class _SwSwitchExecutor 
	\brief Implementation of the Class _SwSwitchExecutor realisant la fonction d'executeur de plusiseurs listes de modules
*/

class _SwSwitchExecutor : 
    public SwComponent_Class , 
    public ISwExecutor, 
    public ISwAdminSetup, 
    virtual public ISwExecutable_Service,
    public ISwSwitchExecutorListProvider,
    public ISwSupportReplay
{


public:
    /*! \brief Constructeur */
    _SwSwitchExecutor();
    /*! \brief Destructeur */
    virtual ~_SwSwitchExecutor();
    /*! \brief Initialisation des ressources
          \note tous les services du composants doivent être déclarés dans cette methodes*/
    virtual void InitializeResources() throw(SwException);
    /*! \brief Callback sur les changements de propriétés*/
    void OnPropertyChange(ISwProperty * property);
    //----------------------------------------------------
    // Interface ISwExecutor
    //----------------------------------------------------
    /*! \brief Execute le stream
    \return le resultat de l'operation */
	virtual int StreamExecute();
	/*! \brief Arrete l'execution du stream
	\return le resultat de l'operation */
	virtual int StreamStop();
    //----------------------------------------------------
    // Interface ISwExecutable
    //----------------------------------------------------
	/*! \brief Initialisation */
	void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);            
	/*! \brief Demarrage */
	void Start(double current_time) throw (SwException);            
	/*! \brief Execution */
	void Execute(double current_time,bool is_first_call) throw (SwException);            
	/*! \brief Execution */
	void Stop(double current_time);            
	//---------------------------------------------------------------------
	// Interface ISwService
	//---------------------------------------------------------------------            
	/*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
	lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
	void Liberate();  
	//---------------------------------------------------------------------
	// Interface ISwHost
	//---------------------------------------------------------------------
	/*! \brief acces a son composant hote */
	SwComponent_Class * GetHostComponent();            
    //----------------------------------------------------
    // Interface ISwAdminSetup
    //----------------------------------------------------
	/*! \brief Permet d'acceder aux preferences d'administration*/
	virtual void AdminSetup();
	
    //----------------------------------------------------
    // Interface ISwSwitchExecutorListProvider
    //----------------------------------------------------
    /*! \brief acces au nom de la liste en execution */
    virtual QString GetListName();  
              
    virtual void AddSwitchExectuorListener (ISwSwitchExecutorListener * listener);
    virtual void RemoveSwitchExectuorListener (ISwSwitchExecutorListener * listener);  
    //----------------------------------------------------
    // Interface ISwSupportReplay
    //----------------------------------------------------
    /*@brief setter replay mode */
    virtual void setReplayMode(bool replayMode);
    /*@brief getter replay mode */
    virtual bool getReplayMode();
    
protected:
    
    /** @brief service de fourniture d'interface */
    SwInterfaces_Provider_Class * _provider_service;
                
    /* service de gestion des propriétés */
    SwProperties_Class * _properties_service;
    /* Pas temporel en millisecondes */
    uint _time_step;
    /* propriété Pas temporel */
    ISwProperty * _time_step_property;
    /* overload */
    SwEnum _overload;
    /* propriété overload */
    ISwProperty * _overload_property;
    /* Priority */
    SwEnum _priority;
    /* propriété Priority */
    ISwProperty * _priority_property;
    /* est l'entree de l'execution */
    SwEnum _executable_entry;
    /* propriété est l'entree de l'execution */
    ISwProperty * _executable_entry_property;
    
    /* service contenant gerant la liste des composants executables */
    _SwSwitchExecution_Service _exe_service;
    /* liste des executables pour le mode esclave */
    QList<ISwExecutable_Service *> * _exe_list;

    _SwSwitchExecutionList _switchExecutionList;

    /*! \brief notification de changement de la liste executée
    */
    void notifyListNameChanged();
    
    /* listeners */
    QList<ISwSwitchExecutorListener *>    _listeners;
    /* replay mode */
    bool _replayMode;    

    /* propriété DefaultActivated */
    ISwProperty * _defaultActivated_property;
    /* default _defaultActivated */
    bool    _defaultActivated;
 
    ISwExecution_Service * _executor;    
};
#endif 
