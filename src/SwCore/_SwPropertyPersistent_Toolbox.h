/*!
\file _SwPropertyPersistent_Toolbox.h
\date 18/04/2006
\brief boite a outil pour l'enregistrement des propriétés
\author  Big
\version 1.0
 */

#ifndef __SwPropertyPersistent_Toolbox_H
#define __SwPropertyPersistent_Toolbox_H
/*
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QByteArray>
/*
 * INCLUDES LOCAUX
 */
#include "ISwProperties.h"

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \class _SwPropertyPersistent_Toolbox
        \brief  boite a outil pour l'enregistrement des propriétés
        \note
        */

        class _SwPropertyPersistent_Toolbox  {
        public:
	        /*! \brief methode permettant de charger une propriété
            \param[in] property_node noeud propriete potentiel
            \param[in] liste des propriétés
            */
	        static void LoadProperty(QDomElement & property_node,ISwProperties * properties);
	        /*! \brief methode permettant de sauver une propriété 
            \param[in] parent_property_node noeud parent
            \param[in] doc document parent
            \param[in] name nom de la propriétés
            \param[in] liste des propriétés
            */
	        static void SaveProperty(QDomElement & parent_property_node,QDomDocument &doc,QString name,ISwProperties * properties);
	        /*! \brief methode permettant de sauver une propriété meme si elle a changer
            \param[in] parent_property_node noeud parent
            \param[in] doc document parent
            \param[in] name nom de la propriétés
            \param[in] liste des propriétés
            \param[in] force la sauvegarde meme si la propriété n'a pas changer
            */
	        static void SavePropertyExtended(QDomElement & parent_property_node,QDomDocument &doc,QString name,ISwProperties * properties,bool forceSave);
       };
    }
}

#endif
