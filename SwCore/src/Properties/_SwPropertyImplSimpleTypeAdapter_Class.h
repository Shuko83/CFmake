/*!
\file _SwPropertyImplSimpleTypeAdapter_Class.h
\date 18/04/2006
\brief implementation generale d'une propriete base sur un simple type adapter
\author  Big
\version 1.0
 */

#ifndef __SwPropertyImplSimpleTypeAdapter_Class_H
#define __SwPropertyImplSimpleTypeAdapter_Class_H
/*
  * INCLUDES GLOBAUX
  */
/*
 * INCLUDES LOCAUX
 */
#include "Properties/_SwPropertyImpl_Class.h"
#include "Types/UserType/ISwSimpleTypeAdapter.h"
#include "Tools/Signal/LibIndeSig.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
        \class _SwPropertyImplSimpleTypeAdapter_Class
        \brief  implementation generale d'une propriete base sur un simple type adapter
        \note
        */

        class _SwPropertyImplSimpleTypeAdapter_Class : public _SwPropertyImpl_Class {
        protected:
            /*! \brief acces au parent */
            ISwSimpleTypeAdapter * _adapter;
        public:
            /*! \brief Constructor */
            _SwPropertyImplSimpleTypeAdapter_Class(ISwSimpleTypeAdapter * adapter,QString name,ISwProperties * hosting_service);
            /*! \brief Destructor */
            virtual ~_SwPropertyImplSimpleTypeAdapter_Class();
	        /*! \brief methode de recuperer la valeur d'une propri�t� en interne*/
	        QVariant GetInternalValue();
	        /*! \brief methode permettant de definir la valeur d'une propri�t� en interne*/
	        void SetInternalValue (const QVariant & val);
	        /*! \brief methode est une sous propri�t�*/
	        virtual bool IsSubProperty();
       };
    }
}

#endif
