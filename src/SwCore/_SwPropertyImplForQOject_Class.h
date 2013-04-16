/*!
\file _SwPropertyImplForQOject_Class.h
\date 18/04/2006
\brief implementation generale d'une propriete d'un QObject
\author  Big
\version 1.0
 */

#ifndef __SwPropertyImplForQOject_Class_H
#define __SwPropertyImplForQOject_Class_H
/*
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QMetaProperty>
/*
 * INCLUDES LOCAUX
 */
#include "_SwPropertyImpl_Class.h"
#include "LibIndeSig.h"
#include "SwIconDescriptor.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
        \class _SwPropertyImplForQOject_Class
        \brief  implementation generale d'une propriete d'un QObject
        \note
        */

        class _SwPropertyImplForQOject_Class : public _SwPropertyImpl_Class {
        protected:
            /*! \brief nom real de la propri�t� */
            QString _real_name;
            /*! \brief acces au parent */
            QObject * _host;
            /*! \brief acces a la metaproperty */
            QMetaProperty _metaproperty;
            /*! \brief icondesc si necessaire */
            SwIconDescriptor * _iconDesc;
        public:
            /*! \brief Constructor */
            _SwPropertyImplForQOject_Class(QObject * host,QString name,ISwProperties * hosting_service,QString prefix=QString());
            /*! \brief Destructor */
            virtual ~_SwPropertyImplForQOject_Class();
	        /*! \brief methode de recuperer le nom reel d'une propri�t� */
	        virtual QString GetRealName();
	        /*! \brief methode de recuperer la valeur d'une propri�t� en interne*/
	        QVariant GetInternalValue();
	        /*! \brief methode permettant de definir la valeur d'une propri�t� en interne*/
	        void SetInternalValue (const QVariant & val);
       };
    }
}

#endif
