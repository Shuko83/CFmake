/*!
 \file SwSaver_Class.h
 \brief Implementation of the Class SwSaver_Class
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli

 \note Principe enregistre un neoud racine dans une structure xml
 \note Implementation
 Enregistre neoud header nommé streamlib
 Enregistre le neoud racine dans un node nommé "component" et deux attributs name=" " et factory_name=""
 Enregistre les services du neoud racine dans un node nommé "service" et un attribut service_name=" "
  (ils doivent implementer ISwPersistent)
 Enregistre les composant enfants du neoud (algo identique au neoud racine)

 \note sur l'attribut factory_name est recuperer au niveau du composant
 par la methode GetFactoryComponentName si la valeur renvoyé est nulle ou vide le composant est un
 SwComponent_Class
*/

#ifndef _SwSaver_Class_H
#define _SwSaver_Class_H

/*
  * INCLUDES GLOBAUX
  */
#include <QDomDocument>
#include <QList>
#include <QXmlStreamWriter>
/*
  * INCLUDES LOCAUX
  */
#include "SwApplication.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwSaver_Class 
		 \brief class de sauvegarde du stream
        @ingroup SwCoreGrp
		*/
		class BUILD_SWCORE SwSaver_Class 
		{
		public:
			/*! Constructeur */
			SwSaver_Class();
			/*! Destructeur */
			~SwSaver_Class();
			/*! Sauvegarde */
			void Save(SwComponent_Class * root_component, QXmlStreamWriter& writer);
			/*! Sauvegarde groupe */
			void SaveGroup(QList<SwComponent_Class *>& components, QXmlStreamWriter& writer);
			/*! Sauvegarde model */
			void SaveModel(QList<SwComponent_Class *>& components, QXmlStreamWriter& writer);
		private:
			/*! Construction de la definition du stream au format xml */
			void BuildXMLStream(SwComponent_Class * component, QXmlStreamWriter& writer);
		};
	}
}
#endif 
