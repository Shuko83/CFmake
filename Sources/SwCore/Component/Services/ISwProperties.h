/*!
\file ISwProperties.h
\date 12/04/2006
\brief service propriétés
\author  Big
\version 1.0
 */

#ifndef _ISwProperties_H
#define _ISwProperties_H
/*
  * INCLUDES GLOBAUX
 */
#include <QList>
#include <QString>
#include <QMetaType>
#include <QObject>
/*
  * INCLUDES LOCAUX
 */
#include "Main/Services/Management/ISwService.h"
#include "Component/Interfaces/ISwHost.h"
#include "Component/Interfaces/ISwPersistent.h"
#include "Tools/Signal/LibIndeSig.h"

//Nom du service
#define CG_SW_SERVICE_PROPERTIES "Properties"

namespace StreamWork
{
    namespace SwCore
    {
        class ISwProperty;
        /*!
        \interface ISwProperties
        \brief service propriétés
        @ingroup SwCoreGrp
        @swinterface
        */
        class ISwProperties: public ISwService, public ISwHost, public ISwPersistent {
        public:
	        /*! \brief Permet de crée une propriété */
	        virtual ISwProperty * CreatePropertyWithType(int type_id,QString name)=0;
	        /*! \brief Attache un fournisseur a une interface donnée */
            template<typename T> inline ISwProperty * CreateProperty(QString name){
                int type_id;
                type_id=qMetaTypeId<T>();
                return CreatePropertyWithType(type_id,name);
            }
	        /*! \brief Permet de crée une propriété issue d'un object Qt*/
	        virtual ISwProperty * CreatePropertyForQObject(QObject * obj,QString name,QString prefix=QString())=0;
	        /*! \brief Permet de créer toutes les propriétés issues d'un object Qt*/
			virtual void CreatePropertiesForQObject(QObject * obj, QString prefix = QString(), bool disable_objectName = false) = 0;
	        /*! \brief Permet de detruire une propriété*/
			virtual void DestroyProperty(QString name) = 0;
			/*! \brief Permet de detruire les propriétés commencant par name */
			virtual void DestroyPropertiesBeginWith(QString name) = 0;
	        /*! \brief Permet d'acceder a une propriété*/
	        virtual ISwProperty * GetProperty(QString name)=0;
	        /*! \brief Permet d'acceder a la liste des propriétés*/
            virtual QList<ISwProperty *> & GetProperties()=0;
	        /*! \brief methode d'acces au signal avant changement*/
	        virtual LibIndeSig::iSignal1<ISwProperties *> & GetOnBeforePropertiesChange()=0;
	        /*! \brief methode d'acces au signal apres changement*/
	        virtual LibIndeSig::iSignal1<ISwProperties *> & GetOnAfterPropertiesChange()=0;
	        /*! \brief methode d'acces au signal de creation de propriété*/
	        virtual LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnCreateProperty()=0;
	        /*! \brief methode d'acces au signal de destruction de propriété*/
	        virtual LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnDestroyProperty()=0;
	        /*! \brief Change la visibilite de la propriété */
	        virtual void ChangePropertyVisibility(QString name,bool is_visible)=0;
	        /*! \brief Change l'editabilite de la propriété */
	        virtual void ChangePropertyEdition(QString name,bool is_editable)=0;
	        /*! \brief Change l'aspect controllable de la propriété */
	        virtual void ChangePropertyControllable(QString name,bool is_controllable)=0;
	        /*! \brief Permet d'indiquer qu'une property a changé */
	        virtual void SignalPropertyChange(QString name)=0;
			/*! \brief Override l'editabilité de toutes les proprieté du service */
			virtual void ChangePropertiesEdition(bool isEditable)=0;
			/*! \brief Renvoi si le service du proprieté autorise l'edition de ses proprietés */
			virtual bool GetPropertiesEdition()=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PROPERTIES); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(*this).name()*/
            QString GetServiceRealName() {return QString(typeid(*this).name());}
        };
    }
}

#endif
