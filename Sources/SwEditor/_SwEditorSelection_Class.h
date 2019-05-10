/*!
\file _SwEditorSelection_Class.h
\date 12/04/2006
\brief implementation de la selection
\author  Big
\version 1.0
 */

#ifndef __SwEditorSelection_Class_H
#define __SwEditorSelection_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QList>
#include <QSet>
#include <ISwService.h>

/*
  * INCLUDES LOCAUX
 */
#include "ISwEditorStreamOperations.h"

//Nom du service
#define CG_SW_SERVICE_SELECTION "_SwEditorSelection"

namespace StreamWork
{
    namespace SwEditor
    {
        /*!
        \class _SwEditorSelection_Class
        \brief implementation de la selection
        */
        class _SwEditorSelection_Class : public SwCore::ISwService, public ISwEditorStreamOperations {
        private:
            /*! \brief composant hote */
            SwCore::SwComponent_Class * _host_component;
            /* \brief liste des observers*/
            QSet<SwCore::ISwObserver *> _observers;
            /* \brief liste des selections*/
            QList<SwCore::SwComponent_Class *> _selecteds;
            /* \brief liste des nonselections*/
            QList<SwCore::SwComponent_Class *> _unselecteds;
        public:
	        /*! \brief Construsteur */
	        _SwEditorSelection_Class(SwCore::SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~_SwEditorSelection_Class();
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
				\return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_SELECTION); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
				\return le nom du service reel (le nom de l'interface)
				\note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
				lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();
            //-------------------------------------------------------------
            // Ajout composant
            //-------------------------------------------------------------
            /*! \brief Ajoute un component enfant au composant courant*/
			virtual void AddChild(QString component_type=QString());
            //-------------------------------------------------------------
            // Operations de la selection
            //-------------------------------------------------------------
            /*! \brief Tout selectionner*/
			virtual void SelectAll();
            /*! \brief Tout Deselectionner*/
			virtual void SelectNone();
            /*! \brief Inverser la selection*/
			virtual void SelectInvert();
            /*! \brief Ajouter a la selection*/
			virtual void AddToSelection(QString component_name);
            /*! \brief Retirer de la selection*/
			virtual void RemoveFromSelection(QString component_name);
            //-------------------------------------------------------------
            // Consultation de la selection
            //-------------------------------------------------------------
            /*! \brief Acces au nombre d'elements selectionnée*/
			virtual int GetSelectedComponentNumber();
            /*! \brief Acces a un element selectionné*/
            virtual SwCore::SwComponent_Class * GetSelectedComponent(int index);
            /*! \brief Acces au nombre d'elements non selectionnée*/
			virtual int GetUnselectedComponentNumber();
            /*! \brief Acces a un element non selectionné*/
            virtual SwCore::SwComponent_Class * GetUnselectedComponent(int index);
            /*! \brief Permet de savoir si un composant est dans la selection*/
            virtual bool IsInSelection(SwCore::SwComponent_Class * component);
            //-------------------------------------------------------------
            // Operations sur la selection
            //-------------------------------------------------------------
            /*! \brief Supprimer la selection courante*/
			virtual void RemoveSelection();
            /*! \brief Copie la selection courante*/
			virtual void CopySelection();
            /*! \brief Coller la selection courante*/
			virtual void PasteSelection();
            /*! \brief Couper la selection courante*/
			virtual void CutSelection();
            //-------------------------------------------------------------
            // Renommage
            //-------------------------------------------------------------
            /*! \brief Couper la selection courante*/
			virtual void RenameComponent(QString old_name,QString new_name);
            //-------------------------------------------------------------
            // Observations
            //-------------------------------------------------------------
            /*! \brief Attachement d'un observateur sur cette interface*/
            virtual void AttachSelectionObserver(SwCore::ISwObserver * observer);
            /*! \brief Detachement d'un observateur sur cette interface*/
            virtual void DetachSelectionObserver(SwCore::ISwObserver * observer);
             /*! \brief Signale un changement aux observers*/
	        void SignalChangeToObservers();
       };
    }
}

#endif
