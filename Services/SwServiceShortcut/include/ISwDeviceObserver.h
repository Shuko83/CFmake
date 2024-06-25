/*!
\file ISwDeviceObserver.h
\date 12/04/2006
\brief interface d'observabilité d'un device
\author  AAY
\version 1.0
 */

#ifndef _ISWDEVICEOBSERVER_H
#define _ISWDEVICEOBSERVER_H
/*
  * INCLUDES LOCAUX
 */


namespace StreamWork
{
    namespace SwCore
    {
		class ISwDevice;

        /*!
        \interface ISwDeviceObserver
        \brief interface d'observabilité d'un device
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwDeviceObserver {
        public:

           /**
            * @brief    : Méthode appelée lors d'un appuye sur une touche
            * @param	 : QString  entryName - le nom de l'entrée appuyée
            * @param	 : ISwDevice * device - sur le device
            */
           virtual void notify(QString  entryName, ISwDevice *device) = 0 ; 
        };
    }
}

#endif
