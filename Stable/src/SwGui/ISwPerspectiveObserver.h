/**
@file ISwPerspectiveObserver.h
@brief Interface Perspective
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_ISWPERSPECTIVEOBSERVER_H
#define _STREAMWORK_SWGUI_ISWPERSPECTIVEOBSERVER_H

namespace StreamWork {

    namespace SwGui {
    
        class ISwPerspective;
        /**
        @class ISwPerspectiveObserver
        @brief Interface Perspective
        */
        class ISwPerspectiveObserver {

        public:
            /** @brief sur changement de l'activation d'une perspective*/
            virtual void OnEnablePerspectiveChange(ISwPerspective *)=0;
            /** @brief sur changement de le la visibilite d'une perspective*/
            virtual void OnVisiblePerspectiveChange(ISwPerspective *)=0;
        };

    }
    
}
#endif
