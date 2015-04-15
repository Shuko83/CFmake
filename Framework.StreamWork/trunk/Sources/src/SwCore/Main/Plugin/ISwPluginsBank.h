/*!
 \file ISwPluginsBank.h
 \brief Implementation of the Interface ISwPluginsBank
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef _ISwPluginsBank_H
#define _ISwPluginsBank_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QIcon>
#include <QAbstractItemModel>
/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "ISwPluginFactory.h"
#include "SwPluginFactory_Class.h"
#include "ISwCreationPostProcessor.h"
#include "SwUUID.h"
#include "ISwObjectFactory.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \interface ISwPluginsBank 
		 \brief Interface definissant l'acces a la banque de plugins
        @ingroup SwCoreGrp
        @swinterface
         Cette interfaces definit le mecanisme d'acces aux fabriques de composants et de données
         
         L'acces a cet interface se fait grace a la macro SW_FACTORIES

         Pour ajouter un path
         \code
         SW_FACTORIES.AddPath(my_path);
         \endcode

         Pour creer un composant
         \code
         SwComponent_Class * my_comp=SW_FACTORIES.CreateComponent(my_component_type_name);
         \endcode

         Pour creer une donnée
         \code
         SwData_Class * my_data=SW_FACTORIES.CreateData(my_data_type_id);
         \endcode
         ou mieux utiliser la macro SW_CREATE_DATA
         \code
         SwData_Class * my_data=SW_CREATE_DATA(my_data_type_id);
         \endcode
         
         On peut ajouter un observateur qui est alerté lors de la création d’un nouveau composant.
         L’enregistrement de ce type d’observer se fait via la méthode AddCreationPostProcessor et
         le desenregistrement via RemoveCreationPostProcessor.

		*/
		class ISwPluginsBank
		{   
		public:
            /*! \brief Ajouter un path */
            virtual void AddPath(QString path,bool registerable=false)=0;
            /*! \brief Ajouter un descripteur de paths */
            virtual void AddPaths(QString pathsdescriptor)=0;
            /*! \brief Acces a liste des paths */
            virtual QMap<QString,bool> GetPathList()=0;
            /*! \brief Acces a liste des plugins usine par path */
            virtual QSet<ISwPluginFactory *> & GetPluginList(QString path) throw(SwException)=0;
            /*! \brief Acces a liste de tous les plugins */
            virtual QMap<QString,SwPluginFactory_Class *> * GetAllPlugins()=0;
            /*! \brief Acces a la liste des noms de tous les composants */
            virtual const QSet<QString> GetComponentsList()=0;
            /*! \brief Acces a la liste des noms de tous les composants controllers relatif a un type donné*/
            virtual QList<QString> GetControllersListForType(int type_identifier)=0;
            /*! \brief Acces a la description d'un composant */
            virtual QString GetComponentDescription(QString component_name) throw(SwException)=0;
            /*! \brief Acces a l'icone d'un composant */
            virtual QIcon GetComponentIcon(QString component_name) throw(SwException)=0;
            /*! \brief Creation d'un composant a partir de son nom*/
            virtual SwComponent_Class * CreateComponent(QString component_name) throw(SwException)=0;
            /*! \brief Relire le contenu d'un plugin (pour les plugins dont le contenu a changer)*/
            virtual void RereadPluginContent(SwPluginFactory_Class * plugin) throw(SwException)=0;
            /*! \brief Acces au modèle pour l'affichage*/
            virtual QAbstractItemModel * GetModel()=0;
			/*! \brief Demande au model de se reconstruire pour optimiser l'affichage*/
			virtual void RebuildModel() = 0;
            /*! \brief Acces a la liste des noms de tous les composants */
            virtual QSet<SwUUID> GetDataList()=0;
            /*! \brief Acces au nom du type d'une donnée definie par son id */
            virtual QString GetDataTypeNameFromDataTypeId(const SwUUID &)=0;
            /*! \brief Creation d'une data */
            virtual SwData_Class * CreateData(const SwUUID &) throw(SwException)=0;
            /*! \brief Creation d'un objet */
            virtual void * CreateObject(const SwUUID &,void *) throw(SwException)=0;
            /*! \brief Enregsitrement d'une fabrique d' objet */
            virtual void RegisterObjectFactory(const SwUUID &,ISwObjectFactory *) throw(SwException)=0;
            /*! \brief Enregsitrement d'une fabrique d' objet */
            virtual void UnregisterObjectFactory(const SwUUID &) throw(SwException)=0;
            //----------------------------------------------------
            //-- Gestion des Creations Post Processor
            //----------------------------------------------------
            /*! \brief Ajoute un post processeur*/
            virtual void AddCreationPostProcessor(ISwCreationPostProcessor *)=0;
            /*! \brief Supprime un post processeur*/
            virtual void RemoveCreationPostProcessor(ISwCreationPostProcessor *)=0;

		};
	}
}
#endif 
