/*!
 \file SwDataBasicBuffer_Class.cpp
 \brief classe representant les données echangées sous forme de buffer
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QDataStream>
#include <QBuffer>
#include "SwDataBasicBuffer_Class.h"
#include "Main/SwMacros.h"


using namespace StreamWork::SwCore;

SW_DEFINE_UUID(CG_SW_DATA_BASIC_BUFFER_ID,2028794819854485715,-6120365880156251503)

/*
 * CONSTANTES
 */

/*! \brief Constructeur */
SwDataBasicBuffer_Class::SwDataBasicBuffer_Class():SwData_Class(){
}
//---------------------------------------------------------------------
// Identification
//---------------------------------------------------------------------
/*! \brief Identification de la donnée par chaine de caractere
A surcharger*/
QString SwDataBasicBuffer_Class::GetType(){
	return CG_SW_DATA_BASIC_BUFFER_NAME;
}
/*! \brief Identification de la donnée par valeur
A surcharger*/
SwUUID SwDataBasicBuffer_Class::GetTypeId(){
	return CG_SW_DATA_BASIC_BUFFER_ID;
}
//---------------------------------------------------------------------
// Serialisation interne
//---------------------------------------------------------------------
/*! \brief Lecture */
void SwDataBasicBuffer_Class::InternalRead(QDataStream * data) throw(SwException){
    //(*data)<<_content;
}
/*! \brief Ecriture */
void SwDataBasicBuffer_Class::InternalWrite(QDataStream * data){
    //(*data)>>_content;
}
//---------------------------------------------------------------------
// Creation
//---------------------------------------------------------------------
/*! \brief Creation */
SwData_Class * SwDataBasicBuffer_Class::CreateInstance(){
    return new SwDataBasicBuffer_Class();
}
//---------------------------------------------------------------------
// specifique SwDataBasicBuffer_Class
//---------------------------------------------------------------------
/*! \brief Definition du contenu */
/*void SwDataBasicBuffer_Class::SetContent(const QByteArray & source) {
    _content=source;
} */
/*! \brief Definition du contenu */
void SwDataBasicBuffer_Class::SetContent(const char * source, int size){
	_content.append(source,size);
}
/*! \brief Acces au contenu */
/*const QByteArray & SwDataBasicBuffer_Class::GetContent(){
    return _content;
} */
/*! \brief Acces au contenu */
const char * SwDataBasicBuffer_Class::GetContentBuffer(){
	return _content.data();
}
/*! \brief Acces a la taille du contenu */
int SwDataBasicBuffer_Class::GetContentBufferSize(){
    return _content.size();
}

