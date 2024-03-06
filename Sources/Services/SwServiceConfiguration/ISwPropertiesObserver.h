/**
	@file : ISwPropertiesObserver.h
	@brief : Interface de gestion des properties de la conf 
	@		 pour la notification des listeners que les properties sont deletées
	@author : CGD
*/

#ifndef SERVICECONF_ISWPROPERTIESOBSERVER_H
#define SERVICECONF_ISWPROPERTIESOBSERVER_H

#include <QString>
#include "Properties/ISwProperty.h"

using namespace StreamWork::SwCore;
/**
*  @class : ISwPropertiesObserver
*  @brief : Interface de gestion des properties de la conf
*		     pour la notification des listeners que les properties sont deletées
*/
class ISwPropertiesObserver  
{
	public:
		/**
		* @brief	:
		*/
		virtual void onPropertyDeleted(ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName) = 0;
};

#endif
