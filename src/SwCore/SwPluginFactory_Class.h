/*!
 \file SwPluginFactory_Class.h
 \brief Implementation of class SwPluginFactory_Class
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli

 \note
    Toute instance de SwPluginFactory_Class est crée via la methode exporté de la librairie dynamique
 \code   
    extern "C" Q_DECL_EXPORT SwPluginFactory_Class *  GetPluginInterface();
 \endcode
 \note
    Une fois l'instance crée la vie du plugin usine ressemble a ceci
 \code
    //Initialisation
    my_plugin_factory->SetPath(path);
    my_plugin_factory->SetPluginName(name);
    my_plugin_factory->Initialize();
    //Exploitation (utilisation de toute les autres methodes du plugin usine)
    //...
    //Liberation
    my_plugin_factory->Liberate();
    delete my_plugin_factory

 \endcode
*/

#ifndef _SwPluginFactory_Class_H
#define _SwPluginFactory_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QMap>
#include <QMultiMap>
/*
  * INCLUDES LOCAUX
  */
#include "ISwPluginFactory.h"
#include "ISwPluginOverview.h"
#include "ISwServicesManager_Listener.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwPluginFactory_Class 
		 \brief classe definissant un plugin usine
        @ingroup SwCoreGrp
		*/
        class BUILD_SWCORE SwPluginFactory_Class: public QAbstractListModel, 
                                                  public ISwPluginFactory,
                                                  public ISwServicesManager_Listener
		{  
            Q_OBJECT
        private:
            /** @brief class overview */
            class Overview : public ISwPluginOverview {
            public:
                //Parent
                ISwPluginFactory * _parent;
                /*! \brief Constructeur */
                Overview(ISwPluginFactory * parent) {_parent=parent;};
                //---------------------------------------------------------------------
                // Interface ISwPluginFactory
                //---------------------------------------------------------------------
                /*! \brief Acces au path du plugin */
                QString GetPath() {return _parent-> GetPath();}
                /*! \brief Acces au nom du plugin */
                QString GetPluginName() {return _parent-> GetPluginName();}
                /*! \brief Acces a la version du plugin */
                virtual QString GetPluginVersion() {return _parent-> GetPluginVersion();}
            };
        private:
            /*! \brief path */
            QString _path;
            /*! \brief name */
            QString _name;
            /*! \brief QMap des composants et de leur descriptions */
            QMap<QString,QString> _component_description;
            /*! \brief QMap des data et de leur types */
            QMap<SwUUID,QString> _data_type;
            /*! \brief QMultiMap des controlleurs clef type */
            QMultiMap<int,QString> _controllers;
            /*! \brief Set des coimposants controllers */
            QSet<QString> _controllers_list;
            /*! \brief Overview */
            Overview overview;
        protected:
            /*! \brief enregistrement d'un composant */
            void RegisterComponent(QString name,QString description);
            /*! \brief enregistrement d'un controller */
            void RegisterController(int type_id,QString component_name);
            /*! \brief enregistrement d'une data */
            void RegisterData(const SwUUID & type_id,QString type_name);
            /*! \brief instanciation d'un composant */
            virtual SwComponent_Class * CreateInstanceOf(QString name);
            /*! \brief instanciation d'une data */
            virtual SwData_Class * CreateInstanceOf(const SwUUID & type_id);
            /*! \brief acces a l'icone d'un composant */
            virtual QIcon CreateIconOf(QString name) const;
		public:
            /*! \brief Constructeur */
            SwPluginFactory_Class();
            /*! \brief Destructeur */
            virtual ~SwPluginFactory_Class();
            /*! \brief Definition path du plugin */
            void SetPath(QString path);
            /*! \brief Definition du nom du plugin */
            void SetPluginName(QString plugin_name);
            /*! \brief Initialisation */
            virtual void Initialize();
            /*! \brief finalisation de l'initialisation DECLARER LES SERVICES GLOBAUX ICI*/
            virtual void FinalizeInitialisation();
            /*! \brief Liberation */
            virtual void Liberate();
            //---------------------------------------------------------------------
            // Interface ISwPluginFactory
            //---------------------------------------------------------------------
            /*! \brief Acces au path du plugin */
            QString GetPath();
            /*! \brief Acces au nom du plugin */
            QString GetPluginName();
            /*! \brief Acces a la version du plugin */
            virtual QString GetPluginVersion();
            /*! \brief Acces a la liste des noms de tous les composants */
            QSet<QString> GetComponentsList();
            /*! \brief Acces a la description d'un composant */
            QString GetComponentDescription(QString component_name)  throw(SwException);
            /*! \brief Acces a l'icone d'un composant */
            QIcon GetComponentIcon(QString component_name) const throw(SwException);
            /*! \brief Creation d'un composant */
            SwComponent_Class * CreateComponent(QString component_name)  throw(SwException);
            /*! \brief Acces au modèle liste */
            QAbstractListModel * GetModelList();
            /*! \brief Acces a la liste des noms de tous les composants */
            QSet<SwUUID> GetDataList();
            /*! \brief Acces a la multimap des controlleyr */
            QMultiMap<int,QString> & GetControllersMap();
            /*! \brief Acces au nom du type d'une donnée definie par son id */
            QString GetDataTypeNameFromDataTypeId(const SwUUID & id);
            /*! \brief Creation d'une data */
            SwData_Class * CreateData(const SwUUID & id) throw(SwException);
            //---------------------------------------------------------------------
            // Interface ISwServicesManager_Listener
            //---------------------------------------------------------------------
	        /*! \brief sur ajout d'un service */
	        virtual void OnRegisterService(ISwService * service);            
	        /*! \brief sur suppression d'une  interface */
	        virtual void OnUnregisterService(ISwService * service);            
            //---------------------------------------------------------------------
            // Interface QAbstractListModel
            //---------------------------------------------------------------------
            /*! \brief Flags du modele*/
            Qt::ItemFlags flags(const QModelIndex &index) const;
            /*! \brief list mimeTypes */
            QStringList mimeTypes() const;
            /*! \brief acces a la mimeData correspondantes */
            QMimeData * mimeData(const QModelIndexList &indexes) const;
            /*! \brief Renvoie le nombre de lignes */
            int rowCount(const QModelIndex &parent = QModelIndex()) const;
            /*! \brief Renvoie la donnée correspondante*/
            QVariant data(const QModelIndex &index, int role) const;
        };
        /*! \brief type definissant la methode d'acces au plugin */
        typedef SwPluginFactory_Class * (* Tf_getPluginEntry)();

	}
}
#endif 
