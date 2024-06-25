/*!
 \file SwException.h
 \brief classe SwException est la mère de toutes les exceptions
 \version 1.0
 \date 23-août-2006 10:35:13
 \author F.Bighelli
*/

#ifndef _SwException_H
#define _SwException_H

#include <QString>
#include <exception>
#include "SwCoreConstantes.h"

namespace StreamWork
{
	namespace SwCore
	{
		/*!
		 \class SwException
		 \brief classe SwException est la mère de toutes les exceptions
        @ingroup SwCoreGrp

         Definit une exception specifique StreamWork

         Au vue de ses propriétés, elle permet - outre de connaitre la raison de l'exception -
         de connaitre a quel endroit a eu lieu l'exception

         Pour générer une exception de ce type utiliser la macro suivante:
         \code
        LAUNCH_SWEXCEPTION("MyComponent","Raison de l'exception");
         \endcode
		*/
		class BUILD_SWCORE SwException : public std::exception
		{
		protected:
			/*! \brief Nom du composant source de l'erreur*/
			QString 			_component;
			/*! \brief Nom du fichier source de l'erreur*/
			QString 			_filename;
			/*! \brief numero de la ligne source de l'erreur*/
			int		 	 		_line;
			/*! \brief raison de l'erreur*/
			QString 			_reason;
			/*! \brief what_string*/
			QString 			_what_string;
			/*! \brief what_string c_format*/
			char * 				_c_what_string;
		public:
			/*! \brief Constructeur */
			SwException(const QString & component, const QString & filename, int line, const QString & reason);
			/*! \brief Constructeur de copie */
			SwException(SwException & source );
			/*! \brief Destructeur */
			virtual ~SwException() throw();

			/*! \brief Renvoie le nom du composant source de l'erreur */
			virtual QString GetComponent();
			/*! \brief Renvoie le nom du fichier source de l'erreur */
			virtual QString GetFilename();
			/*! \brief Renvoie le numero de la ligne source de l'erreur */
			virtual int     GetLine();
			/*! \brief Renvoie la raison de l'erreur */
			virtual QString GetReason();
			/*! \brief Renvoie une chaine de caractères C décrivant la cause générale de l'erreur courante */
			virtual const char * what();
		};
	}
}
#endif
