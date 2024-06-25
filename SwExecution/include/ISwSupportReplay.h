/**
@file ISwSupportReplay.h
@brief Interface de support de rejeu
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_ISWSUPPORTREPLAY_H
#define _STREAMWORK_SWGUI_ISWSUPPORTREPLAY_H

namespace StreamWork {

    namespace SwExecution {
    
        /**
        @interface ISwSupportReplay
        @brief Interface de support de rejeu
        @ingroup SwExeGrp
        @swinterface
        */
        class ISwSupportReplay {

        public:
            /*@brief setter replay mode */
            virtual void setReplayMode(bool replayMode)=0;
            /*@brief getter replay mode */
            virtual bool getReplayMode()=0;
        };

    }
    
}
#endif
