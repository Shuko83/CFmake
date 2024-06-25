/*!
\file SwUUID.cpp
\date 12/04/2006
\brief Definition d'un UUID
\author  Big
\version 1.0
 */

#include "Types/SwUUID.h"
#include <QUuid>
#include <QStringList>
#include <QDataStream>

using namespace StreamWork::SwCore;

_SwUUID::_SwUUID(qint64 m,qint64 l):mostSigBits(m),leastSigBits(l) {

}
//-----------------------------------------------------------------------
_SwUUID::_SwUUID( qint64 l ):mostSigBits(Q_INT64_C(0)),leastSigBits(l)
{

}
_SwUUID::_SwUUID():mostSigBits(Q_INT64_C(0)),leastSigBits(Q_INT64_C(0)) {

}
bool _SwUUID::operator<(const struct _SwUUID & val) const {
  if (mostSigBits<val.mostSigBits)
    return true;
  if (mostSigBits>val.mostSigBits)
    return false;
  return leastSigBits<val.leastSigBits;
} 

bool _SwUUID::operator==(const struct _SwUUID & val) const {
  if (&val==this) return true;
  return mostSigBits==val.mostSigBits && leastSigBits==val.leastSigBits;
}  

bool _SwUUID::operator!=(const struct _SwUUID & val) const {
  if (&val==this) return false;
  return !(mostSigBits==val.mostSigBits && leastSigBits==val.leastSigBits);

}

struct _SwUUID _SwUUID::generateUUID() {
	GUID val=(GUID)QUuid::createUuid(); 	
	struct _SwUUID *  ptr=(struct _SwUUID * )&val;
	return *ptr;
}


 /**
@param value */
std::string _SwUUID::toString() const{
	char tmp[300];
	sprintf(tmp,"%I64d,%I64d",mostSigBits,leastSigBits);
	return tmp;
}
QString _SwUUID::toQString() const{
	char tmp[300];
	sprintf(tmp,"%I64d,%I64d",mostSigBits,leastSigBits);
	return tmp;
}
void _SwUUID::FromQString(const QString & value) {
    QStringList v=value.split(",");
    if (v.size()!=2)
        return;
    mostSigBits=v[0].toLongLong();
    leastSigBits=v[1].toLongLong();
}

//---------------------------------------------------------------------------------
struct _SwUUID _SwUUID::CreateFromQString( const QString & value )
{
	SwUUID id;
	id.FromQString(value);
	return id;
}

//JS Hash Function
uint qHash(const StreamWork::SwCore::SwUUID & uuid) {
   char * tmp=(char *)&uuid;

   unsigned int hash = 1315423911;
   int lenght=sizeof(SwUUID);
  
   for(int i = 0; i < lenght ; i++)
   {
      hash ^= ((hash << 5) + (unsigned int)tmp[i] + (hash >> 2));
   }

   return hash;

}      

BUILD_SWCORE QDataStream &operator<<(QDataStream &out, const StreamWork::SwCore::SwUUID &myObj) {
    out<<myObj.mostSigBits;
    out<<myObj.leastSigBits;
    return out;
}
BUILD_SWCORE QDataStream &operator>>(QDataStream &in, StreamWork::SwCore::SwUUID &myObj) {
    qint64 tmp;
    in>>tmp;
    myObj.mostSigBits=tmp;
    in>>tmp;
    myObj.leastSigBits=tmp;
    return in;

}
