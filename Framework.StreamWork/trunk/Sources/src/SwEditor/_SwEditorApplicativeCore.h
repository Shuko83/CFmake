/*!
 \file _SwEditorApplicativeCore.h
 \brief Coeur de traitement de l'editeur
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwEditorApplicativeCore_H
#define __SwEditorApplicativeCore_H

/*
  * INCLUDES GLOBAUX
  */
#include <QSet>
#include <QList>
/*
  * INCLUDES LOCAUX
  */
#include <SwComponent_Class.h>
#include <SwInterfaces_Provider_Class.h>
#include <ISwComponentProvider.h>
#include <ISwCreationPostProcessor.h>
#include "ISwEditorStreamsManager.h"
#include "ISwEditorStreamOperations.h"
#include "_SwEditorNavigator_Class.h"
#include "_SwEditorSelection_Class.h"
#include "_SwEditorStreamTreeModel.h"
#include "_SwEditorInterfacesLayerManager.h"
#include "_SwEditorPinsLayerManager.h"

namespace StreamWork
{
	namespace SwEditor
	{

        /*!
	        \class _SwEditorApplicativeCore
	        \brief Coeur de traitement de l'editeur
        */
        class _SwEditorApplicativeCore : public SwCore::SwComponent_Class, public ISwEditorStreamsManager, public SwCore::ISwCreationPostProcessor, public SwCore::ISwComponentProvider
        {
        protected:
            /* \brief classe permanente de selection (decorateur)*/
            class _InternalSelection: public ISwEditorStreamOperations, public SwCore::ISwObserver {
                /* \brief Selection courante */
                ISwEditorStreamOperations * _selection;
                /* \brief liste des observers*/
                QSet<SwCore::ISwObserver *> _observers;
                /* \brief applicative core*/
                _SwEditorApplicativeCore * _p_core;
            public:
                /*! \brief Constructeur */
                _InternalSelection(_SwEditorApplicativeCore * p_core);
                /*! \brief Destructeur */
                virtual ~_InternalSelection();
                /*! \brief Definition du navigateur */
                void DefineSelection(ISwEditorStreamOperations * selection);
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
               //-------------------------------------------------------------
                // ISwObserver
                //-------------------------------------------------------------
                /*! \brief methode appelée par l'observable*/
	            virtual void Update();
            };
            /* \brief classe permanente de navigation (decorateur)*/
            class _InternalNavigator: public ISwEditorStreamNavigation, public SwCore::ISwObserver {
                /* \brief Navigateur courant */
                ISwEditorStreamNavigation * _navigator;
                /* \brief liste des observers*/
                QSet<SwCore::ISwObserver *> _observers;
                /* \brief Acces a la selection permanente */
                _InternalSelection * _h_selection;
            public:
                /*! \brief Constructeur */
                _InternalNavigator();
                /*! \brief Destructeur */
                virtual ~_InternalNavigator();
                /*! \brief Definition de la selection permanente */
                void DefinePermanentSelection(_InternalSelection * selection);
                /*! \brief Definition du navigateur */
                void DefineNavigator(ISwEditorStreamNavigation * navigator);

                //-------------------------------------------------------------
                // Navigation dans l'arbre du stream
                //-------------------------------------------------------------
                /*! \brief Remonter au parent*/
			    virtual void GoToParent();
                /*! \brief Descendre sur un des enfants*/
			    virtual void GoToChild(QString child_name);
                /*! \brief Acces au composant courant*/
			    virtual SwCore::SwComponent_Class * GetCurrentComponent();
                /*! \brief Definition au composant courant*/
			    virtual void SetCurrentComponent(SwCore::SwComponent_Class * component);
                /*! \brief Acces au path courant*/
			    virtual QString GetCurrentPath();
                //-------------------------------------------------------------
                // Observations
                //-------------------------------------------------------------
                /*! \brief Attachement d'un observateur sur cette interface*/
                virtual void AttachNavigationObserver(SwCore::ISwObserver * observer);
                /*! \brief Detachement d'un observateur sur cette interface*/
                virtual void DetachNavigationObserver(SwCore::ISwObserver * observer);
                /*! \brief Signale un changement aux observers*/
	            void SignalChangeToObservers();
                //-------------------------------------------------------------
                // ISwObserver
                //-------------------------------------------------------------
                /*! \brief methode appelée par l'observable*/
	            virtual void Update();
            };
        protected:
            /* \brief service de fourniture d'interface */
            SwCore::SwInterfaces_Provider_Class * _provider_service;
            /* \brief liste des observers*/
            QSet<SwCore::ISwObserver *> _observers;
            /* \brief liste des streams*/
            QList<SwCore::SwComponent_Class *> _streams;
            /* \brief index du stream courant*/
            int _current_stream_index;
            /* \brief navigateur permanent*/
            _InternalNavigator _current_navigator;
            /* \brief selection permanente*/
            _InternalSelection _current_selection;
            /* \brief modele*/
            _SwEditorStreamTreeModel * _model;
            /* \brief active la creation d'un service scene*/
            bool _create_graphic_scene;
            /* \brief manager d'affichage des interfaces*/
            _SwEditorInterfacesLayerManager * _il_manager;
            /* \brief manager d'affichage des pins*/
            _SwEditorPinsLayerManager * _pl_manager;
        protected:
            /*! \brief Signale un changement aux observers*/
	        void SignalChangeToObservers();
            /*! \brief Ouverture interne d'un stream*/
	        void InternalOpenStream(QString file_name);
            /*! \brief Sauvegarde interne d'un stream*/
	        void InternalSaveStream(QString file_name);
            /*! \brief Definition du navigateur courant*/
            void DefineComponentAsCurrentNavigator(SwCore::SwComponent_Class * root_component);
        public:
            /*! \brief Constructeur */
            _SwEditorApplicativeCore();
            /*! \brief Destructeur */
            virtual ~_SwEditorApplicativeCore();

            /*! \brief Initialisation des ressources
                \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwCore::SwException);
            /*! \brief Enregistrement du postprocessing suite a creation de composant*/
            void RegisterPostProcessing();
            /*! \brief Enregistrement du postprocessing suite a creation de composant*/
            void UnregisterPostProcessing();
            /*! \brief register graphic scene service*/
            void RegisterGraphicSceneService(SwCore::SwComponent_Class *h_comp);
            /*! \brief Forcer le reset du model*/
            void ForceModelReset();
            //-------------------------------------------------------------
            // Principales operations de gestion de stream
            //-------------------------------------------------------------
            /*! \brief Quitter l'application*/
			virtual void QuitEditor();
            /*! \brief Crée un nouveau stream vide*/
	        virtual void NewStream();
            /*! \brief Ouvrir un stream existant*/
	        virtual void OpenStream();
            /*! \brief Sauvegarde du stream courant */
	        virtual void SaveStream();
            /*! \brief Sauvegarde du stream courant sous un nom donné*/
	        virtual void SaveStreamAs();
            /*! \brief Fermeture du stream courant*/
	        virtual void CloseStream();
            //-------------------------------------------------------------
            // Principales operations de navigation au travers des streams
            //-------------------------------------------------------------
            /*! \brief Renvoie le nombre de streams ouverts*/
	        virtual int GetStreamsCount();
            /*! \brief Renvoie l'information d'un stream*/
	        virtual QVariant GetStreamDescription(int index,int role = Qt::DisplayRole);
            /*! \brief Renvoie le stream courant*/
	        virtual int GetCurrentStream();
            /*! \brief Renvoie la racine du stream courant*/
            virtual SwCore::SwComponent_Class * GetCurrentStreamRoot();
            /*! \brief Definition du stream courant*/
	        virtual void SetCurrentStream(int index);
            //-------------------------------------------------------------
            // Acces a la scene QT du stream courant
            //-------------------------------------------------------------
            /*! \brief Renvoie la scene graphique relative au stream courant courante*/
			virtual ISwEditorGraphicScene * GetCurrentStreamGraphicScene();
            //-------------------------------------------------------------
            // Observations
            //-------------------------------------------------------------
            /*! \brief Attachement d'un observateur sur cette interface*/
            virtual void AttachStreamManagerObserver(SwCore::ISwObserver * observer);
            /*! \brief Detachement d'un observateur sur cette interface*/
            virtual void DetachStreamManagerObserver(SwCore::ISwObserver * observer);
            //-------------------------------------------------------------
            // interface ISwCreationPostProcessor
            //-------------------------------------------------------------
	        /*! \brief methode appelée après la creation d'un composant
            \param[in] h_comp handle sur le composant qui vient d'etre créé*/
	        void AfterCreatePostProcess(SwComponent_Class *h_comp);
            //-------------------------------------------------------------
            // interface ISwComponentProvider
            //-------------------------------------------------------------
	        /*! \brief Fournit un composant */
	        SwComponent_Class * ProvideComponent();

        };
    }
}
#endif
