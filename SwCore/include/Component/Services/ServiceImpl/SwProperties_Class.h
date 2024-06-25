/*!
\file SwProperties_Class.h
\date 12/04/2006
\brief implementation la liste des propriétés
\author  Big
\version 1.0
 */

#ifndef _SwProperties_Class_H
#define _SwProperties_Class_H
/*
  * INCLUDES GLOBAUX
 */
#include <QString>
#include <QMap>
#include <QSet>
#include <QList>
/*
  * INCLUDES LOCAUX
 */
#include "SwCoreConstantes.h"
#include "Tools/Exception/SwException.h"
#include "Main/SwMacros.h"
#include "Component/Services/ISwProperties.h"

namespace StreamWork
{
    namespace SwCore
    {
        class _SwPropertyImpl_Class;
        class ISwProperty;
        class ISwController;
		class SwProperties_ClassHelper;
        /*!
        \class ISwInterfaces_Provider
        \brief implementation du service fournisseur d'interfaces
        @ingroup SwCoreGrp
        */
        class BUILD_SWCORE SwProperties_Class : public ISwProperties {
        private:
            /*! \brief composant hote */
            SwComponent_Class * _host_component;
            /*! \brief QMap des propriétés */
            QMap<QString,_SwPropertyImpl_Class *> _map_properties;
            /*! \brief QList des propriétés */
            QList<ISwProperty *> _set_properties;
			bool _isEditable;

			// ------------------------------------------------------------------------
			//				Gestion des helpers
			// ------------------------------------------------------------------------							
			/*! @brief Set des helpers enregistrés */
			QSet<SwProperties_ClassHelper *> _helpers;
			/*! @brief enregistre un helper pour qu'il soit notifié de la destruction du service */
			void registerHelper(SwProperties_ClassHelper *);
			/*! @brief desenregistre un helper */
			void unregisterHelper(SwProperties_ClassHelper *);

        protected:
            /*! \brief signal de changement de la propriété */
            LibIndeSig::iSignal1<ISwProperties *> _OnBeforeChange;
            /*! \brief signal de changement de la propriété */
            LibIndeSig::iSignal1<ISwProperties *> _OnAfterChange;
            /*! \brief signal de creation d'une propriété */
            LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> _OnCreateProperty;
            /*! \brief signal de destruction d'une propriété */
            LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> _OnDestroyProperty;
	        /*! \brief Permet de crée les sous propriétés pour un type complexe */
	        void CreateSubProperties(_SwPropertyImpl_Class * parent_property);
	        /*! \brief Permet de detruire les sous propriétés pour un type complexe */
	        void DestroySubProperties(_SwPropertyImpl_Class * parent_property);
	        /*! \brief Permet de propager les caracterisques aux sous propriétés */
	        void PropagateFeaturesToSubProperties(_SwPropertyImpl_Class * parent_property);
	        /*! \brief Permet de detruire les controllers et de descativer l'edition des sous propriétés */
	        void UnAllowControlSubProperties(_SwPropertyImpl_Class * parent_property);
	        /*! \brief Permet d'activer l'edition des sous propriétés */
	        void AllowControlSubProperties(_SwPropertyImpl_Class * parent_property);
	        /*! \brief Sur changement de controle d'une propriété */
	        void OnPropertyControlChange(ISwProperty * property,ISwController *new_controller,ISwController *old_controller);
        public:
	        /*! \brief Construsteur */
	        SwProperties_Class(SwComponent_Class * host);
	        /*! \brief Destrusteur */
	        virtual ~SwProperties_Class();
	        /*! \brief Change la visibilite de la propriété */
	        void ChangePropertyVisibility(QString name,bool is_visible);
	        /*! \brief Change l'editabilite de la propriété */
	        void ChangePropertyEdition(QString name,bool is_editable);
	        /*! \brief Change l'aspect controllable de la propriété */
	        void ChangePropertyControllable(QString name,bool is_controllable);
 	        /*! \brief Permet d'indiquer qu'une property a changé */
	        void SignalPropertyChange(QString name);
           //---------------------------------------------------------------------
            // Interface ISwProperties
            //---------------------------------------------------------------------
	        /*! \brief Permet de crée une propriété */
	        ISwProperty * CreatePropertyWithType(int type_id,QString name);
	        /*! \brief Permet de crée une propriété issue d'un object Qt*/
	        ISwProperty * CreatePropertyForQObject(QObject * obj,QString name,QString prefix=QString());
	        /*! \brief Permet de crée toutes les propriétés issues d'un object Qt*/
	        void CreatePropertiesForQObject(QObject * obj,QString prefix=QString(),bool disable_objectName=false);
	        /*! \brief Permet de detruire une propriété*/
			void DestroyProperty(QString name);
			/*! \brief Permet de detruire les propriétés commencant par name */
			virtual void DestroyPropertiesBeginWith(QString name);
	        /*! \brief Permet d'acceder a une propriété*/
	        ISwProperty * GetProperty(QString name);
	        /*! \brief Permet d'acceder a la liste des propriétés*/
            QList<ISwProperty *> & GetProperties();
	        /*! \brief methode d'acces au signal avant changement*/
	        LibIndeSig::iSignal1<ISwProperties *> & GetOnBeforePropertiesChange();
	        /*! \brief methode d'acces au signal apres changement*/
	        LibIndeSig::iSignal1<ISwProperties *> & GetOnAfterPropertiesChange();
	        /*! \brief methode d'acces au signal de creation de propriété*/
	        LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnCreateProperty();
	        /*! \brief methode d'acces au signal de destruction de propriété*/
	        LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnDestroyProperty();
			/*! \brief Override l'editabilité de toutes les proprieté du service /!\ cette proprieté n'est pas sauvé et remise à false au début*/
			virtual void ChangePropertiesEdition(bool isEditable);
			/*! \brief Renvoi si le service du proprieté autorise l'edition de ses proprietés */
			virtual bool GetPropertiesEdition();
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief acces a son composant hote */
	        SwComponent_Class * GetHostComponent();
            //---------------------------------------------------------------------
            // Interface ISwHost
            //---------------------------------------------------------------------
	        /*! \brief methode permettant de charger des donnees */
	        void Load(QDomElement & elt,ISwFinalizerManager & finalizer_manager);
			/*! \brief methode permettant de sauver des donnees */
			void Save(QXmlStreamWriter& writer);

			friend class SwProperties_ClassHelper;
        };


		/**
		*	@class SwProperties_ClassHelper
		*	@brief Classe de base pour permettre d'etendre les fonctionalités de la SwProperties_Class (Cf PlgPropertiesHelper dans protolg3)
		*	Les composants instancient le SwProperties_Class, c'est pour ça qu'il n'est pas possible d'heriter de la SwProperties_Class
		*/
		class BUILD_SWCORE SwProperties_ClassHelper
		{
		public :
			/** @brief Construit l'helper et s'enregistre en tant qu'helper du service passé en parametre */
			SwProperties_ClassHelper(SwProperties_Class *);
			/** @brief detruit l'helper et se desenregistre en tant qu'helper du service de proprieté*/
			virtual ~SwProperties_ClassHelper();
			/** @brief renvoi true si le service de proprieté est setté */
			bool isValid();	
		protected :
			/** @brief Notifie le service de proprieté avant un changement*/
			void sendOnBeforeChange();
			/** @brief Notifie le service de proprieté aprés un changement*/
			void sendOnAfterChange();
			/** @brief Permet d'ajouter une proprieté au service de proprieté
			*	@param prefix : prefix de la proprieté
			*	@param name : nom sans le prefix de la proprieté
			*	@param property : proprieté à ajouter au service		*/
			void addProperty(QString prefix, QString name, _SwPropertyImpl_Class * property);
			/** @brief Permet d'acceder au service de proprieté 
			*   @return renvoi un pointeur sur le service de proprieté*/
			ISwProperties * getHost();
		private : 
			/** @brief service de proprieté sur lequel le helper agit */
			SwProperties_Class * _propertiesClass;

			void unValidate();
			void validate();

			friend class SwProperties_Class;
		};
    }
}

#endif
