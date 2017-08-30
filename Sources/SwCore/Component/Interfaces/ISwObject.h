/**
 * @file ISwObject.h
 * @brief Interface de QObject
 * @version 1.0
 * @date 2011/09/22 - 22:9:2011 - 16:35
 * @author AAY
 */
#ifndef _ISWOBJECT_H
#define _ISWOBJECT_H

/*
  * INCLUDES GLOBAUX
  */
#include <QObject>


namespace StreamWork
{
	namespace SwCore
	{
        /*!
		@interface ISwObject 
		@brief interface definissant un QObject
        @ingroup SwCoreGrp
        @swinterface
		*/
        class ISwObject 
		{   
		public:

			/**
			 * @brief    : Renvoie une référence sur l'objet
			 * @return   : QObject & - référence sur l'objet
			 */
			virtual QObject * getObject()=0;
		};
	}
}
#endif 
