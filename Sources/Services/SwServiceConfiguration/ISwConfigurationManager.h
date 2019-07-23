

#ifndef SERVICECONF_ISWCONFIGURATIONMANAGER_H
#define SERVICECONF_ISWCONFIGURATIONMANAGER_H

#include "ISwPropertiesObserver.h"

/**
*  @class : ISwConfigurationManager
*  @brief : Interface de gestion des properties de la conf
*		     pour la notification des listeners que les properties sont deletées
*/
class ISwConfigurationManager
{
	public:

	/**
	* @brief	: Permet d'enregistrer un ConfPropertiesObserver
	* @Param	: ISwConfPropertiesObserver* observer des properties
	*/
	virtual bool registerConfPropertiesObserver(ISwPropertiesObserver * observer) = 0;

	/**
	* @brief	: Permet désenregistrer un ConfPropertiesObserver
	* @Param	: ISwConfPropertiesObserver* observer des properties
	*/
	virtual void unregisterConfPropertiesObserver(ISwPropertiesObserver * observer) = 0;

	virtual void notifyPropertyDeleted(ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName) = 0;
};

#endif
