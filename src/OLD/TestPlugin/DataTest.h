/*!
 \file DataTest.h
 \brief classe representant les données echangées sous forme de buffer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _DataTest_H
#define _DataTest_H

/*
  * INCLUDES GLOBAUX
  */
#include <QByteArray>
/*
  * INCLUDES LOCAUX
  */
#include <SwData_Class.h>
#include <SwException.h>
#include <SwRefPtr.h>
#include "TestPluginConstantes.h"
#include "SwMacros.h"

#define CG_DATA_TEST "DataTest"
#define CG_DATA_TEST_ITEST "DataTestITest"
TestPlugin_DECLARE_UUID(CG_DATA_TEST_ID)

class ITest {
public:
    virtual ~ITest() {}
    virtual int getVal()=0;
};

class TestImpl : public ITest {
public:
    int val;
    TestImpl(int v) {val=v;}
    virtual ~TestImpl() {}
    virtual int getVal() {
        return val;   
    };
};


/*!
 \class DataTest 
 \brief classe representant les données echangées sous forme de buffer
 Commentaire a faire
*/
class TestPlugin_EXPORT DataTest : public  StreamWork::SwCore::SwData_Class 
{
private:
    /*! \brief Contenu */
	QString  _content;
public:
    ITest * _itest;
protected:
	//---------------------------------------------------------------------
	// Serialisation interne
	//---------------------------------------------------------------------
	/*! \brief Lecture */
	void InternalRead(QDataStream * data) throw(StreamWork::SwCore::SwException);
	/*! \brief Ecriture */
	void InternalWrite(QDataStream * data);
	//---------------------------------------------------------------------
	// Creation
	//---------------------------------------------------------------------
	/*! \brief Creation */
	virtual StreamWork::SwCore::SwData_Class * CreateInstance();
public:
	/*! \brief Constructeur */
	DataTest();
	/*! \brief Constructeur */
	~DataTest();
	//---------------------------------------------------------------------
	// Identification
	//---------------------------------------------------------------------
	/*! \brief Identification de la donnée par chaine de caractere
	A surcharger*/
	virtual QString GetType();
	/*! \brief Identification de la donnée par valeur
	A surcharger*/
	virtual StreamWork::SwCore::SwUUID GetTypeId();
	//---------------------------------------------------------------------
	// specifique DataTest
	//---------------------------------------------------------------------
	/*! \brief Definition du contenu */
    //void SetContent(const QByteArray & source);
	/*! \brief Definition du contenu */
    void SetContent(QString source);
	/*! \brief Acces au contenu */
    //const QByteArray & GetContent();
	/*! \brief Acces au contenu */
    QString GetContent();

    QString toString();

};
#endif 
