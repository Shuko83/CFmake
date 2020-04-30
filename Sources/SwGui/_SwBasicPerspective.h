/**
@file _SwBasicPerspective.h
@brief Perspective basique
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI__SWBASICPERSPECTIVE_H
#define _STREAMWORK_SWGUI__SWBASICPERSPECTIVE_H

#include <QtWidgets>

#include "SwDefaultPerspective.h"
#include <SwProperties_Class.h>
#include <ISwProperty.h>

using namespace StreamWork::SwCore;

namespace StreamWork {

    namespace SwGui {
    
        /**
        @class _SwBasicPerspective
        @brief Perspective basique
        */
        class _SwBasicPerspective : public SwDefaultPerspective {
            Q_OBJECT
        private:
            /** @brief header */
            QWidget * _wHeader;
            /** @brief part high */
			QWidget * _wHighPart;
            /** @brief part low */
			QWidget * _wLowPart;
            /** @brief footer */
			QWidget * _wFooter;
            /** @brief splitter du milieu */
            QSplitter * _splitter;
            /** @brief box layout */
            QVBoxLayout * _layout;
            /** @brief title */
            QFrame * _title;
            /** @brief mtitle */
            QWidget * _mtitle;
            /** @brief enabledPerspective */
            bool _enabledPerspective;
            /** @brief enabledPerspective */
            bool _isCurrent;
			/* @brief propriété style sheet du titre*/
			ISwProperty * _title_stylesheet_property;
			/* @brief stylesheet utilise */
			QString	_title_stylesheet;
        public:
	        /** @brief Constructor */
	        _SwBasicPerspective();
            /*! \brief Destructeur */
            virtual ~_SwBasicPerspective();
            /*! \brief Initialisation des ressources
            \note tous les services du composants doivent être déclarés dans cette methodes*/
            virtual void InitializeResources() throw(SwCore::SwException);
            //---------------------------------------------------------------------
            // Interface ISwInterfaces_ConsumerObserver
            //---------------------------------------------------------------------
	        /*! \brief Avant changement de la disponibilité de l'interface */
	        virtual void BeforeInterfaceAvailabilityChange(QString interface_name,SwCore::SwComponent_Class * provider_host);            
	        /*! \brief Apres changement de la disponibilité de l'interface */
	        virtual void AfterInterfaceAvailabilityChange(QString interface_name,SwCore::SwComponent_Class * provider_host);            
            //---------------------------------------------------------------------
            // Interface ISwPerspective
            //---------------------------------------------------------------------
            /** @brief Construction de la vue */
            virtual void buildPerspectiveView(QWidget * widget);
            /** @brief Destruction de la vue */
            virtual void destroyPerspectiveView(QWidget * widget);
            /** @brief Permet de savoir si la persective est active */
            virtual bool isPerspectiveEnabled();
			/** @brief changement de proprietes */
			void OnPropertyChange(ISwProperty * property);
        };
    }
}

#endif
