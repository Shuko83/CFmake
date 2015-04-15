/**
	@file : ISwConfSaver.h
	@brief : Interface Interne pour la sauvegarde de la configuration
	@author : CGD
 */

#ifndef _SX_BASICS_ISWCONFSAVER_H
#define _SX_BASICS_ISWCONFSAVER_H

#include <QString>


namespace StreamWork
{
	namespace SwCore
	{
        /**
          @class : ISwConfSaver
          @brief : Interface Interne pour la sauvegarde de la configuration
        */
        class ISwConfSaver {
        public:
            
            /** @brief : sauvegarde dans un fichier des données de la QString */
            virtual bool saveCallBack(QString file) = 0;         
            
			/** @brief sauvegarde manuelle dans un fichier des données de la QString */
			virtual bool manualSave(QString filedata) = 0;
        };
    }
}

#endif
