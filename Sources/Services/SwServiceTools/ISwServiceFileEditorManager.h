/**
@file ISwServiceFileEditorManager.h
@brief Manager des editors de fichier
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE_ISWSERVICEFILEEDITORMANAGER_H
#define _STREAMWORK_SWCORE_ISWSERVICEFILEEDITORMANAGER_H

#include "ISwService.h"
#include "ISwFileEditor.h"
#include "SwFileDescriptor.h"

//Nom du service
#define CG_SW_SERVICE_FILE_EDITOR_MANAGER "FileEditorManager"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @interface ISwServiceFileEditorManager
        @brief Manager des editors de fichier
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwServiceFileEditorManager : public ISwService {
        public:
            //---------------------------------------------------------------------
            // Partie pour les editeurs
            //---------------------------------------------------------------------
            /** 
            * @brief enregistrement d'un editeur de fichier
            * @param editor acces interface editor 
            */
            virtual void registerFileEditor(ISwFileEditor * editor)=0;
            /** 
            * @brief deenregistrement d'un editeur de fichier
            * @param editor acces interface editor 
            */
            virtual void unregisterFileEditor(ISwFileEditor * editor)=0;
            //---------------------------------------------------------------------
            // Partie pour les utlisateurs de l'editeur
            //---------------------------------------------------------------------
            /** 
            * @brief edition d'un fichier (methode non bloquante)
            * @param descriteur de fichier
            * @return false si aucun editeur n'a été trouvé
            */            
            virtual bool edit(SwFileDescriptor fd)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}   
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_FILE_EDITOR_MANAGER); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}    
        };

    }
    
}
#endif
