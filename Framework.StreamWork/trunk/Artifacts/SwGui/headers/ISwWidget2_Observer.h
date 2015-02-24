/**
@file ISwWidget2_Observer.h
@brief interface widget observer
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_ISWWIDGET2_OBSERVER_H
#define _STREAMWORK_SWGUI_ISWWIDGET2_OBSERVER_H

#include "ISwWidget.h"

namespace StreamWork {

    namespace SwGui {
    
        /**
        @interface ISwWidget2_Observer
        @brief interface widget observer (voir StreamWork::SwGui::ISwWidget2 )
        @ingroup SwGuiGrp
        @swinterface
        */
        class ISwWidget2_Observer {

        public:
            /** @brief mettre en avant le widget si c'est possible */
            virtual void OnBringToFrontRequest(ISwWidget * w)=0;
        };

    }
    
}
#endif
