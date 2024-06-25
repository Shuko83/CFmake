/*!
 \file SwDataBasicBuffer_Class.h
 \brief classe representant les données echangées sous forme de buffer
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef _SwDataBasicBuffer_Class_H
#define _SwDataBasicBuffer_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QByteArray>
/*
  * INCLUDES LOCAUX
  */
#include "Component/Pin/SwData_Class.h"
#include "Tools/Exception/SwException.h"
#include "SwRef/SwRefPtr.h"
#include "SwDatasBaseConstantes.h"
#include "Main/SwMacros.h"

#define CG_SW_DATA_BASIC_BUFFER_NAME "BasicBuffer"
SWDATASBASE_DECLARE_UUID(CG_SW_DATA_BASIC_BUFFER_ID)
/*!
 \class SwDataBasicBuffer_Class 
 \brief classe representant les données echangées sous forme de buffer
 Commentaire a faire
*/
class SWDATASBASE_EXPORT SwDataBasicBuffer_Class : public  StreamWork::SwCore::SwData_Class 
{
private:
    /*! \brief Contenu */
	std::string  _content;
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
	SwDataBasicBuffer_Class();
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
	// specifique SwDataBasicBuffer_Class
	//---------------------------------------------------------------------
	/*! \brief Definition du contenu */
    //void SetContent(const QByteArray & source);
	/*! \brief Definition du contenu */
    void SetContent(const char * source, int size);
	/*! \brief Acces au contenu */
    //const QByteArray & GetContent();
	/*! \brief Acces au contenu */
    const char * GetContentBuffer();
	/*! \brief Acces a la taille du contenu */
    int GetContentBufferSize();

};
#endif 
