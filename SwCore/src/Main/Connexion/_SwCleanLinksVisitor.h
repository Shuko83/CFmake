/**
@file _SwCleanLinksVisitor.h
@brief visitor supprimant les liens en modules lors de leur destruction
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE__SWCLEANLINKSVISITOR_H
#define _STREAMWORK_SWCORE__SWCLEANLINKSVISITOR_H

#include "Component/Interfaces/ISwVisitor.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class _SwCleanLinksVisitor
        @brief visitor supprimant les liens en modules lors de leur destruction
        */
        class _SwCleanLinksVisitor : public ISwVisitor{

        public:
	        /** @brief Constructor */
	        _SwCleanLinksVisitor();
	        /*! \brief methode de visite */
	        virtual void Visit(SwComponent_Class * component);

        };

    }
    
}
#endif
