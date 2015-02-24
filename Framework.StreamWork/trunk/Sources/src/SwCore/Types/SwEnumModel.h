/*!
 \file SwEnumModel.h
 \brief Modele QT correspondant a un SwEnum (principal usage -> edition SwEnum par combobox Qt)
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#ifndef _SwEnumModel_H
#define _SwEnumModel_H
/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QList>
#include <QString>
#include <QVariant>
#include <QDataStream>
#include <QAbstractItemModel>

/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "SwEnum.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
        \class SwEnumModel
        \date 12/04/2006
        \brief  Modele QT correspondant a un SwEnum (principal usage -> edition SwEnum par combobox Qt)
        \author  BIG
        \version 1.0
        @ingroup SwCoreGrp
         */
 
        class BUILD_SWCORE SwEnumModel : public QAbstractItemModel{
			Q_OBJECT
        private:
			//Enumere de stockage du contenu
            SwEnum _content;
            //Liste pont avec la map de l'enumere
            QList<int> _bridge_list;
        public:
            /*! \brief Constructeur de base */
            SwEnumModel(SwEnum * source,QObject * parent=0);
            /*! \brief Destructeur */
            virtual ~SwEnumModel();

            /*! \brief Acces valeur interne enumere*/
            SwEnum GetEnum() const;
            /*! \brief Acces valeur pour un index donné*/
            int GetVal(int index) const;
            /*! \brief Acces index pour une valeur donné*/
            int GetIndex(int val) const;
            //-----------------------------------------------------------------------------------
            // Implementation QAbstractItemModel
            //-----------------------------------------------------------------------------------
            /*! \brief Compteur de ligne */
            virtual int rowCount(const QModelIndex &parent) const;
            /*! \brief Compteur de colonne */
            virtual int columnCount(const QModelIndex &parent) const;
            /*! \brief enfants existants*/
            virtual bool hasChildren(const QModelIndex &parent) const
            { return rowCount(parent) > 0; }

             /*! \brief Acces Parent */
            virtual QModelIndex parent(const QModelIndex &index) const;
             /*! \brief Acces Index */
            virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
             /*! \brief Acces contenu */
            virtual QVariant data(const QModelIndex &index, int role) const;
             /*! \brief Definition contenu */
            virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
             /*! \brief Flags items */
            virtual Qt::ItemFlags flags(const QModelIndex &index) const;

        };
	}
}

//---------------------------------------------------------------------------
#endif
