/**
@file ISwPluginOverview.h
@brief Info Plugin Parent
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWPLUGINOVERVIEW_H
#define _STREAMWORK_SWCORE_ISWPLUGINOVERVIEW_H

#include <QString>
#include "ISwService.h"

//Nom du service
#define CG_SW_SERVICE_PLUGIN_OVERVIEW "PluginOverview"


namespace StreamWork {

    namespace SwCore {
    
        /**
        @class ISwPluginOverview
        @brief Info Plugin Parent
        */
        class ISwPluginOverview : public ISwService
		{
		public:
            /*! \brief Acces au path du plugin */
            virtual QString GetPath()=0;
            /*! \brief Acces au nom du plugin */
            virtual QString GetPluginName()=0;
            /*! \brief Acces a la version du plugin */
            virtual QString GetPluginVersion()=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PLUGIN_OVERVIEW); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}

        };

    }
    
}
#endif
