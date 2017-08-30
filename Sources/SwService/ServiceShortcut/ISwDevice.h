/*!
\file ISwDevice.h
\date 12/04/2006
\brief Interface des devices
\author  AAY
\version 1.0
 */

#ifndef _ISWDEVICE_H
#define _ISWDEVICE_H

/*
 * INCLUDES LOCAUX
 */
#include <QString>
#include <QList>

namespace StreamWork
{
    namespace SwCore
    {

		class ISwDeviceObserver;
		class ISwDeviceEntry;

        /*!
        \interface ISwDevice
        \brief Interface des devices
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwDevice {
        public:

           /**
            * @brief    : retourne le nom du device
            * @return   : QString 
            */
           virtual QString getName()  = 0 ; 

		   /**
		    * @brief    : Enregistre un observateur
		    * @param	 : ISwDeviceObserver * obs - pointeur sur l'observateur
		    */
		   virtual void registerObserver(ISwDeviceObserver * obs) = 0;

		   /**
		    * @brief    : Désenregistre un observateur
		    * @param	 : ISwDeviceObserver * obs - pointeur sur l'observateur
		    */
		   virtual void unregisterObserver(ISwDeviceObserver * obs) = 0;

        };
    }
}

#endif
