/*!
 \file DataTest.cpp
 \brief classe representant les données echangées sous forme de buffer
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#include <QtGlobal>
#include <QDataStream>
#include <QBuffer>
#include "DataTest.h"
#include "SwMacros.h"


using namespace StreamWork::SwCore;

SW_DEFINE_UUID(CG_DATA_TEST_ID,-7863916104866970092,-7687833170389311553)

/*
 * CONSTANTES
 */

/*! \brief Constructeur */
DataTest::DataTest():SwData_Class(){
    _itest=0;
}
DataTest::~DataTest() {
    if (_itest!=0)
        delete _itest;
}
//---------------------------------------------------------------------
// Identification
//---------------------------------------------------------------------
/*! \brief Identification de la donnée par chaine de caractere
A surcharger*/
QString DataTest::GetType(){
	return CG_DATA_TEST;
}
/*! \brief Identification de la donnée par valeur
A surcharger*/
SwUUID DataTest::GetTypeId(){
	return CG_DATA_TEST_ID;
}
//---------------------------------------------------------------------
// Serialisation interne
//---------------------------------------------------------------------
/*! \brief Lecture */
void DataTest::InternalRead(QDataStream * data) throw(SwException){
    //(*data)<<_content;
}
/*! \brief Ecriture */
void DataTest::InternalWrite(QDataStream * data){
    //(*data)>>_content;
}
//---------------------------------------------------------------------
// Creation
//---------------------------------------------------------------------
/*! \brief Creation */
SwData_Class * DataTest::CreateInstance(){
    return new DataTest();
}
//---------------------------------------------------------------------
// specifique DataTest
//---------------------------------------------------------------------
/*! \brief Definition du contenu */
//void SetContent(const QByteArray & source);
/*! \brief Definition du contenu */
void DataTest::SetContent(QString source) {
    _content=source;
}
/*! \brief Acces au contenu */
//const QByteArray & GetContent();
/*! \brief Acces au contenu */
QString DataTest::GetContent(){
    return _content;
}

QString DataTest::toString() {
    if (_itest==0) {
        return QString("DataTest:%1").arg(_content,5);
    }
    return QString("DataTest:%1 ITest:%2").arg(_content,5).arg(_itest->getVal());
}
