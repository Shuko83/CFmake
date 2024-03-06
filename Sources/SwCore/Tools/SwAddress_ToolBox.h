/*!
 \file SwAddress_ToolBox.h
 \brief Implementation of the Class SwAddress_ToolBox
 \version 1.0
 \date 24-août-2006 12:21:05
 \author F.Bighelli
*/

#ifndef _SwAddress_Class_H
#define _SwAddress_Class_H

/*
  * INCLUDES LOCAUX
  */
#include "Component/Base/SwComponent_Class.h"

namespace StreamWork
{
	namespace SwCore
	{
        /*!
		 \class SwAddress_ToolBox 
		 \brief Address d'une cible par rapport a une source travers le stream
        @ingroup SwCoreGrp

         Cette classe n'est pas instanciable. C'est une boite a outils qui contient des
         methodes statiques permettant de gerer le mecanisme d'adresse

        Pour connaitre l'adresse absolue d'un composant
        \code
        my_path=SwAddress_ToolBox::BuildAbsolutePath(my_component);
        \endcode
        Pour connaitre l'adresse relative d'un composant par rapport a un autre composant
        \code
        my_path=SwAddress_ToolBox::BuildRelativePath(my_component_source,my_component_target);
        \endcode
        Pour connaitre l'adresse universelle d'un composant par rapport a un autre composant
        (Est utile lorsque le composant cible et le composant source ne sont pas dans le même stream)
        \code
        my_path=SwAddress_ToolBox::BuildUniversalPath(my_component);
        \endcode
        Pour retrouver un componsant cible à partir d'un path (relatif,absolue,universel) et d'un
        composant source
        \code
        try {
            my_component_target=SwAddress_ToolBox::FindTarget(mypath,my_component_source);
        } catch (SwException &e) {
            //Si nous sommes la, c'est que l'adresse n'est pas valide
        }
        if (my_component_target==NULL) {
            //Aucun composant n'a été trouvé à l'adresse donnée
        }
        \endcode

		*/
		class BUILD_SWCORE SwAddress_ToolBox
		{
        private:
            /*! \brief Constructeur */
            SwAddress_ToolBox(){}
            /*! \brief Destructeur*/
            ~SwAddress_ToolBox(){}
        public:
            /*! \brief Construit le chemin absolu jusqu'a la cible*/
            static QString BuildAbsolutePath(SwComponent_Class *target);
            /*! \brief Construit le chemin relatif de la source jusqu'a la cible*/
            static QString BuildRelativePath(SwComponent_Class *source,SwComponent_Class *target) throw(SwException);
             /*! \brief Construit un chemin universel jusqu'a la cible (utile si source et cible ne sont pas dans le même stream)*/
            static QString BuildUniversalPath(SwComponent_Class *target);
            /*! \brief Trouve la cible a partir de la source et du path*/
            static SwComponent_Class * FindTarget(QString path,SwComponent_Class *source) throw(SwException) ;
       };
	}
}
#endif 
