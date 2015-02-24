
/**
	@file : ISwConfPropertiesObserver.h
	@brief : Interface de gestion des properties de la conf 
	@		 pour la notification des listeners que les properties sont deletées
	@author : CGD
*/

#ifndef _STREAMWORK_SWCORE_ISWCONFPROPERTIESOBSERVER_H
#define _STREAMWORK_SWCORE_ISWCONFPROPERTIESOBSERVER_H


namespace StreamWork 
{
    namespace SwCore 
    {
        /**
         *  @class : ISwConfPropertiesObserver
		 *  @brief : Interface de gestion des properties de la conf 
		 *		     pour la notification des listeners que les properties sont deletées
         */
        class ISwConfPropertiesObserver  
        {
        public:

			
			/**
             * @brief	: 
             */
			virtual void onPropertyDeleted(ISwProperty * propertyDeleted, QString propertyDecoratedName, QString confName = "" ) = 0;
        };
}
}

#endif
