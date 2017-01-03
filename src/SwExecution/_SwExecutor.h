/*!
 \file _SwExecutor.h
 \brief Implementation of the Class _SwExecutor realisant la fonction d'executeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
 */

#ifndef __SwExecutor_H
#define __SwExecutor_H

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
#include <SwPins_Manager_Class.h>
#include <ISwProperty.h>
#include <ISwPin_Listener.h>
#include <ISwExecutor2.h>
#include <ISwAdminSetup.h>
#include "_SwExecution_Service.h"
#include "ISwExecutable_Service.h"
#include "ISwSupportReplay.h"
#include "SwExecutionConstantes.h"
#include "ISwExecution_Service.h"

using namespace StreamWork::SwCore;
using namespace StreamWork::SwExecution;
/*!
	\class _SwExecutor
	\brief Implementation of the Class _SwExecutor generant un router de données basique
	*/
class _SwExecutor : public SwComponent_Class,
	public ISwExecutor2,
	public ISwAdminSetup,
	virtual public ISwExecutable_Service,
	public ISwSupportReplay
{
	Q_OBJECT

public:
	/*! \brief Constructeur */
	_SwExecutor();
	/*! \brief Destructeur */
	virtual ~_SwExecutor();
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
	//----------------------------------------------------
	// Interface ISwExecutor2
	//----------------------------------------------------
	/*! \brief Arrete l'execution du stream
	\return le resultat de l'operation */
	virtual int StreamStop();
	//----------------------------------------------------
	// Interface ISwExecutable
	//----------------------------------------------------
	/*! \brief Initialisation */
	void Initialize(double start_time, ISwExecution_Service * executor) throw (SwException);
	/*! \brief Demarrage */
	void Start(double current_time) throw (SwException);
	/*! \brief Execution */
	void Execute(double current_time, bool is_first_call) throw (SwException);
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
	// Interface ISwSupportReplay
	//----------------------------------------------------
	/*@brief setter replay mode */
	virtual void setReplayMode(bool replayMode);
	/*@brief getter replay mode */
	virtual bool getReplayMode();



	public slots:

	/*! \brief sur fin*/
	void onQuit();

protected:
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
	_SwExecution_Service _exe_service;
	/* liste des executables pour le mode esclave */
	QList<ISwExecutable_Service *> * _exe_list;
	/* replay mode */
	bool _replayMode;
	/* propriété DefaultActivated */
	ISwProperty * _defaultActivated_property;
	/* default _defaultActivated */
	bool    _defaultActivated;

	ISwExecution_Service * _executor;

	bool _doCheckTimer;
};
#endif 
