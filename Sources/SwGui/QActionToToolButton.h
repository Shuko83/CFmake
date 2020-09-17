/*
\file SwActionToToolButton.h
\brief
\version 1.0
\author EPO
\date 09/01/2015
*/

#ifndef SWGUI_QACTIONTOTOOLBUTTON_H
#define SWGUI_QACTIONTOTOOLBUTTON_H

/*
* INCLUDES LOCAUX
*/
#include "SwAssistedComponent.h"
#include <QToolButton>

namespace StreamWork {
    namespace SwGui {
        /**
        *   @class SwActionToToolButton
        *   @brief
        */
        class QActionToToolButton : public StreamWork::SwFoundation::SwAssistedComponent
        {
            Q_OBJECT
        public:
            /*! \brief Constructeur */
			QActionToToolButton();
            
            /*! \brief Destructeur */
            virtual ~QActionToToolButton();
            
            /**
            * @brief    : Initialisation du composant
            * @note  : A surcharger
            */
            virtual void initializeComponent() throw(StreamWork::SwCore::SwException );
            
            //----------------------------------------------------
            // Interface de consomation d'interfaces
            //----------------------------------------------------
            
            /**
            * @brief    : Callback appel?e lors de la disponibilit? de l'interface
            * @param     : QString interfaceName - Nom de l'interface
            */
            virtual void interfaceAvailable( QString interface_name );
            
            /**
            * @brief    : Callback appel?e lors de l'indisponibilit? de l'interface
            * @param     : QString interfaceName - Nom de l'interface
            */
            virtual void interfaceUnavailable( QString interface_name );
            
        protected slots:
            /**
            * @brief    : Update visibility's ToolButton
            */
            void updateVisibilityAction();
            
        private:
            QToolButton*  _toolButton;
        };
    }
}

#endif
