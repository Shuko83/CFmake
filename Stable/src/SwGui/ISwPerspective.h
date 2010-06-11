/**
@file ISwPerspective.h
@brief Interface Perspective
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_ISWPERSPECTIVE_H
#define _STREAMWORK_SWGUI_ISWPERSPECTIVE_H

#include <QtGui>
#include "ISwPerspectiveObserver.h"

namespace StreamWork {

    namespace SwGui {
    
        /**
        @class ISwPerspective
        @brief Interface Perspective
        */
        class ISwPerspective {

        public:
            /** @brief acces icon */
            virtual QString getPerspectiveIcon()=0;
            /** @brief acces texte */
            virtual QString getPerspectiveLabel()=0;
            /** @brief acces description */
            virtual QString getPerspectiveDescription()=0;
            /** @brief Construction de la vue */
            virtual void buildPerspectiveView(QWidget *)=0;
            /** @brief Destruction de la vue */
            virtual void destroyPerspectiveView(QWidget *)=0;
            /** @brief Permet de savoir si la persective est active */
            virtual bool isPerspectiveEnabled()=0;
            /** @brief Enregistrement de l'observer */
            virtual void registerISwPerspectiveObserver(ISwPerspectiveObserver *)=0;
            /** @brief Desregistrement de l'observer */
            virtual void unregisterISwPerspectiveObserver(ISwPerspectiveObserver *)=0;
            /** @brief Permet de savoir si la persective est visible */
            virtual bool isPerspectiveVisible()=0;
            /** @brief Permet de savoir si la persective est visible */
            virtual void setPerspectiveVisible(bool visibility)=0;

        };

    }
    
}
#endif
