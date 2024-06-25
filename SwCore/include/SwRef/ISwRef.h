/**
@file ISwRef.h
@brief Abstraction d'une entité reference compté
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWREF_H
#define _STREAMWORK_SWCORE_ISWREF_H

#include "Tools/Signal/LibIndeSig.h"
namespace StreamWork {

    namespace SwCore {
    
        /**
        @interface ISwRef
        @brief Abstraction d'une entité reference compté
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwRef {
        public:
			/*! \brief Ajout d'une r?f?rence */
			virtual void _addRef()=0;
			/*! \brief Retrait d'une r?f?rence */
			virtual void _release()=0;
			/*! \brief Permet de connaitre le nombre de r?ferences*/
			virtual unsigned long _getReferencesNb() const=0;

			virtual LibIndeSig::iSignal0 & getOnDestroySignal()=0;
		protected:
			/*! \brief Destructeur */
			virtual ~ISwRef() {};
        
        };

    }
    
}
#endif
