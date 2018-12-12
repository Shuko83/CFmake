/*!
\file SwPropertyPersistentToolbox.h
\date 18/04/2006
\brief boite a outil pour l'enregistrement des propriétés
\author  Big
\version 1.0
 */

#ifndef _SwPropertyPersistentToolbox_H
#define _SwPropertyPersistentToolbox_H
/*
  * INCLUDES GLOBAUX
  */
#include <QObject>
#include <QByteArray>
#include <QVariant>
/*
 * INCLUDES LOCAUX
 */
#include "ISwProperties.h"

namespace StreamWork 
{
	namespace SwCore
	{
        /*!
        \class SwPropertyPersistentToolbox
        \brief  boite a outil pour l'enregistrement des propriétés
        \note
        */

        class BUILD_SWCORE SwPropertyPersistentToolbox  {
        public:
	        /*! \brief methode permettant de charger une propriété via un ISwProperties
            \param[in] property_node noeud propriete potentiel
            \param[in] liste des propriétés
            */
	        static void LoadProperty(QDomElement & property_node,ISwProperties * properties);

			/*! \brief methode permettant de charger une propriété via une ISwProperty
            \param[in] "property_node" noeud propriete potentiel
            \param[in] propriété
            */
	        static void LoadProperty(QDomElement & property_node, ISwProperty * property);

			/*! \brief methode permettant de sauver une propriété  via une ISwProperty
            \param[in] "parent_property_node" noeud parent
            \param[in] "doc" document parent
            \param[in] "propCustomName" nom de la propriétés
            \param[in] propriété
            \param[in] "prefix" prefix de la conf pour la sauvegarde
            */
			static void SaveProperty(QDomElement & parent_property_node, QDomDocument &doc, QString propCustomName, ISwProperty * property, QString prefix, QVariant overWriteValue = QVariant());
			
			/*! \brief methode permettant de sauver une propriété  via une ISwProperties
            \param[in] "writer" StreamWriter handle
            \param[in] "name" nom de la propriétés
            \param[in] liste des propriétés
            */
	        static void SaveProperty(QXmlStreamWriter &writer, QString name, ISwProperties * properties);

	        /*! \brief methode permettant de sauver une propriété meme si elle a changer
            \param[in] "writer" StreamWriter handle
            \param[in] "name" nom de la propriétés
            \param[in] liste des propriétés
            \param[in] force la sauvegarde meme si la propriété n'a pas changé
            */
	        static void SavePropertyExtended(QXmlStreamWriter &writer, QString name, ISwProperties * properties, bool forceSave);
				        
			static QVariant createQVariantFromString(ISwProperty* prop, QString value);

		private:
			/*  @brief : méthode interne pour sauvegarder une property au bon format selon son type
			 *			 appellée lors du LoadProperty
			 *	@param : property_node, noeud parent auque ajouté la valeur de la property
			 *	@param : inProperty, propiété
			 */
			static void setProperty(QDomElement & property_node, ISwProperty * inProperty) ;

			/* @brief : méthode interne pour récupérer la valeur d'une property au bon format selon son type
			 *			appellée lors du SaveProperty
			 * @param : property_node, noeud parent auque ajouté la valeur de la property
			 * @param : doc, QDomDocument associé
			 * @param : inProperty, propiété
			 * @param : elt, QDomElement concernant la propriété
			 * @param : var, QVariant contenant la valeur de la propriété
			 */
			static void createProperty(QDomElement & parent_property_node, QDomDocument &doc, ISwProperty * inProperty, QDomElement &elt, QVariant var);

			static void createProperty(QXmlStreamWriter &writer, ISwProperty * inProperty, QVariant var);
       };
    }
}

#endif
