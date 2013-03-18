/**
 * @file TestComponentReceiverVassisted.h
 * @brief NoDescription
 * @version 1.0
 * @date Tue Apr 28 15:53:53 CEST 2009
 * @author F.Bighelli
 */

#ifndef _TESTCOMPONENTRECEIVERVASSISTED_H
#define _TESTCOMPONENTRECEIVERVASSISTED_H

/*
  * INCLUDES LOCAUX
  */
#include <SwAssistedComponent.h>



//Check namespace needed (for exemple ISwAction need namespace StreamWork::SwGui)
using namespace StreamWork::SwFoundation;

/**
 *	@class TestComponentReceiverVassisted
 *	@brief NoDescription
 */
class TestComponentReceiverVassisted : public SwAssistedComponent
{
protected:

	
    SwPin * _pin;

    double _ltime;
public:
    /** @brief Constructeur */
    TestComponentReceiverVassisted();
    /** @brief Destructeur */
    virtual ~TestComponentReceiverVassisted();
   
    //----------------------------------------------------
    // Interface ISwPin_Listener
    //----------------------------------------------------
    /** @brief Callback sur les changements de propriétés*/
	/** @brief Sur reception d'une donnée*/
	void eventReceiveData(SwPin * src,SwData_Class * data);

	void initializeComponent();
   
  
	    
};
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
