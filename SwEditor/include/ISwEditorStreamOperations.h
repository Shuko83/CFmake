/*!
 \file ISwEditorStreamOperations.h
 \brief Operations sur la selection courante d'un stream
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwEditorStreamOperations_H
#define _ISwEditorStreamOperations_H

/*
  * INCLUDES GLOBAUX
  */
#include <QWidget>
#include "Component/Base/SwComponent_Class.h"
#include "Component/Interfaces/ISwObserver.h"



namespace StreamWork
{
	namespace SwEditor
	{
        /*!
		 \class ISwEditorStreamOperations 
		 \brief Operations sur la selection courante d'un stream
		*/
        class ISwEditorStreamOperations 
		{   
		public:
            //-------------------------------------------------------------
            // Ajout composant
            //-------------------------------------------------------------
            /*! \brief Ajoute un component enfant au composant courant*/
			virtual void AddChild() = 0;
			virtual void AddChild(QString plugin_name, QString component_type) = 0;
            //-------------------------------------------------------------
            // Operations de la selection 
            //-------------------------------------------------------------
            /*! \brief Tout selectionner*/
			virtual void SelectAll()=0;
            /*! \brief Tout Deselectionner*/
			virtual void SelectNone()=0;
            /*! \brief Inverser la selection*/
			virtual void SelectInvert()=0;
            /*! \brief Ajouter a la selection*/
			virtual void AddToSelection(QString component_name)=0;
            /*! \brief Retirer de la selection*/
			virtual void RemoveFromSelection(QString component_name)=0;
            //-------------------------------------------------------------
            // Consultation de la selection 
            //-------------------------------------------------------------
            /*! \brief Acces au nombre d'elements selectionnée*/
			virtual int GetSelectedComponentNumber()=0;
            /*! \brief Acces a un element selectionné*/
            virtual SwCore::SwComponent_Class * GetSelectedComponent(int index)=0;
            /*! \brief Acces au nombre d'elements non selectionnée*/
			virtual int GetUnselectedComponentNumber()=0;
            /*! \brief Acces a un element non selectionné*/
            virtual SwCore::SwComponent_Class * GetUnselectedComponent(int index)=0;
            /*! \brief Permet de savoir si un composant est dans la selection*/
            virtual bool IsInSelection(SwCore::SwComponent_Class * component)=0;
            //-------------------------------------------------------------
            // Operations sur la selection
            //-------------------------------------------------------------
            /*! \brief Supprimer la selection courante*/
			virtual void RemoveSelection()=0;
            /*! \brief Copie la selection courante*/
			virtual void CopySelection()=0;
            /*! \brief Coller la selection courante*/
			virtual void PasteSelection()=0;
            /*! \brief Couper la selection courante*/
			virtual void CutSelection()=0;
            //-------------------------------------------------------------
            // Renommage
            //-------------------------------------------------------------
            /*! \brief Couper la selection courante*/
			virtual void RenameComponent(QString old_name,QString new_name)=0;            
            //-------------------------------------------------------------
            // Observations
            //-------------------------------------------------------------
            /*! \brief Attachement d'un observateur sur cette interface*/
            virtual void AttachSelectionObserver(SwCore::ISwObserver * observer)=0;
            /*! \brief Detachement d'un observateur sur cette interface*/
            virtual void DetachSelectionObserver(SwCore::ISwObserver * observer)=0;
        };
	}
}
#endif 
