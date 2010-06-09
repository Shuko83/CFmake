/*!
 \file _SwModelExportedPin.h
 \brief Class implementant une entite exportable pin
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedPin_H
#define __SwModelExportedPin_H

#include <QString>
#include <SwComponent_Class.h>
#include "_SwModelExportedEntity.h"
#include <ISwPin_Listener.h>
#include <ISwPins_ManagerListener.h>
#include <SwPin.h>
#include <SwPins_Manager_Class.h>
#include <LibIndeSig.h>
#include <SwProperties_Class.h>

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedPin 
	\brief Class implementant une entite exportable pin
*/
class _SwModelExportedPin : public _SwModelExportedEntity, public ISwPin_Listener, public ISwPins_ManagerListener{
	public:
		/*! \brief Service pin manager de l'hote*/
		ISwPins_Manager * _host_pin_manager;
		/*! \brief Pin de l'hote*/
		SwPin * _host_pin;
		/*! \brief Service pin manager interne*/
		SwPins_Manager_Class * _internal_pins_manager;
		/*! \brief Pin interne*/
		SwPin * _internal_pin;
		/*! \brief Service pin manager externe*/
		SwPins_Manager_Class * _external_pins_manager;
		/*! \brief Pin externe*/
		SwPin * _external_pin;
        /*! \brief Link done*/
        bool _link_done;
	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	public:
		/*! \brief Constructeur */
		_SwModelExportedPin();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedPin();
        //-------------------------------------------------------------------------
        //Interface ISwPins_ManagerListener
        //-------------------------------------------------------------------------
	    /*! \brief Sur ajout d'un nouveau pin*/
	    void OnAddPin(SwPin *pin);            
	    /*! \brief Sur suppression d'un pin existant*/
	    void OnRemovePin(SwPin *pin);            
	    /*! \brief Sur connexion d'un pin*/
	    void OnConnectPin(SwPin * local_pin,SwPin * remote_pin);            
	    /*! \brief Sur deconnexion d'un pin*/
	    void OnDisconnectPin(SwPin * local_pin,SwPin * remote_pin);            
        //-------------------------------------------------------------------------
        //Interface ISwPin_Listener
        //-------------------------------------------------------------------------
	    /*! \brief Sur reception d'une donnée*/
	    void OnReceiveData(SwPin *p,SwData_Class *data);            
		

};

#endif