/*!
\file SwUUID.h
\date 12/04/2006
\brief Definition d'un UUID
\author  Big
\version 1.0
 */

#ifndef _SwUUID_H
#define _SwUUID_H


#include <QMetaType>
//#include <QVariant>
#include <QDataStream>
#include "SwCoreConstantes.h"
//#include "SwMacros.h"
#include "Types/SwUUID_Simplified.h"



BUILD_SWCORE uint qHash(const StreamWork::SwCore::SwUUID & uuid);

Q_DECLARE_METATYPE(StreamWork::SwCore::SwUUID)

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwUUID &myObj);
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwUUID &myObj);

#endif
