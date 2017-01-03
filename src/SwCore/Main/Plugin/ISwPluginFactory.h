/*!
 \file ISwPluginFactory.h
 \brief Implementation of the Interface ISwPluginFactory
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef _ISwPluginFactory_H
#define _ISwPluginFactory_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QIcon>
#include <QSet>
#include <QAbstractListModel>
#include <typeinfo>
/*
  * INCLUDES LOCAUX
  */
#include "SwComponent_Class.h"
#include "ISwService.h"
#include "SwData_Class.h"
#include "SwUUID.h"

//Nom du service
#define CG_SW_SERVICE_PLUGIN_FACTORY "PluginFactory"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \interface ISwPluginFactory
		 \brief Interface definissant l'acces a un plugin usine
        @ingroup SwCoreGrp
        @swinterface
		*/
        class ISwPluginFactory : public ISwService
		{
		public:
            /*! \brief Acces au path du plugin */
            virtual QString GetPath()=0;
            /*! \brief Acces au nom du plugin */
            virtual QString GetPluginName()=0;
            /*! \brief Acces a la version du plugin */
            virtual QString GetPluginVersion()=0;
			/*! \brief Acces a la date de compilation du plugin depuis le 1er janvier 1970*/
			virtual double GetPluginCompilationDate()=0;
            //---------------------------------------------------------------------
            // Acces fabrique de composants
            //---------------------------------------------------------------------
            /*! \brief Acces a la liste des noms de tous les composants */
            virtual QSet<QString> GetComponentsList()=0;
            /*! \brief Acces a la description d'un composant */
            virtual QString GetComponentDescription(QString component_name)  throw(SwException) =0;
            /*! \brief Acces a l'icone d'un composant */
            virtual QIcon GetComponentIcon(QString component_name) const throw(SwException) =0;
            /*! \brief Creation d'un composant */
            virtual SwComponent_Class * CreateComponent(QString component_name)  throw(SwException) =0;
            /*! \brief Acces au modèle liste */
            virtual QAbstractListModel * GetModelList()=0;
            //---------------------------------------------------------------------
            // Acces fabrique de données
            //---------------------------------------------------------------------
            /*! \brief Acces a la liste des noms de tous les composants */
            virtual QSet<SwUUID> GetDataList()=0;
            /*! \brief Acces au nom du type d'une donnée definie par son id */
            virtual QString GetDataTypeNameFromDataTypeId(const SwUUID &)=0;
            /*! \brief Creation d'une data */
            virtual SwData_Class * CreateData(const SwUUID &) throw(SwException)=0;
            //---------------------------------------------------------------------
            // Interface ISwService
            //---------------------------------------------------------------------
            /*! \brief Est appele uniquement par le service manager aupres duquel le service est enregistré
            lorsque ce premier se detruit ou une operation de desenregistrement du service est réalisée*/
            void Liberate(){/*Rien a faire*/}
            /*! \brief Renvoie le nom du service
            \return le nom du service */
            QString GetServiceName(){return QString(CG_SW_SERVICE_PLUGIN_FACTORY); }
            /*! \brief Renvoie le nom du service reel (le nom de l'interface)
            \return le nom du service reel (le nom de l'interface)
            \note au niveau de l'interface (pas d'une de ses classes derivées) renvoyer typeid(this).name()*/
            QString GetServiceRealName() {return QString(typeid(this).name());}
		};
	}
}
#endif
