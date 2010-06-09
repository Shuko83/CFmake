/**
@file ISwWidget2.h
@brief interface widget observable
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_ISWWIDGET2_H
#define _STREAMWORK_SWGUI_ISWWIDGET2_H

#include "ISwWidget.h"
#include "ISwWidget2_Observer.h"

namespace StreamWork {

    namespace SwGui {
    
        /**
        @class ISwWidget2
        @brief interface widget observable
        */
        class ISwWidget2 : public ISwWidget{
        public:
            /** @brief Enregistrement de l'observer */
            virtual void RegisterISwWidgetObserver(ISwWidget2_Observer *)=0;
            /** @brief Desregistrement de l'observer */
            virtual void UnregisterISwWidgetObserver(ISwWidget2_Observer *)=0;
        };

    }
    
}
#endif
