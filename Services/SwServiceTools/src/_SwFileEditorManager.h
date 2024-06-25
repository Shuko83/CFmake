/**
@file _SwFileEditorManager.h
@brief Implementation du manager des editeurs de fichiers
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWCORE__SWFILEEDITORMANAGER_H
#define _STREAMWORK_SWCORE__SWFILEEDITORMANAGER_H

#include <QMap>
#include <QList>
#include "ISwServiceFileEditorManager.h"

namespace StreamWork {

    namespace SwCore {
    
        /**
        @class _SwFileEditorManager
        @brief Implementation du manager des editeurs de fichiers
        */
        class _SwFileEditorManager : public ISwServiceFileEditorManager{
        private:
            /** @brief base de donnée des editeurs */
            QMap<QString,QList<ISwFileEditor *> *> _db;
        public:
	        /** @brief Constructor */
	        _SwFileEditorManager();
	        /** @brief destructor */
	        ~_SwFileEditorManager();
            //---------------------------------------------------------------------
            // Partie pour les editeurs
            //---------------------------------------------------------------------
            /** 
            * @brief enregistrement d'un editeur de fichier
            * @param editor acces interface editor 
            */
            void registerFileEditor(ISwFileEditor * editor);
            /** 
            * @brief deenregistrement d'un editeur de fichier
            * @param editor acces interface editor 
            */
            void unregisterFileEditor(ISwFileEditor * editor);
            //---------------------------------------------------------------------
            // Partie pour les utlisateurs de l'editeur
            //---------------------------------------------------------------------
            /** 
            * @brief edition d'un fichier (methode non bloquante)
            * @param descriteur de fichier
            * @return false si aucun editeur n'a été trouvé
            */            
            bool edit(SwFileDescriptor fd);

        };

    }
    
}
#endif
