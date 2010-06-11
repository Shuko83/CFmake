/*!
\file ISwProperties.h
\date 12/04/2006
\brief service propri�t�s
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
#include "ISwService.h"
#include "ISwHost.h"
#include "ISwPersistent.h"
#include "LibIndeSig.h"

//Nom du service
#define CG_SW_SERVICE_PROPERTIES "Properties"

namespace StreamWork
{
    namespace SwCore
    {
        class ISwProperty;
        /*!
        \class ISwProperties
        \brief service service propri�t�s
        */
        class ISwProperties: public ISwService, public ISwHost, public ISwPersistent {
        public:
	        /*! \brief Permet de cr�e une propri�t� */
	        virtual ISwProperty * CreatePropertyWithType(int type_id,QString name)=0;
	        /*! \brief Attache un fournisseur a une interface donn�e */
            template<typename T> inline ISwProperty * CreateProperty(QString name){
                int type_id;
                type_id=qMetaTypeId<T>();
                return CreatePropertyWithType(type_id,name);
            }
	        /*! \brief Permet de cr�e une propri�t� issue d'un object Qt*/
	        virtual ISwProperty * CreatePropertyForQObject(QObject * obj,QString name,QString prefix=QString())=0;
	        /*! \brief Permet de cr�e toutes les propri�t�s issues d'un object Qt*/
	        virtual void CreatePropertiesForQObject(QObject * obj,QString prefix=QString(),bool disable_objectName=false)=0;
	        /*! \brief Permet de detruire une propri�t�*/
	        virtual void DestroyProperty(QString name)=0;
	        /*! \brief Permet d'acceder a une propri�t�*/
	        virtual ISwProperty * GetProperty(QString name)=0;
	        /*! \brief Permet d'acceder a la liste des propri�t�s*/
            virtual QList<ISwProperty *> & GetProperties()=0;
	        /*! \brief methode d'acces au signal avant changement*/
	        virtual LibIndeSig::iSignal1<ISwProperties *> & GetOnBeforePropertiesChange()=0;
	        /*! \brief methode d'acces au signal apres changement*/
	        virtual LibIndeSig::iSignal1<ISwProperties *> & GetOnAfterPropertiesChange()=0;
	        /*! \brief methode d'acces au signal de creation de propri�t�*/
	        virtual LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnCreateProperty()=0;
	        /*! \brief methode d'acces au signal de destruction de propri�t�*/
	        virtual LibIndeSig::iSignal2<ISwProperties *,ISwProperty *> & GetOnDestroyProperty()=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PROPERTIES); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes deriv�es) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}
        };
    }
}

#endif
