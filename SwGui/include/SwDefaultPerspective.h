/**
@file SwDefaultPerspective.h
@brief Perspective par default
@author F.Bighelli
 */

#ifndef _STREAMWORK_SWGUI_SWDEFAULTPERSPECTIVE_H
#define _STREAMWORK_SWGUI_SWDEFAULTPERSPECTIVE_H

#include "Component/Base/SwComponent_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Provider_Class.h"
#include "Component/Services/ServiceImpl/SwInterfaces_Consumer_Class.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"
#include "Properties/ISwProperty.h"
#include "ISwPerspective.h"
#include "ISwPerspectiveObserver.h"
#include "SwGuiConstantes.h"

namespace StreamWork {

    namespace SwGui {
    
        class _SwPerspectivesManager;
        /**
        @class SwDefaultPerspective
        @brief Perspective par default
        */
        class SWGUI_EXPORT SwDefaultPerspective : public SwCore::SwComponent_Class,
                                     public SwCore::ISwInterfaces_ConsumerObserver,
                                     public ISwPerspective{
            Q_OBJECT
            Q_PROPERTY(QString iconPath READ getIconPath WRITE setIconPath)
            Q_PROPERTY(QString label READ getLabel WRITE setLabel)
            Q_PROPERTY(QString description READ getDescription WRITE setDescription)
            Q_PROPERTY(bool visible READ isPerspectiveVisible WRITE setPerspectiveVisible)
        protected:
             /* service de fourniture d'interface */
            SwCore::SwInterfaces_Provider_Class * _provider_service;
            /* service de consommation d'interface */
            SwCore::SwInterfaces_Consumer_Class * _consumer_service;
            /* service de gestion des propriétés */
            SwCore::SwProperties_Class * _properties_service;
            /* iconPath */
            QString _iconPath;
            /* label */
            QString _label;
            /* description */
            QString _description;
            /* liste des observers */
            QList<ISwPerspectiveObserver *> _pObservers;
            /* visibilite */
            bool _visible;
        public:
	        /** @brief Constructor */
	        SwDefaultPerspective();
            /*! \brief Destructeur */
            virtual ~SwDefaultPerspective();
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
            /** @brief acces icon */
            virtual QString getPerspectiveIcon();
            /** @brief acces texte */
            virtual QString getPerspectiveLabel();
            /** @brief acces description */
            virtual QString getPerspectiveDescription();
            /** @brief Construction de la vue */
            virtual void buildPerspectiveView(QWidget * widget);
            /** @brief Destruction de la vue */
            virtual void destroyPerspectiveView(QWidget * widget);
            /** @brief Permet de savoir si la persective est active */
            virtual bool isPerspectiveEnabled();
            /** @brief Enregistrement de l'observer */
            virtual void registerISwPerspectiveObserver(ISwPerspectiveObserver * observer);
            /** @brief Desregistrement de l'observer */
            virtual void unregisterISwPerspectiveObserver(ISwPerspectiveObserver * observer);
            /** @brief sur changement de l'activation */
            virtual void OnEnablePerspectiveChange();
            /** @brief Permet de savoir si la persective est visible */
            virtual bool isPerspectiveVisible();
            /** @brief Permet de savoir si la persective est visible */
            virtual void setPerspectiveVisible(bool visibility);
            //---------------------------------------------------------------------
            // Interface getter and setter
            //---------------------------------------------------------------------
            /** @brief iconPath getter*/
            QString getIconPath();
            /** @brief iconPath getter*/
            void setIconPath(QString value);
            /** @brief label getter*/
            QString getLabel();
            /** @brief label getter*/
            void setLabel(QString value);
            /** @brief label getter*/
            QString getDescription();
            /** @brief description getter*/
            void setDescription(QString value);
        };

    }
    
}
#endif
