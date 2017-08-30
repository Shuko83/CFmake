/*!
 \file _SwEditorStreamTreeModel.h
 \brief Implementation d'un modèle QT du stream en cours d'edition
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwEditorStreamTreeModel_H
#define __SwEditorStreamTreeModel_H

/*
  * INCLUDES GLOBAUX
  */
#include <Qt>
#include <QAbstractItemModel>
#include <QIcon>
#include <QFont>
#include <QColor>
/*
  * INCLUDES LOCAUX
  */
#include <ISwObserver.h>
#include <ISwQAbstractItemViewSlots.h>
#include <SwComponent_Class.h>
#include "ISwEditorStreamNavigation.h"
#include "ISwEditorStreamOperations.h"
#include "ISwEditorStreamsManager.h"

namespace StreamWork
{
	namespace SwEditor
	{
        
        /*!
	        \class _SwEditorApplicativeCore 
	        \brief Coeur de traitement de l'editeur
        */
        class _SwEditorStreamTreeModel : public QAbstractItemModel, SwCore::ISwObserver, SwGui::ISwQAbstractItemViewSlots{
			Q_OBJECT
		private:
			/*! \brief manager */
			ISwEditorStreamsManager * _manager;
			/*! \brief manager */
			ISwEditorStreamNavigation * _navigator;
			/*! \brief manager */
			ISwEditorStreamOperations * _operations;
            /*! \brief host */
            SwCore::SwComponent_Class * _current_host;
            /*! \brief host */
            SwCore::SwComponent_Class * _current_selection;
            /*! icone pour l'edition */
            QIcon _ico_edit;
            /*! font pour l'edition */
            QFont _font_ed;
            /*! color pour l'edition */
            QColor _color_ed;
            /*! font pour la selection */
            QFont _font_sel;
            /*! color pour la selection */
            QColor _color_sel;
            /*! font pour la non selection */
            QFont _font_notsel;
            /*! color pour la non selection */
            QColor _color_notsel;
            /*! font pour le reste */
            QFont _font_other;
            /*! color pour le reste */
            QColor _color_other;
            /*! Etat boutons */
            Qt::MouseButtons _m_buttons;
            /*! Etat Modificateurs */
            Qt::KeyboardModifiers _m_modifiers;
            /*! Etat Aide */
            QString _help;
		public:
            /*! \brief Constructeur */
			_SwEditorStreamTreeModel(QObject * parent,ISwEditorStreamsManager * manager,ISwEditorStreamNavigation * navigator,ISwEditorStreamOperations * operations);
            /*! \brief Constructeur */
			virtual ~_SwEditorStreamTreeModel();
            /*! \brief forceReset */
            void ForceReset();
            //-------------------------------------------------------------
            // Interface QAbstractItemModel
            //------------------------------------------------------------
            /*! \brief Renvoie les capacites du modèle */
            virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
            /*! \brief Renvoie le nombre de colonnes pour les enfants d'un parent donné */
            virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;			
             /*! \brief Renvoie le nombre de ligne pour un parent donné */
            virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Renvoie les données d'entete */
            virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
            /*! \brief Renvoie les données stockées sous un certain role pour un item nommé index */
            virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            /*! \brief Permet de definir la valeur d'un item*/
            virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
            /*! \brief Renvoie l'item index specifie par la ligne et la colonne pour un parent donné*/
            virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            /*! \brief Renvoie l'item parent d'un item index donné */
            virtual QModelIndex parent ( const QModelIndex & index ) const;
            //-------------------------------------------------------------
            // Interface observeur
            //------------------------------------------------------------
            /*! \brief methode appelée par l'observable*/
			virtual void Update(StreamWork::SwCore::ISwObservable* sender = nullptr);
            //-------------------------------------------------------------
            // Interface ISwQAbstractItemViewSlots
            //------------------------------------------------------------
            /*! \brief sur activation */
			virtual void onActivated ( const QModelIndex & index );
            /*! \brief sur click */
			virtual void onClicked ( const QModelIndex & index );
            /*! \brief sur double click */
			virtual void onDoubleClicked ( const QModelIndex & index );
            /*! \brief sur entree */
			virtual void onEntered ( const QModelIndex & index );
            /*! \brief sur  appuie de la sourie sur l'item */
			virtual void onPressed ( const QModelIndex & index );
            /*! \brief sur entre dans la vue*/
			virtual void onViewportEntered ();
            /*! \brief sur evenement sourie quelconque*/
            virtual void onMouseEvent(QMouseEvent * mouse_event);
            /*! \brief sur demande de l'aide*/
			virtual void onHelp();
            /*! \brief OnMenuRequest*/
            virtual void onMenuRequested( const QPoint & globalpos,QModelIndex index);

		};
	}
}
#endif 
