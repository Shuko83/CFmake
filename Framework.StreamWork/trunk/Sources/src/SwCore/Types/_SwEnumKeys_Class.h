/*!
 \file _SwEnumKeys_Class.h
 \brief Classe pour la gestion des enumeres
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

*/

#ifndef __SwEnumKeys_Class_H
#define __SwEnumKeys_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QMap>
#include <QString>
/*
  * INCLUDES LOCAUX
  */
#include "SwCoreConstantes.h"
#include "SwRef.h"

namespace StreamWork
{
	namespace SwCore
	{

        /*!
        \class SwEnum
        \date 12/04/2006
        \brief Classe d'enumeration
        \author  BIG
        \version 1.0
        \note
        */

	    class _SwEnumKeys_Class : public SwRef
        {
        public:
            QMap<int,QString> _enum_values;
        public:
            _SwEnumKeys_Class();
        protected:
            ~_SwEnumKeys_Class();
        };

    }
}
#endif
