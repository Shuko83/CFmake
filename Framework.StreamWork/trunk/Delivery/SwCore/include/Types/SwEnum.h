/*!
 \file SwEnum.h
 \brief Classe pour la gestion des enumeres
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#ifndef _SwEnum_H
#define _SwEnum_H
/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDataStream>
#include <QAbstractItemModel>

/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "SwRefPtr.h"

namespace StreamWork
{
	namespace SwCore
	{
        class _SwEnumKeys_Class;
        class SwEnumModel;
        /*!
        \class SwEnum
        \date 12/04/2006
        \brief Classe d'enumeration
        \author  BIG
        \version 1.0
        @ingroup SwCoreGrp
        \note Permet de convertir une propriete Qt enum ou flag en QVariant
        \warning la methode qRegisterMetaType et qRegisterMetaTypeStreamOperators
        doivent etre appelé si usage de SwEnum dans les signals/slots
        */
 
        class BUILD_SWCORE SwEnum {
        private:
            /*! \brief Valeur de l'enumere */
            int _i;
            /*! \brief indicateur flag */
            bool _is_flag;
            /*! \brief liste des valeurs d'enumere */
            SwRefPtr<_SwEnumKeys_Class> _keys_values_pt;
        public:
            /*! \brief Constructeur de base */
            SwEnum();
            /*! \brief Constructeur de copie */
            SwEnum(const SwEnum & source);
            /*! \brief Destructeur */
            virtual ~SwEnum();

            /*! \brief Operateur d'affectation */
            SwEnum & operator=(const SwEnum& source);
            /*! \brief Operateur de comparaison*/
            bool operator==(const SwEnum& source) const;
            /*! \brief Operateur d'affectation par un int*/
            void FromInt(const int source);
			/*! \brief Operateur d'affectation par un QString (ne fait rien si ne trouve pas la valeur)*/
			void FromString(const QString val,Qt::CaseSensitivity caseS = Qt::CaseSensitive);
            /*! \brief de recuperation de la valeur par un int*/
            int ToInt() const;
            /*! \brief de recuperation de la valeur par une string*/
            QString ToString() const;
            /*! \brief permet de savoir si l'enumere est valide*/
            bool IsValid() const;

            /*! \brief Acces aux valeurs possible pour l'enumere*/
            const QMap<int,QString> * GetValues() const;
            /*! \brief isFlag si true alors les valeurs peuvent etre combinées avec un |*/
            bool IsFlag() const;
            
            /*! Indique que cet enumere est un flag */
            void ChangeFlagStatus(bool is_flag);
            /*! Ajoute une clef d'enumere */
            void AddKey(int idx,QString enum_string);
            /*! Supprime une clef d'enumere */
            void RemoveKey(int idx);

            /*! demande de model */
            SwEnumModel * QueryModel(QObject * parent=0);
            /*! affectation du contenu du model à l'enumere + indice de selection (cas non flag)
            Rq:Le modele est fonctionnlement equivalent a l'enumeré*/
            void AffectModel(SwEnumModel * model,int index=-1);

        };
	}
}

Q_DECLARE_METATYPE(StreamWork::SwCore::SwEnum)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwEnum &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwEnum &myObj);
//---------------------------------------------------------------------------
#endif
