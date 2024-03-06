/*!
 \file ISwEditorStreamNavigation.h
 \brief navigation dans un stream
 \date 02-octobre-2006 16:04:34
 \author F.Bighelli
*/

#ifndef _ISwEditorStreamNavigation_H
#define _ISwEditorStreamNavigation_H

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
		 \class ISwEditorStreamNavigation 
		 \brief navigation dans un stream
		*/
        class ISwEditorStreamNavigation 
		{   
		public:
            //-------------------------------------------------------------
            // Navigation dans l'arbre du stream
            //-------------------------------------------------------------
             /*! \brief Remonter au parent*/
			virtual void GoToParent()=0;
             /*! \brief Descendre sur un des enfants*/
			virtual void GoToChild(QString child_name)=0;
             /*! \brief Acces au composant courant*/
			virtual SwCore::SwComponent_Class * GetCurrentComponent()=0;
             /*! \brief Definition au composant courant*/
			virtual void SetCurrentComponent(SwCore::SwComponent_Class *)=0;
             /*! \brief Acces au path courant*/
			virtual QString GetCurrentPath()=0;
            //-------------------------------------------------------------
            // Observations
            //-------------------------------------------------------------
            /*! \brief Attachement d'un observateur sur cette interface*/
            virtual void AttachNavigationObserver(SwCore::ISwObserver * observer)=0;
            /*! \brief Detachement d'un observateur sur cette interface*/
            virtual void DetachNavigationObserver(SwCore::ISwObserver * observer)=0;
        };
	}
}
#endif 
