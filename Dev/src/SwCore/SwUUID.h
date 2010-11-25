/*!
\file SwUUID.h
\date 12/04/2006
\brief Definition d'un UUID
\author  Big
\version 1.0
 */

#ifndef _SwUUID_H
#define _SwUUID_H

#include <QtGlobal>
#include <QVariant>
#include <QDataStream>
#include "SwCoreConstantes.h"
#include "SwMacros.h"


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
		  _SwUUID();
          bool operator<(const struct _SwUUID & val) const;
          bool operator==(const struct _SwUUID & val) const;
          bool operator!=(const struct _SwUUID & val) const;
          std::string toString() const;
          QString toQString() const;
          void FromQString(const QString & value);
		  static struct _SwUUID generateUUID();
        };
        typedef  struct _SwUUID SwUUID;

        
    }
}

BUILD_SWCORE uint qHash(const StreamWork::SwCore::SwUUID & uuid);

Q_DECLARE_METATYPE(StreamWork::SwCore::SwUUID)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwUUID &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwUUID &myObj);

#endif