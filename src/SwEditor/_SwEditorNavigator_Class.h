/*!
\file _SwEditorNavigator_Class.h
\date 12/04/2006
\brief implementation de la navigation
\author  Big
\version 1.0
 */

#ifndef __SwEditorNavigator_Class_H
#define __SwEditorNavigator_Class_H
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
#include "ISwEditorStreamNavigation.h"

//Nom du service
#define CG_SW_SERVICE_NAVIGATOR "_SwEditorNavigator"

namespace StreamWork
{
    namespace SwEditor
    {
        /*!
        \class _SwEditorNavigator_Class
        \brief implementation de la navigation
        */
        class _SwEditorNavigator_Class : public SwCore::ISwService, public ISwEditorStreamNavigation {
        private:
            /*! \brief composant hote */
            SwCore::SwComponent_Class * _host_component;
            /*! \brief composant selection */
            SwCore::SwComponent_Class * _current_selection;
            /* \brief liste des observers*/
            QSet<SwCore::ISwObserver *> _observers;
        public:
	        /*! \brief Construsteur */
	        _SwEditorNavigator_Class(SwCore::SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~_SwEditorNavigator_Class();
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
				\return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_NAVIGATOR); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
				\return le nom du service reel (le nom de l'interface)
				\note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
				lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();
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
        };
    }
}

#endif
