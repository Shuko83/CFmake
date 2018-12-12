/**
@file SwPropertiesPersistentToolbox.h
@brief Boite a outils publique pour l'enregistrement des propriétés
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_SWPROPERTIESPERSISTENTTOOLBOX_H
#define _STREAMWORK_SWCORE_SWPROPERTIESPERSISTENTTOOLBOX_H

#include "SwApplication.h"
#include "ISwProperties.h"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class SwPropertiesPersistentToolbox
        @brief Boite a outils publique pour l'enregistrement des propriétés
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwPropertiesPersistentToolbox {

        public:
	        /*! \brief methode permettant de charger des donnees de propriétés*/
	        static void Load(QDomElement & elt,ISwProperties * properties);
	        /*! \brief methode permettant de sauver des donnees de propriétés */
	        static void Save(QXmlStreamWriter & writer,ISwProperties * properties);

        };

    }
    
}
#endif
