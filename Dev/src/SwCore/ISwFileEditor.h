/**
@file ISwFileEditor.h
@brief Adresse IPV4
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWFILEEDITOR_H
#define _STREAMWORK_SWCORE_ISWFILEEDITOR_H

#include <QStringList>
#include "SwFileDescriptor.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @interface ISwFileEditor
        @brief interface d'un editeur de fichier
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwFileEditor {
        public:
            /** 
            * @brief renvoie le nom de l'editeur
            */            
            virtual QString getFileEditorName()=0;
            /** 
            * @brief renvoie la liste des suffixs pris en charge
            * @return liste des suffixs pris en charge
            */            
            virtual QStringList * getSuffixs()=0;
            /** 
            * @brief edition d'un fichier (methode non bloquante)
            * @param descriteur de fichier
            * @return false si l'edition a échoué
            */            
            virtual bool edit(SwFileDescriptor fd)=0;

        };

    }
    
}
#endif
