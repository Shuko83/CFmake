/*!
 \file ISwEditorStreamsManager.h
 \brief  Manager de streams pour l'editeur de stream
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwEditorStreamsManager_H
#define _ISwEditorStreamsManager_H

/*
 * INCLUDES GLOBAUX
 */
#include <QString>
#include <QVariant>
#include <QGraphicsScene>
#include <ISwObserver.h>
#include <SwComponent_Class.h>
#include "ISwEditorGraphicScene.h"

namespace StreamWork
{
	namespace SwEditor
	{
        /*!
		 \class ISwEditorStreamsManager 
		 \brief Manager de streams pour l'editeur de stream
		*/
        class ISwEditorStreamsManager 
		{   
		public:
            /*! \brief Quitter l'application*/
			virtual void QuitEditor()=0;
            //-------------------------------------------------------------
            // Principales operations de gestion de stream
            //-------------------------------------------------------------
            /*! \brief Crée un nouveau stream vide*/
			virtual void NewStream()=0;
            /*! \brief Ouvrir un stream existant*/
			virtual void OpenStream()=0;
            /*! \brief Sauvegarde du stream courant */
			virtual void SaveStream()=0;
            /*! \brief Sauvegarde du stream courant sous un nom donné*/
			virtual void SaveStreamAs()=0;
            /*! \brief Fermeture du stream courant*/
			virtual void CloseStream()=0;
            //-------------------------------------------------------------
            // Principales operations de navigation au travers des streams
            //-------------------------------------------------------------
            /*! \brief Renvoie le nombre de streams ouverts*/
			virtual int GetStreamsCount()=0;
            /*! \brief Renvoie l'information d'un stream*/
			virtual QVariant GetStreamDescription(int index,int role = Qt::DisplayRole)=0;
            /*! \brief Renvoie le stream courant*/
			virtual int GetCurrentStream()=0;
            /*! \brief Renvoie la racine du stream courant*/
            virtual SwCore::SwComponent_Class * GetCurrentStreamRoot()=0;
            /*! \brief Definition du stream courant*/
			virtual void SetCurrentStream(int index)=0;
            //-------------------------------------------------------------
            // Acces a la scene QT du stream courant
            //-------------------------------------------------------------
            /*! \brief Renvoie la scene graphique relative au stream courant courante*/
			virtual ISwEditorGraphicScene * GetCurrentStreamGraphicScene()=0;
            //-------------------------------------------------------------
            // Observations
            //-------------------------------------------------------------
            /*! \brief Attachement d'un observateur sur cette interface*/
            virtual void AttachStreamManagerObserver(SwCore::ISwObserver * observer)=0;
            /*! \brief Detachement d'un observateur sur cette interface*/
            virtual void DetachStreamManagerObserver(SwCore::ISwObserver * observer)=0;

		};
	}
}
#endif 
