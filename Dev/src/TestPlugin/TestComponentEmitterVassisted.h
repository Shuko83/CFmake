/**
 * @file TestComponentEmitterVassisted.h
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TestComponentEmitterVassisted_H
#define _TestComponentEmitterVassisted_H

/*
  * INCLUDES LOCAUX
  */
#include <SwEnum.h>
#include "SwAssistedComponent.h"

using namespace StreamWork::SwFoundation;

/**
 *	@class TestComponentEmitterVassisted
 *	@brief NoDescription
 */
class TestComponentEmitterVassisted : public SwAssistedComponent
{
protected:

 	//--------------------------------------------------------------
	//Pins
	//--------------------------------------------------------------
    SwPin * _pin;
    int _count;

public:
    /** @brief Constructeur */
    TestComponentEmitterVassisted();
    /** @brief Destructeur */
    virtual ~TestComponentEmitterVassisted();
    virtual void initializeComponent() throw(SwException);
   
	//----------------------------------------------------
    // Interface ISwExecutable_Service
    //----------------------------------------------------
	/*! \brief Initialisation */
	void Initialize(double start_time,ISwExecution_Service * executor) throw (SwException);            
	/*! \brief Demarrage */
	void Start(double current_time) throw (SwException);            
	/*! \brief Execution */
	void Execute(double current_time,bool is_first_call) throw (SwException);            
	/*! \brief Execution */
	void Stop(double current_time);            
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
