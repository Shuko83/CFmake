/*!
 \file _SwPluginsBank_Class.h
 \brief Implementation of the class _SwPluginsBank_Class
 \date 23-août-2006 16:04:34
 \version 1.0
 \author F.Bighelli
*/

#ifndef __SwPluginsBank_Class_H
#define __SwPluginsBank_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QString>
#include <QFile>
#include <QMultiMap>
#include <QHash>
#include <QSystemTrayIcon>
/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "ISwPluginsBank.h"
#include "SwPluginFactory_Class.h"
#include "_SwTreeItem.h"

class ProductLicense;

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class _SwPluginsBank_Class 
		 \brief Implementation de la banque de plugins
		*/
        class _SwPluginsBank_Class: public QAbstractItemModel, public ISwPluginsBank
		{
            Q_OBJECT
        private:
            /*! \brief type interne liste de plugins*/
            typedef QSet<ISwPluginFactory *> TL_plugins;
            typedef QSet<ISwPluginFactory *>::const_iterator TL_pluginsIt;
            /*! \brief QMap des paths et s'il doivent être enregistrée*/
            QMap<QString,bool> _paths;    
            /*! \brief Liste plugins par path */
            QMap<QString,TL_plugins> _plugins_paths;    
            /*! \brief Liste plugin par nom de plugin */
            QMap<QString,SwPluginFactory_Class *> _plugin_by_name;    
            /*! \brief QMap d'une data vers son plugin */
            QMap<SwUUID,SwPluginFactory_Class *> _data_to_factory;
            /*! \brief QMultiMap des controlleurs clef type */
            QMultiMap< int, QPair<QString,QString> > _controllers;
			/*! \Enregistrement des fabriques d'objet */
			QHash<SwUUID,ISwObjectFactory *> object_factory_map;
            /*! \brief arbre pour le modele */
            _SwTreeItem * _tree_items;
            /*! \brief indicateur de changement*/
            bool _has_changed;
            /*! \brief liste des post_processors */
            QSet<ISwCreationPostProcessor *> _post_processors;
            /*! \brief liste des fichiers mis a jour */
            QStringList _updatedFiles;
            /*! \brief trayIcon */
            QSystemTrayIcon * _trayIcon;
            /*! \brief user */
            QString _userName;
            /*! \brief list path user */
            QList<QString> globalUserPathList;
            /*! \brief Reconstruction du modele*/
            void RebuildModel();
			void addPluginToModel(QString name, TL_plugins& plugin);
            /*! \brief recuperatoin d'une liste de path d'un fichier descripteur */
            QList<QString> getPathsFromFile(QFile *f);

			std::string getPluginLicence() const;

			std::string _pluginLicence;

			QList<QString> _dllWithError;

			ProductLicense * _productLicense;
		public:
            /*! \brief Constructeur */
            _SwPluginsBank_Class(ProductLicense * productLicense = nullptr);
            /*! \brief Destructeur */
            ~_SwPluginsBank_Class();
            //------------------------------------------------------------------
            // Interface ISwPluginsBank
            //------------------------------------------------------------------
            /*! \brief Ajouter un path */
            void AddPath(QString path,bool registerable=false);
            /*! \brief Ajouter un descripteur de paths */
            void AddPaths(QString pathsdescriptor);
            /*! \brief Acces a liste des paths */
            QMap<QString,bool> GetPathList();
            /*! \brief Acces a liste des plugins usine par path */
            QSet<ISwPluginFactory *> & GetPluginList(QString path)  throw(SwException);
            /*! \brief Acces a liste de tous les plugins */
            QMap<QString,SwPluginFactory_Class *> * GetAllPlugins();
            /*! \brief Acces a la liste des noms de tous les composants controllers relatif a un type donné*/
			QList< QPair<QString, QString> > GetControllersListForType(int type_identifier);
            /*! \brief Acces a la description d'un composant */
            QString GetComponentDescription(QString plugin_name, QString component_name) throw(SwException);
            /*! \brief Acces a l'icone d'un composant */
            QIcon GetComponentIcon(QString plugin_name, QString component_name) throw(SwException);
            /*! \brief Creation d'un composant a partir de son nom*/
            SwComponent_Class * CreateComponent(QString plugin_name, QString component_name) throw(SwException);
            /*! \brief Relire le contenu d'un plugin (pour les plugins dont le contenu a changer)*/
            virtual void RereadPluginContent(SwPluginFactory_Class * plugin) throw(SwException);
            /*! \brief Acces au modèle pour l'affichage*/
            virtual QAbstractItemModel * GetModel();
            /*! \brief Acces a la liste des noms de tous les composants */
            QSet<SwUUID> GetDataList();
            /*! \brief Acces au nom du type d'une donnée definie par son id */
            QString GetDataTypeNameFromDataTypeId(const SwUUID & id);
            /*! \brief Creation d'une data */
            SwData_Class * CreateData(const SwUUID & id) throw(SwException);
             /*! \brief Creation d'un objet */
            void * CreateObject(const SwUUID & uuid,void * parameter) throw(SwException);
            /*! \brief Enregsitrement d'une fabrique d' objet */
            void RegisterObjectFactory(const SwUUID & uuid,ISwObjectFactory * factory) throw(SwException);
            /*! \brief Enregsitrement d'une fabrique d' objet */
            void UnregisterObjectFactory(const SwUUID & uuid) throw(SwException);
           //------------------------------------------------------------------
            // Interface QAbstractTableModel
            //------------------------------------------------------------------
            /*! \brief Returns the number of columns for the children of the given parent */
            virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Returns the number of row for the children of the given parent */
            virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Returns the data stored under the given role for the item referred to by the index */
            virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            /*! \brief Returns the index of the item in the model specified by the given row, column and parent index */
            virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Returns the parent of the item in the model specified*/
            virtual QModelIndex parent ( const QModelIndex & index ) const;
            /*! \brief Returns the data for the given role and section in the header with the specified orientation */
            virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;

			void beginInsertRow(int index, _SwTreeItem * node);
			void endInsertRow();

            //----------------------------------------------------
            //-- Gestion des Creations Post Processor
            //----------------------------------------------------
            /*! \brief Ajoute un post processeur*/
            void AddCreationPostProcessor(ISwCreationPostProcessor * post_processor);
            /*! \brief Supprime un post processeur*/
            void RemoveCreationPostProcessor(ISwCreationPostProcessor * post_processor);
            /*! \brief Indique qu'il y a eu une mise a jour */
            bool updateHaveDone();
            /*! \brief Affiche les mises a jours */
            void displayUpdate();
        public slots:
            /*! \brief Affiche les mises a jours */
            void hideDisplayUpdate();
        private:
            /*! \brief Operation de mise a jour */
            void doUpdate(QString path);
        };
	}
}
#endif 
