/*!
 \file SwData_Class.cpp
 \brief classe representant les données echangées
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QDataStream>
#include <QBuffer>
#include "Component/Pin/SwData_Class.h"
#include "Main/SwApplication.h"
#include "Main/SwMacros.h"

using namespace StreamWork::SwCore;

/*
 * CONSTANTES
 */

/*! \brief Constructeur */
SwData_Class::SwData_Class():SwRef(){
    Channel=0;
    Priority=0;
    Time=0.0;
}

/*! \brief Destructeur */
SwData_Class::~SwData_Class(){
}
//---------------------------------------------------------------------
// Serialisation interne
//---------------------------------------------------------------------
/*! \brief Lecture */
void SwData_Class::InternalRead(QDataStream * data) throw(SwException) {
}
/*! \brief Ecriture */
void SwData_Class::InternalWrite(QDataStream * data){
}
//---------------------------------------------------------------------
// Creation
//---------------------------------------------------------------------
/*! \brief Creation */
SwData_Class * SwData_Class::CreateInstance(){
    return new SwData_Class();
}
//---------------------------------------------------------------------
// Identification
//---------------------------------------------------------------------
/*! \brief Identification de la donnée par chaine de caractere
A surcharger*/
QString SwData_Class::GetType(){
    return CG_SW_DEFAULT_TYPE;
}
/*! \brief Identification de la donnée par valeur
A surcharger*/
SwUUID SwData_Class::GetTypeId(){
    return SwUUID();
}
//---------------------------------------------------------------------
// Clonage
//---------------------------------------------------------------------
/*! \brief Clonage de la donnée */
SwData_Class * SwData_Class::Clone() throw(SwException){
    SwData_Class * target;
    QBuffer buffer;
    QDataStream ds(&buffer);
    Write(&ds);
    buffer.reset();
    target=CreateInstance();
    target->Read(&ds);
    return target;
}
//---------------------------------------------------------------------
// Serialisation
//---------------------------------------------------------------------
/*! \brief Lecture
\warning si les données ne sont pas complete -> exception*/
void SwData_Class::Read(QDataStream * data) throw(SwException){
    SwUUID type_id;
    qint64 size_read;

    if (data->device()==NULL) {
        QString msg=QString("Invalid data stream");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Lecture du type
    (*data)>>type_id.mostSigBits;
    (*data)>>type_id.leastSigBits;
    if (type_id!=GetTypeId()) {
        QString msg=QString("When read data of type %1, data containt an other data type").arg(GetType());
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Lecture des données communes
    (*data)>>Channel;
    (*data)>>Priority;
    (*data)>>Time;
    (*data)>>size_read;
    if (data->device()->size()<size_read) {
        QString msg=QString("Fail to read data: Not enough data in data stream");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Lecture des données specifiques
    InternalRead(data);
}
/*! \brief Lecture avec resolution*/
SwData_Class * SwData_Class::ReadResolve(QDataStream * data) throw(SwException){
    SwUUID type_id;
    qint64 size_read;
    SwData_Class * data_readed=NULL;

    if (data->device()==NULL) {
        QString msg=QString("Invalid data stream");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Lecture du type
    (*data)>>type_id.mostSigBits;
    (*data)>>type_id.leastSigBits;
    //Instanciation du type de données correspondant
    data_readed=SW_APP->ComponentsBank().CreateData(type_id);
    if (data_readed==NULL) {
        QString msg=QString("Fail to read data: Unknown data type");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //Lecture des données communes
    (*data)>>data_readed->Channel;
    (*data)>>data_readed->Priority;
    (*data)>>data_readed->Time;
    (*data)>>size_read;
    if (data->device()->size()<size_read) {
        QString msg=QString("Fail to read data: Not enough data in data stream");
        LAUNCH_SWEXCEPTION("SwCore",msg);
    }
    //lecture des données specifiques
    data_readed->InternalRead(data);
    //Fin
    return data_readed;
}
/*! \brief Ecriture */
void SwData_Class::Write(QDataStream * data){
    SwUUID type_id;
    QBuffer buff;
    QDataStream ds(&buff);
    ds.setByteOrder(data->byteOrder());
    ds.setVersion(data->version());
    //Ecriture dans un buffer temporaire (pour connaitre la taille)
    InternalWrite(&ds);
    //Ecriture des champs parent
    type_id=GetTypeId();
    (*data)<<type_id.mostSigBits;
    (*data)<<type_id.leastSigBits;
    (*data)<<Channel;
    (*data)<<Priority;
    (*data)<<Time;
    //Ecriture de la taille
    (*data)<<buff.size();
    //Ecriture des données propres
    data->writeRawData(buff.data().constData(),buff.size());
}
