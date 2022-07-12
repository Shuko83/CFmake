/**
@file SwPropertiesPersistentToolbox.cpp
@brief Boite a outils publique pour l'enregistrement des propriétés
@author F.Bighelli
 */

#include "SwPropertiesPersistentToolbox.h"
#include "SwPropertyPersistentToolbox.h" 
#include "ISwProperty.h"

using namespace StreamWork::SwCore;

/*! \brief methode permettant de charger des donnees de propriétés*/
void SwPropertiesPersistentToolbox::Load(QDomElement & elt,ISwProperties * properties) {
    for(QDomElement elt_property = elt.firstChildElement(); !elt_property.isNull(); elt_property = elt_property.nextSiblingElement())
    {
        SwPropertyPersistentToolbox::LoadProperty(elt_property,properties);
    }

}

/*! \brief methode permettant de sauver des donnees de propriétés */
void SwPropertiesPersistentToolbox::Save(QXmlStreamWriter &writer,ISwProperties * properties) {
    for (auto it = properties->GetProperties().constBegin(); it != properties->GetProperties().constEnd(); ++it) {
        SwPropertyPersistentToolbox::SavePropertyExtended(writer, (*it)->GetRealName(), properties, true);
    }
}
