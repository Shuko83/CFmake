/*!
\file _SwUUID_Simplified_H.h
\date 12/04/2006
\brief Definition d'un UUID
\author  Big
\version 1.0
 */

#ifndef _SwUUID_Simplified_H
#define _SwUUID_Simplified_H

#include "SwCoreConstantes.h"

#include <string>
#include <QString>

namespace StreamWork 
{
    namespace SwCore
    {
		/*!
        \class SwUUID
        \brief Definition d'un UUID
        @ingroup SwCoreGrp
        */
        struct BUILD_SWCORE _SwUUID  {
        public:
          qint64 mostSigBits;
          qint64 leastSigBits;
          
		  _SwUUID(qint64 m,qint64 l);
		  _SwUUID(qint64 l);
		  _SwUUID();
          bool operator<(const struct _SwUUID & val) const;
          bool operator==(const struct _SwUUID & val) const;
          bool operator!=(const struct _SwUUID & val) const;
          std::string toString() const;
          QString toQString() const;
          void FromQString(const QString & value);
		  static struct _SwUUID generateUUID();
		  static struct _SwUUID CreateFromQString(const QString & value);
        };

		typedef  struct _SwUUID SwUUID;

        
    }
}

#endif
