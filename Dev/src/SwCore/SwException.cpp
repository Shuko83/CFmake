/*!
 \file SwException.cpp
 \brief Implementation of the Class SwException
 \date 23-août-2006 10:35:13
 \author F.Bighelli
*/
/*
  * INCLUDES
  */
#include <stdio.h>

#include "SwException.h"

using namespace StreamWork::SwCore;

/*!
\brief Constructeur
\param[in] component    nom du composant source de l'erreur
\param[in] filename    nom du fichier source de l'erreur
\param[in] line    numero de ligne source de l'erreur
\param[in] reason    raison de l'erreur
*/
SwException::SwException(const QString & component, const QString & filename, int line,const QString & reason)
	:_component(component),_filename(filename),_line(line),_reason(reason),_what_string("")
{
	QString line_string=QString("%1").arg(_line);

	if (!_component.isEmpty()) {
		_what_string+=_component;
		_what_string+=":";
	}
	_what_string+=_filename;
	_what_string+=":";
	_what_string+=line_string;
	_what_string+=":";
	_what_string+=reason;
	_c_what_string=new char[strlen(_what_string.toLatin1().data())+1];
	strcpy(_c_what_string,_what_string.toLatin1().data());

}
/*!
\brief Constructeur de copie
\param[in] source  source de la copie
*/
SwException::SwException(SwException & source ){
	if (&source!=this) {
		_component=source._component;
		_filename=source._filename;
		_line=source._line;
		_reason=source._reason;
		_what_string=source._what_string;
		_c_what_string=new char[strlen(_what_string.toLatin1().data())+1];
		strcpy(_c_what_string,_what_string.toLatin1().data());
	}
}
/*!
\brief Destructeur
*/
SwException::~SwException()  throw(){
	if (_c_what_string!=NULL)
		delete _c_what_string;
}


/*!
\brief Renvoie le nom du composant source de l'erreur
\return e nom du composant source de l'erreur
*/
QString SwException::GetComponent(){
	return _component;
}


/*!
\brief Renvoie le nom du fichier source de l'erreur
\return le nom du fichier source de l'erreur
*/
QString SwException::GetFilename(){
	return _filename;
}


/*!
\brief Renvoie le numero de la ligne source de l'erreur
\return le numero de la ligne source de l'erreur
*/
int SwException::GetLine(){
	return _line;
}


/*!
\brief Renvoie la raison de l'erreur
\return la raison de l'erreur
*/
QString SwException::GetReason(){
	return _reason;
}


/*! \brief Renvoie une chaine de caractères C décrivant la cause générale de l'erreur courante
\return une chaine de caractères C décrivant la cause générale de l'erreur courante
*/
const char * SwException::what(){
	return _c_what_string;
}
