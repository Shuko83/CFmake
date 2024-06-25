/*!
 \file _SwEditorGIMenu.h
 \brief menu pour un graphics item composant
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwEditorGIMenu_H
#define _SwEditorGIMenu_H

#include <QMenu>
#include "ISwEditorStreamNavigation.h"
#include "ISwEditorStreamOperations.h"
#include "ISwExecution_Service.h"


namespace StreamWork
{
	namespace SwEditor
	{
        /*!
	        \class _SwEditorGIMenu 
	        \brief Implementation of the Class _SwEditorGIMenu qui surcharge une tabbar pour la gestion des streams
        */
        class _SwEditorGIMenu : public QMenu
        {
	        Q_OBJECT
		protected:
            /*! \brief interface operations*/
			ISwEditorStreamOperations *_op;
            /*! \brief interface navigation*/
            ISwEditorStreamNavigation * _nav;
            /*! \brief interface service d'execution*/
            SwExecution::ISwExecution_Service * _exe_serv;
		public:
			/*! \brief constructeur*/
            _SwEditorGIMenu(ISwEditorStreamOperations *op,ISwEditorStreamNavigation * nav,QWidget *parent=0);
			/*! \brief destructeur*/
            ~_SwEditorGIMenu();
        public slots:
            //-------------------------------------------------------------
            // Navigation dans l'arbre du stream
            //-------------------------------------------------------------
             /*! \brief Remonter au parent*/
			void GoToParent();
            //-------------------------------------------------------------
            // Operations de la selection 
            //-------------------------------------------------------------
            /*! \brief Tout selectionner*/
			void SelectAll();
            /*! \brief Tout Deselectionner*/
			void SelectNone();
            /*! \brief Inverser la selection*/
			void SelectInvert();
            //-------------------------------------------------------------
            // Operations sur la selection
            //-------------------------------------------------------------
            /*! \brief Supprimer la selection courante*/
			void RemoveSelection();
            /*! \brief Copie la selection courante*/
			void CopySelection();
            /*! \brief Coller la selection courante*/
			void PasteSelection();
            /*! \brief Couper la selection courante*/
			void CutSelection();
            //-------------------------------------------------------------
            // Ajout composant
            //-------------------------------------------------------------
            /*! \brief Ajoute un component enfant au composant courant*/
			void AddEmptyChild();
            //-------------------------------------------------------------
            // AdminSetup
            //-------------------------------------------------------------
            /*! \brief Acces au setup du premier composant selectionné s'il en est capable*/
			void AdminSetup();
            //-------------------------------------------------------------
            // Execution
            //-------------------------------------------------------------
            /*! \brief Demarrage de l'execution*/
			void StartExecution();
            /*! \brief Arret de l'execution*/
			void StopExecution();
        };

    }
}

#endif // _SwEditorGIMenu_H
