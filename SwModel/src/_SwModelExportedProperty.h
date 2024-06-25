/*!
 \file _SwModelExportedProperty.h
 \brief Class implementant une entite exportable property
 \version 1.0
 \date 23-août-2006 18:59:26
 \author F.Bighelli
*/

#ifndef __SwModelExportedProperty_H
#define __SwModelExportedProperty_H

#include <QString>
#include "Component/Base/SwComponent_Class.h"
#include "_SwModelExportedEntity.h"
#include "Properties/ISwProperty.h"
#include "Component/Services/ISwProperties.h"
#include "Tools/Signal/LibIndeSig.h"
#include "Component/Services/ServiceImpl/SwProperties_Class.h"

using namespace StreamWork::SwCore;
/*!
	\class _SwModelExportedProperty 
	\brief Class implementant une entite exportable property
*/
class _SwModelExportedProperty : public _SwModelExportedEntity{
	public:
		/*! \brief Service de propriété interne*/
		ISwProperties * _internal_properties;
		/*! \brief Propriete interne*/
		ISwProperty * _internal_property;
		/*! \brief Service de propriété externe*/
		SwProperties_Class * _external_properties;
		/*! \brief Propriete  externe*/
		ISwProperty * _external_property;
		/*! \brief Desactivation de la propagation du changement (evite les rebouclages)*/
        bool _propagate_change_on;
	protected:
	    /*! \brief Specific bind */
		virtual void SpecificBind();
	    /*! \brief Defait la liaison */
		virtual void SpecificUnbind();
	public:
		/*! \brief Constructeur */
		_SwModelExportedProperty();
	    /*! \brief Destructeur */
		virtual ~_SwModelExportedProperty();
	    /*! \brief Slot de connexion */
		void OnInternalChange(ISwProperty * p);
	    /*! \brief Slot de connexion */
		void OnInternalVisibleChange(ISwProperty * p);
	    /*! \brief Slot de connexion */
		void OnInternalEditableChange(ISwProperty * p);
	    /*! \brief Slot de connexion */
		void OnExternalChange(ISwProperty * p);
	    /*! \brief Sur creation d'une propriété */
		void OnCreateProperty(ISwProperties *ps,ISwProperty *p);
	    /*! \brief Sur destruction d'une propriété */
		void OnDestroyProperty(ISwProperties *ps,ISwProperty *p);
		

};

#endif
