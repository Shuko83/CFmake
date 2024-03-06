/**
* @file _ReplayManager.h
* @brief _ReplayManager
* @version 1.0
* @date Tue Apr 28 15:55:16 CEST 2009
* @author F.Bighelli
*/

#ifndef __REPLAYMANAGER_H
#define __REPLAYMANAGER_H

/*
* INCLUDES LOCAUX
*/
#include "Types/SwEnum.h"
#include "ISwReplayControler.h"
#include "_SwServiceRecording.h"
#include "Types/SwFileDescriptor.h"
#include "_TimeLine.h"
#include "ISwReplayListener.h"
#include "SwAssistedComponent.h"
//#include "_ReplayWidget.h"

//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwCore;
using namespace StreamWork::SwRecord;
using namespace StreamWork::SwFoundation;

/**
*	@class _ReplayManager
*	@brief _ReplayManager
*/
class _ReplayManager : 
	public SwAssistedComponent,
	virtual public ISwReplayControler
{
	Q_OBJECT
	Q_PROPERTY(StreamWork::SwCore::SwFileDescriptor recordDirectory READ getRecordDirectory WRITE loadReplay)
	Q_PROPERTY(bool replay READ getEnableReplay WRITE setEnableReplay)
	Q_PROPERTY(QString executor READ getExecutor WRITE setExecutor)
	Q_PROPERTY(QString activator READ getActivator WRITE setActivator);
	Q_PROPERTY(int timerStep READ getTimerStep WRITE setTimerStep)
	Q_PROPERTY(int jumpTime READ getJumpTime WRITE setJumpTime)

public:
	/**
	 * @brief    : Constructeur
	 * @param	 : _SwServiceRecording * serviceRecord - pointeur sur le service de record
	 */
	_ReplayManager(_SwServiceRecording * serviceRecord);
	
	/**
	 * @brief    : Destructeur
	 */
	virtual ~_ReplayManager();

	/**
	* @brief    : Initialisation du composant
	*/
	virtual void initializeComponent() throw(SwException);


	void startReplay(StreamWork::SwExecution::ISwExecution_Service * exeService);


	//---------------------------------------------------------------------
	// Interface ISwReplayManager
	//---------------------------------------------------------------------
	/**
     * @brief    : definition du repertoire d'enregistrement
     * @param	 : QString directoryName - Chemin complet du répertoire
     */
    virtual void loadReplay(QString directoryName);

    /**
     * @brief    : Retourne le fichier de configuration
     * @return   : QString - chemin complet 
     */
    virtual QString getConfigurationFile();

    /**
     * @brief    : Demarrage du rejeu avec l'executeur de pilotage
     * @param	 : StreamWork::SwExecution::ISwExecution_Service * exeService - Pointeur sur le servide d'éxecution
     */
    virtual void startReplay();

    /**
     * @brief    : Arret du rejeu
     */
    virtual void stopReplay();

	/**
     * @brief    : Pause du rejeu
     */
    virtual void pauseReplay();

    /**
     * @brief    : Ajout de listener
     * @param	 : ISwReplayManagerListener * listener - Pointeur sur un listener
     */
    virtual void addReplayManagerListener(ISwReplayListener * listener);

    /**
     * @brief    : Suppression de listener
	 * @param	 : ISwReplayManagerListener * listener - Pointeur sur un listener
     */
	virtual void removeReplayManagerListener(ISwReplayListener * listener);
	
	/**
     * @brief    : Definit la vitesse de rejeu (si la vitesse d'execution de l'excutor est 50ms, en vitesse 2 l'executor tournera a 25Ms)
     * @param	 : int val - speed (2 = vitesse *2 ) 
     */
    virtual void setSpeed(int val);
	//---------------------------------------------------
	//				Accessors
	//---------------------------------------------------

	SwFileDescriptor getRecordDirectory() const;
	void loadReplay(const SwFileDescriptor & val);

	/** @brief replay */
	bool getEnableReplay() const;
	void setEnableReplay(bool val);

	/** @brief executor */
	QString getExecutor() const;
	void setExecutor(QString val);

	int getTimerStep() const { return _timerStep; }
	void setTimerStep(int val);

	QString getActivator() const { return _activator; }
	void setActivator(QString val );

	int getJumpTime() const;
	void setJumpTime(int val);

protected:

	/*@ service d'enregistrement */
	_SwServiceRecording *_serviceRecord;

	/* @brief repertoire d'enregistrement */
	SwFileDescriptor _repository;

	/* @brief activation du rejeu */
	bool _enableReplay;

	/* @brief adresse de l'executeur pour les tests */
	QString _executorPath;
	QString _activator;
	
	/* @brief executeur*/
	StreamWork::SwExecution::ISwExecution_Service * _executor;

	/* @brief ligne de temps*/
	_TimeLine _timeLine;

	int _timerStep;
	
	QList<ISwReplayListener *> _listeners;
};	
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
